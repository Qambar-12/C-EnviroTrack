#ifndef PROCESS_WEATHER_DATA_H
#define PROCESS_WEATHER_DATA_H
typedef struct {
    float heat_index;
    float dew_point;
} ProcessedData;
ProcessedData process_weather_data(const char *raw_response, const char *output_file);
void write_csv_headers(FILE *output);
double extract_value(const char *json_string, const char *key);
double calculate_heat_index(float temp_kelvin, float humidity);
double calculate_dew_point(float temp_kelvin, float humidity);
#endif //PROCESS_WEATHER_DATA_H
