#include "ts/packet.h"

// Packet header

void TsPacketHeader::setSyncByte(short value) {
    syncByte = value;
}

short TsPacketHeader::getSyncByte() {
    return syncByte;
}

void TsPacketHeader::setErrorIndicator(short value) {
    errorIndicator = value;
}

short TsPacketHeader::getErrorIndicator() {
    return errorIndicator;
}

void TsPacketHeader::setUnitStartIndicator(short value) {
    unitStartIndicator = value;
}

short TsPacketHeader::getUnitStartIndicator() {
    return unitStartIndicator;
}

void TsPacketHeader::setTransportPriority(short value) {
    transportPriority = value;
}

short TsPacketHeader::getTransportPriority() {
    return transportPriority;
}

void TsPacketHeader::setPid(short value) {
    pid = value;
}

short TsPacketHeader::getPid() {
    return pid;
}

void TsPacketHeader::setTransportScramblingControl(short value) {
    transportScramblingControl = value;
}

short TsPacketHeader::getTransportScramblingControl() {
    return transportScramblingControl;
}

void TsPacketHeader::setAdaptationFieldControl(short value) {
    adaptationFieldControl = value;
}

short TsPacketHeader::getAdaptationFieldControl() {
    return adaptationFieldControl;
}

void TsPacketHeader::setContinuityCounter(short value) {
    continuityCounter = value;
}

short TsPacketHeader::getContinuityCounter() {
    return continuityCounter;
}

void TsPacketHeader::setAdaptationFieldLength(short value) {
    adaptationFieldLength = value;
}

short TsPacketHeader::getAdaptationFieldLength() {
    return adaptationFieldLength;
}

void TsPacketHeader::setDiscontinuityIndicator(short value) {
    discontinuityIndicator = value;
}

short TsPacketHeader::getDiscontinuityIndicator() {
    return discontinuityIndicator;
}

void TsPacketHeader::setRandomAccessIndicator(short value) {
    randomAccessIndicator = value;
}

short TsPacketHeader::getRandomAccessIndicator() {
    return randomAccessIndicator;
}

void TsPacketHeader::setElementaryStreamPriorityIndicator(short value) {
    elementaryStreamPriorityIndicator = value;
}

short TsPacketHeader::getElementaryStreamPriorityIndicator() {
    return elementaryStreamPriorityIndicator;
}

void TsPacketHeader::setPcrFlag(short value) {
    pcrFlag = value;
}

short TsPacketHeader::getPcrFlag() {
    return pcrFlag;
}

void TsPacketHeader::setOpcrFlag(short value) {
    opcrFlag = value;
}

short TsPacketHeader::getOpcrFlag() {
    return opcrFlag;
}

void TsPacketHeader::setSplicingPointFlag(short value) {
    splicingPointFlag = value;
}

short TsPacketHeader::getSplicingPointFlag() {
    return splicingPointFlag;
}

void TsPacketHeader::setTransportPrivateDataFlag(short value) {
    transportPrivateDataFlag = value;
}

short TsPacketHeader::getTransportPrivateDataFlag() {
    return transportPrivateDataFlag;
}

void TsPacketHeader::setAdaptationFieldExtensionFlag(short value) {
    adaptationFieldExtensionFlag = value;
}

short TsPacketHeader::getAdaptationFieldExtensionFlag() {
    return adaptationFieldExtensionFlag;
}

void TsPacketHeader::setPcrBase(long long value) {
    pcrBase = value;
}

long long TsPacketHeader::getPcrBase() {
    return pcrBase;
}

void TsPacketHeader::setPcrExt(short value) {
    pcrExt = value;
}

short TsPacketHeader::getPcrExt() {
    return pcrExt;
}

void TsPacketHeader::setOpcrBase(long long value) {
    opcrBase = value;
}

long long TsPacketHeader::getOpcrBase() {
    return opcrBase;
}

void TsPacketHeader::setOpcrExt(short value) {
    opcrExt = value;
}

short TsPacketHeader::getOpcrExt() {
    return opcrExt;
}

void TsPacketHeader::setSplicingCountdown(short value) {
    splicingCountdown = value;
}

short TsPacketHeader::getSplicingCountdown() {
    return splicingCountdown;
}

void TsPacketHeader::setTransportPrivateDataLength(short value) {
    transportPrivateDataLength = value;
}

short TsPacketHeader::getTransportPrivateDataLength() {
    return transportPrivateDataLength;
}

// Packet

TsPacket::TsPacket() {
    header = new TsPacketHeader();
}

TsPacket::~TsPacket() {
    delete header;
}

TsPacketHeader *TsPacket::getHeader() {
    return header;
}

std::vector<unsigned char>& TsPacket::getBytes() {
    return bytes;
}