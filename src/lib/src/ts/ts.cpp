#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <deque>
#include <cstdlib>
#include <cmath>
#include "ts/ts.h"
#include "bit.h"
#include "logger.h"

TransportStream::TransportStream() {
    loaded = false;

    insideTsPacket = false;

    parserByteCounter = 0;

    tsFileReadIndex = 0;

    fileSize = 0;

    tsFd = NULL;

    listener = NULL;

    packetOffsets.clear();

    config = Config::getInstance();

    clearPacketBuffer();
}

TransportStream::~TransportStream() {
    close();
}

void TransportStream::setPath(std::string path) {
    Logger::debug("TransportStream::setPath(%s)", path.c_str());

    if (path.length() < 0) {
        Logger::debug("O caminho do arquivo não pôde ser configurado. O caminho informado não é válido.");
        return;
    }

    this->path = path;
}

std::deque<int>& TransportStream::getPacketOffsets() {
    return packetOffsets;
}

int TransportStream::load() {
    Logger::debug("TransportStream::load()");

    loaded = false;

    packetOffsets.clear();

    if (path.length() <= 0) {
        Logger::debug("O caminho do arquivo informado não é válido.");
        return -1;
    }

    tsFd = fopen(path.c_str(), "rb");

    if (!tsFd) {
        Logger::debug("Não foi possível abrir o arquivo %s", path.c_str());

        return -1;
    }

    // Get file size

    fseek(tsFd, 0, SEEK_END);

    fileSize = ftell(tsFd);

    Logger::debug("Tamanho do arquivo: %ld", fileSize);

    if (listener != NULL) {
        ((TransportStreamListener *) listener)->callbackOnLoad(this);
    }

    loaded = true;

    return 0;
}

long TransportStream::getFileSize() {
    return fileSize;
}

void TransportStream::clearPacketBuffer() {
    memset(streamPacket, 0, TS_PACKET_SIZE);
}

int TransportStream::loadPacketOffsets() {
    Logger::debug("TransportStream::loadPacketOffsets()");

    unsigned char byte;

    packetOffsets.clear();

    // Seek to the begining of the file

    rewind(tsFd);

    int bytes_counter = 0;

    for (int tsFileReadIndex = 0; tsFileReadIndex < fileSize; tsFileReadIndex++) {
        bytes_counter++;

        fread(&byte, 1, 1, tsFd);

        if (byte == 0x47) {
            //Logger::debug("Bouquet name descriptor");

            packetOffsets.push_back(tsFileReadIndex);

            // Set state to inside the TS packet container

            insideTsPacket = true;

            // Packet size (188) - Sync byte size (8) = 180

            // Not a complete TS packet (8 bytes * 22 = 180 bytes)

            if (tsFileReadIndex + (TS_PACKET_SIZE - 1) > fileSize) {
                Logger::error("Incomplete TS Packet. Residual bytes: " + (fileSize - tsFileReadIndex));
                return -1;
            }

            unsigned char streamPacket[TS_PACKET_SIZE - 1];

            // Read packet content

            int readBytes = fread(streamPacket, 1, TS_PACKET_SIZE - 1, tsFd);

            if (readBytes != TS_PACKET_SIZE - 1) {
                Logger::error("Unexpected incomplete TS Packet. Cannot read it to the end.");
                return -1;
            }

            /*
            printf("read bytes: %d\n", readBytes);

            for (int i = 0; i < readBytes; i++) {
                printf("%02x ", streamPacket[i]);
            }
            printf("\n");
            */

            tsFileReadIndex = tsFileReadIndex + (TS_PACKET_SIZE - 1);
        }
    }

    Logger::debug("TS packets found: %u", packetOffsets.size());

    if (listener != NULL) {
        ((TransportStreamListener *) listener)->callbackOnLoadPacketOffsets(this);
    }

    return 0;
}

int TransportStream::parsePacket(unsigned int index, TsPacket *packet) {
    Logger::debug("TransportStream::parsePacket(index=%d)", index);

    if (index > packetOffsets.size() - 1) {
        Logger::error("Índice de pacotes maior do que o máximo registrado para o arquivo.");
        return -1;
    }

    if (packetOffsets[index] > fileSize) {
        Logger::error("Offset do pacote maior do que o offset máximo do arquivo.");
        return -2;
    }

    if (!packet) {
        Logger::error("Pacote TS inválido.");
        return -3;
    }

    packet->getBytes().clear();

    unsigned char byte;

    fseek(tsFd, packetOffsets[index], SEEK_SET);

    fread(&byte, 1, 1, tsFd);

    packet->getBytes().push_back(byte);

    //if (byte != 0x47) {
        // TODO: Log error
    //    return;
    //}

    // Packet size (188) - Sync byte size (8) = 180

    // Not a complete TS packet (8 bytes * 22 = 180 bytes)

    if (packetOffsets[index] + (TS_PACKET_SIZE - 1) > fileSize) {
        // TODO: Log error to GUI level
        Logger::error("Pacote TS incompleto no fim do arquivo, bytes residuais: %d", (fileSize - packetOffsets[index]));
        return 0;
    }

    // Packet bytes

    clearPacketBuffer();

    // Read packet content

    int readBytes = fread(streamPacket, 1, TS_PACKET_SIZE - 1, tsFd);

    if (readBytes < TS_PACKET_SIZE - 1) {
        Logger::debug("Leitura incompleta do pacote TS. A partir do byte %d do arquivo foi possível ler somente %d byte(s)", packetOffsets[index], readBytes);
    }

    TsPacketHeader *header = packet->getHeader();

    // Put bytes in the raw attribute of packet

    if (config->hasKey("show-packet-binary-info") && config->getValueBool("show-packet-binary-info")) {
        Logger::debug("Packet content - sync byte (187 bytes):");
    }

    for (int i = 0; i < (TS_PACKET_SIZE - 1); i++) {
        packet->getBytes().push_back(streamPacket[i]);

        if (config->hasKey("show-packet-binary-info") && config->getValueBool("show-packet-binary-info")) {
            Logger::debug("%.3d %s 0x%.2x (%d)", i, Bit::toBinary((short) streamPacket[i]).c_str(), streamPacket[i], streamPacket[i]);
        }
    }

    Logger::debug("Packet header:");

    // Error Indicator (1 bit)

    header->setErrorIndicator(streamPacket[0] >> 7);

    Logger::debug("%d = errorIndicator", header->getErrorIndicator());

    // Payload Unit Start Indicator (1 bit)

    header->setUnitStartIndicator((streamPacket[0] >> 6) & 0x1);

    Logger::debug("%d = unitStartIndicator", header->getUnitStartIndicator());

    // Transport Priority (1 bit)

    header->setTransportPriority((streamPacket[0] >> 5) & 0x1);

    Logger::debug("%d = transportPriority", header->getTransportPriority());

    // PID (13 bits)

    header->setPid((((short) streamPacket[0] & 0x1F) << 8) | streamPacket[1]);

    Logger::debug("%x (%d) = PID", header->getPid(), header->getPid());

    // Transport Scrambling Control (2 bits)

    header->setTransportScramblingControl((streamPacket[2] >> 6) & 0x3);

    Logger::debug("%d = transportScramblingControl", header->getTransportScramblingControl());

    // Transport Scrambling Control (2 bits)

    header->setAdaptationFieldControl((streamPacket[2] >> 4) & 0x3);

    Logger::debug("%d = adaptationFieldControl", header->getAdaptationFieldControl());

    // Continuity Counter (4 bits)

    header->setContinuityCounter(streamPacket[2] & 0xF);

    Logger::debug("%x (%d) = continuityCounter", header->getContinuityCounter(), header->getContinuityCounter());

    // Adaptation field control == 10 || 11: Parse adaptation field
    if (header->getAdaptationFieldControl() == 2 || header->getAdaptationFieldControl() == 3) {
        processAdaptationField(packet);
    }

    // Adaptation field control == 01 || 11: Parse data bytes
    if (header->getAdaptationFieldControl() == 1 || header->getAdaptationFieldControl() == 3) {
        Logger::debug("Parse data bytes");
    }

    return 0;
}

int TransportStream::processAdaptationField(TsPacket *packet) {
    if (packet == NULL) {
        Logger::error("TransportStream::processAdaptationField(): Packet is invalid.");
        return -1;
    }

    TsPacketHeader *header = packet->getHeader();

    Logger::debug("Parse adaptation field");

    // Adaptation field length (8 bits)
    header->setAdaptationFieldLength(streamPacket[3]);

    Logger::debug("adaptationFieldLength = %d", header->getAdaptationFieldLength());

    if (header->getAdaptationFieldLength() > 0) {
        // Error Indicator (1 bit)

        header->setDiscontinuityIndicator(streamPacket[4] >> 7);

        Logger::debug("discontinuityIndicator = %d", header->getDiscontinuityIndicator());

        // Random access indicator (1 bit)

        header->setRandomAccessIndicator((streamPacket[4] >> 6) & 0x1);

        Logger::debug("randomAccessIndicator = %d", header->getRandomAccessIndicator());

        // Elementary stream priority indicator (1 bit)

        header->setElementaryStreamPriorityIndicator((streamPacket[4] >> 5) & 0x1);

        Logger::debug("elementaryStreamPriorityIndicator = %d", header->getElementaryStreamPriorityIndicator());

        // PCR flag (1 bit)

        header->setPcrFlag((streamPacket[4] >> 4) & 0x1);

        Logger::debug("pcrFlag = %d", header->getPcrFlag());

        // OPCR flag (1 bit)

        header->setOpcrFlag((streamPacket[4] >> 3) & 0x1);

        Logger::debug("opcrFlag = %d", header->getOpcrFlag());

        // Splicing point flag (1 bit)

        header->setSplicingPointFlag((streamPacket[4] >> 2) & 0x1);

        Logger::debug("splicingPointFlag = %d", header->getSplicingPointFlag());

        // Transport private data flag (1 bit)

        header->setTransportPrivateDataFlag((streamPacket[4] >> 1) & 0x1);

        Logger::debug("transportPrivateDataFlag = %d", header->getTransportPrivateDataFlag());

        // Transport private data flag (1 bit)

        header->setAdaptationFieldExtensionFlag(streamPacket[4] & 0x1);

        Logger::debug("adaptationFieldExtensionFlag = %d", header->getAdaptationFieldExtensionFlag());

        if (header->getPcrFlag() == 1) {
            Logger::debug("PCR-base parts = %d %d %d %d %d", streamPacket[5], streamPacket[6], streamPacket[7], streamPacket[8], streamPacket[9] >> 7);

            // PCR (33 bits)

            Logger::debug("streamPacket[5] << 26 = %u (%s)", streamPacket[5] << 26, Bit::toBinary(streamPacket[5] << 26).c_str());
            Logger::debug("streamPacket[5] << 17 = %u (%s)", streamPacket[5] << 17, Bit::toBinary(streamPacket[5] << 17).c_str());
            Logger::debug("streamPacket[5] << 9 = %u (%s)", streamPacket[5] << 9, Bit::toBinary(streamPacket[5] << 9).c_str());
            Logger::debug("(streamPacket[8] << 1) | (streamPacket[9] >> 7 = %u (%s)", (streamPacket[8] << 1) | (streamPacket[9] >> 7), Bit::toBinary((streamPacket[8] << 1) | (streamPacket[9] >> 7)).c_str());

            long long pcrBase = (streamPacket[5] << 26) | (streamPacket[6] << 17) | (streamPacket[7] << 9) | (streamPacket[8] << 1) | (streamPacket[9] >> 7);

            header->setPcrBase(pcrBase);

            Logger::debug("PCR_base = %ld (0x%lx)", header->getPcrBase(), header->getPcrBase());

            // Resersed (6 bits)
            // Skip

            // PCR extension (9 bits)
            header->setPcrExt((streamPacket[9] & 1 << 8) | streamPacket[10]);

            Logger::debug("PCR_ext = %d (0x%x)", header->getPcrExt(), header->getPcrExt());
        }

        if (header->getOpcrFlag() == 1) {
            long long opcrBase = (streamPacket[11] << 26) | (streamPacket[12] << 17) | (streamPacket[13] << 9) | (streamPacket[14] << 1) | (streamPacket[15] >> 7);

            header->setOpcrBase(opcrBase);

            Logger::debug("OPCR_base = %ld (0x%lx)", header->getOpcrBase(), header->getOpcrBase());

            // Resersed (6 bits)
            // Skip

            // PCR extension (9 bits)
            header->setPcrExt((streamPacket[15] & 1 << 8) | streamPacket[16]);

            Logger::debug("OPCR_ext = %d (0x%x)", header->getPcrExt(), header->getPcrExt());
        }

        if (header->getSplicingPointFlag() == 1) {
            header->setSplicingCountdown(streamPacket[17]);

            Logger::debug("splicingCountdown = %d (0x%x)", header->getSplicingCountdown(), header->getSplicingCountdown());
        }

        int currentIndex = 18;

        if (header->getTransportPrivateDataFlag() == 1) {
            short transportPrivateDataLength = streamPacket[currentIndex];

            Logger::debug("transportPrivateDataLength = %d", transportPrivateDataLength);

            for (int transportPrivateDataIndex = 0; transportPrivateDataIndex < transportPrivateDataLength; transportPrivateDataIndex++) {
                short transportPrivateData = streamPacket[++currentIndex];

                Logger::debug("transportPrivateData = %d (0x%x)", transportPrivateData, transportPrivateData);
            }
        }

        if (header->getAdaptationFieldExtensionFlag() == 1) {
            short adaptationFieldExtensionLength = streamPacket[++currentIndex];

            Logger::debug("adaptationFieldExtensionLength = %d", adaptationFieldExtensionLength);
        }
    }

    return 0;
}

int TransportStream::parse() {
    unsigned char byte;

    // Seek to the begining of the file

    rewind(tsFd);

    tsFileReadIndex = 0;
    packetCounter = 0;

    std::cerr << "Reading..." << std::endl;

    int bytes_counter = 0;

    for (tsFileReadIndex = 0; tsFileReadIndex < fileSize; tsFileReadIndex++) {
        bytes_counter++;

        fread(&byte, 1, 1, tsFd);

        if (byte == 0x47) {
            //Logger::debug("Bouquet name descriptor");

            ++packetCounter;

            printf("TS PACKET %d\n", packetCounter);

            // Set state to inside the TS packet container

            insideTsPacket = true;

            // Packet size (188) - Sync byte size (8) = 180

            // Not a complete TS packet (8 bytes * 22 = 180 bytes)

            if (tsFileReadIndex + (TS_PACKET_SIZE - 1) > fileSize) {
                Logger::error("Incomplete TS Packet. Residual bytes: " + (fileSize - tsFileReadIndex));
                return -1;
            }

            unsigned char streamPacket[TS_PACKET_SIZE - 1];

            // Read packet content

            int readBytes = fread(streamPacket, 1, TS_PACKET_SIZE - 1, tsFd);

            printf("read bytes: %d\n", readBytes);

            for (int i = 0; i < readBytes; i++) {
                printf("%02x ", streamPacket[i]);
            }
            printf("\n");

            tsFileReadIndex = tsFileReadIndex + (TS_PACKET_SIZE - 1);

            // Error Indicator (1 byte)

            short errorIndicator = streamPacket[0] >> 7;

            printf("\t%d = errorIndicator\n", errorIndicator);

            // Payload Unit Start Indicator (1 byte)

            short unitStartIndicator = (streamPacket[0] >> 6) & 0x1;

            printf("\t%d = unitStartIndicator\n", unitStartIndicator);

            // Transport Priority (1 byte)

            short transportPriority = (streamPacket[0] >> 5) & 0x1;

            printf("\t%d = transportPriority\n", transportPriority);

            // PID (13 bytes)

            short pid = (streamPacket[0] << 11) | streamPacket[1];

            printf("\t%x (%d) = PID\n", pid, pid);

            // Transport Scrambling Control (2 bytes)

            short transportScramblingControl = (streamPacket[2] >> 6) & 0x3;

            printf("\t%d = transportScramblingControl\n", transportScramblingControl);

            // Transport Scrambling Control (2 bytes)

            short adaptationFieldControl = (streamPacket[2] >> 4) & 0x3;

            printf("\t%d = adaptationFieldControl\n", adaptationFieldControl);

            // Continuity Counter

            short continuityCounter = streamPacket[2] & 0xF;

            printf("\t%x (%d) = continuityCounter\n", continuityCounter, continuityCounter);
        }
    }

    return 0;
}

void TransportStream::close() {
    if (tsFd != NULL) {
        fclose(tsFd);
    }
}

void TransportStream::setListener(TransportStreamListener *listener) {
    this->listener = listener;
}

bool TransportStream::isLoaded() {
    return loaded;
}