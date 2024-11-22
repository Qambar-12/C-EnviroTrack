#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "fetch_weather_data.h"
#include "process_weather_data.h"
#include "assign_level_and_trigger.h"
#include "cJSON/cJSON.h"

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
    ProcessedData data = process_weather_data(response, "weather_data.csv");
    int danger_level = assign_level(data,"weather_data.csv");
    trigger_alert(danger_level);


    return 0;
}
