#ifndef TS_H
#define TS_H

#include <deque>
#include "ts/packet.h"
#include "config.h"

#define TS_PACKET_SIZE 188

class TransportStream; // Forward declaration

class TransportStreamListener {
public:
    virtual void callbackOnLoad(TransportStream *ts)=0;

    virtual void callbackOnLoadPacketOffsets(TransportStream *ts)=0;
};

class TransportStream {
private:

    bool loaded;

    std::string path;

    unsigned int packetCounter;

    std::deque<int> packetOffsets;

    bool insideTsPacket;

    int parserByteCounter;

    long tsFileReadIndex;

    long fileSize;

    FILE *tsFd;

    void *listener;

    Config *config;

    // Packet bytes
    unsigned char streamPacket[TS_PACKET_SIZE - 1];

    void clearPacketBuffer();

    int processAdaptationField(TsPacket *packet);

public:

    TransportStream();

    ~TransportStream();

    void setPath(std::string path);

    bool isLoaded();

    std::deque<int>& getPacketOffsets();

    int loadPacketOffsets();

    int load();

    long getFileSize();

    int parse();

    int parsePacket(unsigned int index, TsPacket *packet);

    void close();

    void setListener(TransportStreamListener *listener);
};

#endif // TS_H