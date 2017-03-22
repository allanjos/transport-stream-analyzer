#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <vector>

// TS Packet header

class TsPacketHeader {
private:

    short syncByte;

    // Error Indicator (1 byte)
    short errorIndicator;

    // Payload Unit Start Indicator (1 byte)
    short unitStartIndicator;

    // Transport Priority (1 byte)
    short transportPriority;

    // PID (13 bytes)
    short pid;

    // Transport scrambling control (2 bytes)
    short transportScramblingControl;

    // Adaptation field control (2 bytes)
    short adaptationFieldControl;

    // Continuity counter
    short continuityCounter;

    // Adaptation field length
    short adaptationFieldLength;

    short discontinuityIndicator;

    short randomAccessIndicator;

    short elementaryStreamPriorityIndicator;

    short pcrFlag;

    short opcrFlag;

    short splicingPointFlag;

    short transportPrivateDataFlag;

    short adaptationFieldExtensionFlag;

    long long pcrBase;

    short pcrExt;

    long long opcrBase;

    short opcrExt;

    short splicingCountdown;

    short transportPrivateDataLength;

public:

    // Sync byte
    void setSyncByte(short value);
    short getSyncByte();

    // Error Indicator (1 byte)
    void setErrorIndicator(short value);
    short getErrorIndicator();

    // Payload Unit Start Indicator (1 byte)
    void setUnitStartIndicator(short value);
    short getUnitStartIndicator();

    // Transport Priority (1 byte)
    void setTransportPriority(short value);
    short getTransportPriority();

    // PID (13 bytes)
    void setPid(short value);
    short getPid();

    // Transport Scrambling Control (2 bytes)
    void setTransportScramblingControl(short value);
    short getTransportScramblingControl();

    // Transport Scrambling Control (2 bytes)
    void setAdaptationFieldControl(short value);
    short getAdaptationFieldControl();

    // Continuity Counter
    void setContinuityCounter(short value);
    short getContinuityCounter();

    // Adaptation field length (size: 8 bits)
    void setAdaptationFieldLength(short value);
    short getAdaptationFieldLength();

    void setDiscontinuityIndicator(short value);
    short getDiscontinuityIndicator();

    void setRandomAccessIndicator(short value);
    short getRandomAccessIndicator();

    void setElementaryStreamPriorityIndicator(short value);
    short getElementaryStreamPriorityIndicator();

    void setPcrFlag(short value);
    short getPcrFlag();

    void setOpcrFlag(short value);
    short getOpcrFlag();

    void setSplicingPointFlag(short value);
    short getSplicingPointFlag();

    void setTransportPrivateDataFlag(short value);
    short getTransportPrivateDataFlag();

    void setAdaptationFieldExtensionFlag(short value);
    short getAdaptationFieldExtensionFlag();

    void setPcrBase(long long value);
    long long getPcrBase();

    void setPcrExt(short value);
    short getPcrExt();

    void setOpcrBase(long long value);
    long long getOpcrBase();

    void setOpcrExt(short value);
    short getOpcrExt();

    void setSplicingCountdown(short value);
    short getSplicingCountdown();

    void setTransportPrivateDataLength(short value);
    short getTransportPrivateDataLength();
};

// TS Packet

class TsPacket
{
private:

    TsPacketHeader *header;

    std::vector<unsigned char> bytes;

public:

    TsPacket();

    ~TsPacket();

    std::vector<unsigned char>& getBytes();

    TsPacketHeader *getHeader();
};

#endif