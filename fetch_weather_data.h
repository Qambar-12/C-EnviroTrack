#ifndef FETCH_WEATHER_DATA_H
#define FETCH_WEATHER_DATA_H
#include <stdlib.h>
size_t dataWriteHandler(void *contents, size_t size, size_t nmemb, char **output);
char* get_api_key(const char* file_path);
char *executeHttpRequest(const char *output_filename);
#endif //FETCH_WEATHER_DATA_H
