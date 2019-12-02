//
// Created by tas on 29.11.2019.
//

#include "logger.h"

void logMessage(const char *message){
    fprintf(stderr, "Error: %s", message);
}
