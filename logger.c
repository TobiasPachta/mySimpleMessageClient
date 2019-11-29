//
// Created by tas on 29.11.2019.
//

#include "logger.h"

void logMessage(char *message){
    fprintf(stderr, "Error: %s", message);
}