#ifndef FILE_READER_H
#define FILE_READER_H

#define FILE_READER_BUFFER_SIZE 1024

class FileReaderChunk {
  public:
    char buffer [FILE_READER_BUFFER_SIZE];
    unsigned int bytes;
};

class FileReader {
private:
    FileReaderChunk chunk;

public:
    void setArguments(int argc, char *argv[]);

    char *getArgument(std::string name);

    FileReaderChunk *getChunk();
};

#endif // FILE_READER_H