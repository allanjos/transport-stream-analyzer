#ifndef MAIN_H
#define MAIN_H

#include "application.h"

typedef enum ts_markers_e {
  BOUQUET_NAME_DESCRIPTOR = 0x47,
} ts_markers_t;

bool inside_ts_packet = false;

void terminate();

void printUsage(const char *program_name);

int parseProgramArgs(Application application, int argc, char *argv[]);

#endif // MAIN_H