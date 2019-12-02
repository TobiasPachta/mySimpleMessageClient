//
// Created by tas on 29.11.2019.
//

#include "logger.h"

void logMessage(const char *message){
    fprintf(stdout, "Error: %s\n", message);
}

void logError(const char *message){
    fprintf(stderr, "Error: %s\n", message);
}