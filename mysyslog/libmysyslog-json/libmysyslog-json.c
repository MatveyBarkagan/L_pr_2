#include "mylib.h"
#include <stdio.h>
#include <string.h>

const char* level_to_string(int level) {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARN: return "WARN";
        case ERROR: return "ERROR";
        case CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void format_message(const char* msg, int level, int format, char* buffer, size_t buffer_size) {
    if (format == FORMAT_PLAIN) {
        snprintf(buffer, buffer_size, "[%s] %s", level_to_string(level), msg);
    } else if (format == FORMAT_JSON) {
        snprintf(buffer, buffer_size, "{\"level\": \"%s\", \"message\": \"%s\"}", level_to_string(level), msg);
    } else {
        snprintf(buffer, buffer_size, "%s", msg);
    }
}

int mysyslog(const char* msg, int level, int format, const char* path) {
    FILE* logfile = fopen(path, "a");
    if (!logfile) {
        return -1; 
    }

    char formatted_msg[1024];
    format_message(msg, level, format, formatted_msg, sizeof(formatted_msg));
    
    if (fprintf(logfile, "%s\n", formatted_msg) < 0) {
        fclose(logfile);
        return -1; // Ошибка записи в файл
    }

    fclose(logfile);
    return 0; 
}
