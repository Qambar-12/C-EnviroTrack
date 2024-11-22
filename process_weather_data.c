#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "process_weather_data.h"
#include "cJSON/cJSON.h"
#include "logError.h"

#define MAX_BUFFER_SIZE 2048
#define MAX_CSV_LINES 30

// Function to extract a numeric value from the JSON string for a specific key
double extract_value(const char *json_string, const char *key) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        return -1; // JSON parsing failed
    }

    cJSON *main = cJSON_GetObjectItemCaseSensitive(json, "main");
    if (main == NULL) {
        cJSON_Delete(json);
        return -1; // "main" object not found
    }

    cJSON *value = cJSON_GetObjectItemCaseSensitive(main, key);
    if (value == NULL || !cJSON_IsNumber(value)) {
        cJSON_Delete(json);
        return -1; // Key not found or not a number
    }

    double result = (float)value->valuedouble;
    cJSON_Delete(json);
    return result;
}

// Function to calculate heat index using Kelvin temperature
double calculate_heat_index(float temp_kelvin, float humidity) {
    // Convert temperature from Kelvin to Celsius
    double t = temp_kelvin - 273.15;
    double h = humidity;

    // Heat index formula with temperature in Celsius
    return -8.78469475556 +
           (1.61139411 * t) +
           (2.33854883889 * h) -
           (0.14611605 * t * h) -
           (0.012308094 * pow(t, 2)) -
           (0.0164248277778 * pow(h, 2)) +
           (0.002211732 * pow(t, 2) * h) +
           (0.00072546 * t * pow(h, 2)) -
           (0.000003582 * pow(t, 2) * pow(h, 2));
}

// Function to calculate dew point
double calculate_dew_point(float temp_kelvin, float humidity) {
    // Convert temperature from Kelvin to Celsius
    float temp_celsius = temp_kelvin - 273.15;
    float a = 17.27, b = 237.7;
    float alpha = ((a * temp_celsius) / (b + temp_celsius)) + log(humidity / 100.0);
    return (b * alpha) / (a - alpha); // Dew point in Celsius
}

// Function to write headers if they do not exist
void write_csv_headers(FILE *output) {
    // Move to the start of the file
    fseek(output, 0, SEEK_END);
    if (ftell(output) == 0) {
        fprintf(output, "TEMP       HUMIDITY        PRESSURE        HEAT_INDEX        DEW_POINT       DANGER\n");
    }
    fseek(output, 0, SEEK_END);
}

// Process data returned from an HTTP request
ProcessedData process_weather_data(const char *raw_response, const char *output_file) {
    ProcessedData data = {0};

    if (!raw_response) {
        logError("HTTP request failed or returned NULL", NULL);
        return data;
    }

    // Extract required values
    double temp_kelvin = extract_value(raw_response, "temp");
    double temp_celcius = temp_kelvin - 273.15;
    double humidity = extract_value(raw_response, "humidity");
    double pressure = extract_value(raw_response, "pressure");

    // Perform calculations
    data.heat_index = calculate_heat_index(temp_kelvin, humidity);
    data.dew_point = calculate_dew_point(temp_kelvin, humidity);

    // Open the CSV file
    FILE *output = fopen(output_file, "a+");
    if (!output) {
        logError("Error opening output file", NULL);
        return data;
    }

    // Write headers if they do not exist
    if (ftell(output) == 0) {
        write_csv_headers(output);
    }

    // Count lines in CSV
    int line_count = 0;
    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), output)) {
        line_count++;
    }

    // Rotate file if line count exceeds MAX_CSV_LINES
    if (line_count >= (MAX_CSV_LINES + 1)) {
        FILE *temp_file = fopen("temp.csv", "w");
        if (!temp_file) {
            logError("Error opening temporary file", NULL);
            fclose(output);
            return data;
        }

        fseek(output, 0, SEEK_SET);
        fgets(buffer, sizeof(buffer), output); // Skip the header
        fgets(buffer, sizeof(buffer), output); // Skip the first line (oldest data)
        while (fgets(buffer, sizeof(buffer), output)) {
            fprintf(temp_file, "%s", buffer);
        }
        fclose(temp_file);
        fclose(output);

        remove(output_file);
        rename("temp.csv", output_file);
        output = fopen(output_file, "a+");
        write_csv_headers(output); // Re-add headers
    } else {
        fclose(output);
        output = fopen(output_file, "a+");
    }

    // Append processed data
    fprintf(output, "%.2f      %.2f           %.2f          %.2f            %.2f       ",
            temp_kelvin, humidity, pressure, data.heat_index, data.dew_point);

    fclose(output);
    return data;
}