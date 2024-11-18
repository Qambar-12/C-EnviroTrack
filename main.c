#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>
#include "fetch_weather_data.h"
//#include "cJSON/cJSON.h"

#define ResponseFile "raw_response.txt"
int main(void)
{
    printf("Fetching weather data...\n");
    char *response = executeHttpRequest(ResponseFile);
    if (response == NULL) {
        fprintf(stderr, "Failed to fetch weather data\n");
        return 1;
    }
    printf("Weather data fetched successfully\n");
    return 0;
}
