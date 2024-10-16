#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// Уровни сообщений
#define MYSYSLOG_EMERG   0
#define MYSYSLOG_ALERT   1
#define MYSYSLOG_CRIT    2
#define MYSYSLOG_ERROR   3
#define MYSYSLOG_WARNING 4
#define MYSYSLOG_NOTICE  5
#define MYSYSLOG_INFO    6
#define MYSYSLOG_DEBUG   7

// Форматы сообщений
#define MYSYSLOG_TEXT    0
#define MYSYSLOG_JSON    1
#define MYSYSLOG_XML     2

// Драйверы вывода
#define MYSYSLOG_STDOUT  0
#define MYSYSLOG_FILE    1
#define MYSYSLOG_SYSLOG  2

// Функция вывода сообщения в журнал
int mysyslog(const char* msg, int level, int driver, int format, const char* path) {
    // Проверка параметров
    if (msg == NULL || level < 0 || level > MYSYSLOG_DEBUG || driver < 0 || driver > MYSYSLOG_SYSLOG || format < 0 || format > MYSYSLOG_XML || (driver == MYSYSLOG_FILE && path == NULL)) {
        return -1;
    }

    // Получение текущего времени
    time_t timestamp = time(NULL);
    struct tm* timeinfo = localtime(&timestamp);

    // Форматирование сообщения в зависимости от формата
    char formatted_msg[1024];
    switch (format) {
        case MYSYSLOG_TEXT:
            snprintf(formatted_msg, sizeof(formatted_msg), "[%s] %s: %s", asctime(timeinfo), mysyslog_level_to_string(level), msg);
            break;
        case MYSYSLOG_JSON:
            snprintf(formatted_msg, sizeof(formatted_msg), "{\"level\": \"%s\", \"timestamp\": \"%s\", \"message\": \"%s\"}", mysyslog_level_to_string(level), asctime(timeinfo), msg);
            break;
        case MYSYSLOG_XML:
            snprintf(formatted_msg, sizeof(formatted_msg), "<log><level>%s</level><timestamp>%s</timestamp><message>%s</message></log>", mysyslog_level_to_string(level), asctime(timeinfo), msg);
            break;
    }

    // Вывод сообщения в журнал в зависимости от драйвера
    switch (driver) {
        case MYSYSLOG_STDOUT:
            printf("%s", formatted_msg);
            break;
        case MYSYSLOG_FILE:
            FILE* file = fopen(path, "a");
            if (file != NULL) {
                fprintf(file, "%s\n", formatted_msg);
                fclose(file);
            }
            break;
        case MYSYSLOG_SYSLOG:
            // TODO: Реализовать поддержку syslog
            break;
    }

    return 0;
}

// Функция преобразования уровня сообщения в строку
const char* mysyslog_level_to_string(int level) {
    switch (level) {
        case MYSYSLOG_EMERG:   return "EMERG";
        case MYSYSLOG_ALERT:   return "ALERT";
        case MYSYSLOG_CRIT:    return "CRIT";
        case MYSYSLOG_ERROR:   return "ERROR";
        case MYSYSLOG_WARNING: return "WARNING";
        case MYSYSLOG_NOTICE:  return "NOTICE";
        case MYSYSLOG_INFO:    return "INFO";
        case MYSYSLOG_DEBUG:   return "DEBUG";
        default:               return "UNKNOWN";
    }
}


int main() {
    // Вывод сообщения в журнал в текстовом формате на стандартный вывод
    mysyslog("Привет, мир!", MYSYSLOG_INFO, MYSYSLOG_STDOUT, MYSYSLOG_TEXT, NULL);

    // Вывод сообщения в журнал в JSON-формате в файл
    mysyslog("Это JSON-сообщение", MYSYSLOG_INFO, MYSYSLOG_FILE, MYSYSLOG_JSON, "log.json");

    return 0;
}
