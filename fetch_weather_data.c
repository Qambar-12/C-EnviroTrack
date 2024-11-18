#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "fetch_weather_data.h"
//#include "logFile.h"

#define MAX_LINES 30
#define MAX_BUFFER_SIZE 1024

// Callback function for writing received data
size_t dataWriteHandler(void *contents, size_t size, size_t nmemb, char **output) {
    size_t realsize = size * nmemb;

    // Allocate memory for the output buffer
    char *temp = realloc(*output, realsize + 1);
    if (temp == NULL) {
        //logError("Memory allocation error in dataWriteHandler", NULL);
        fprintf(stderr, "Memory allocation error\n");
        return 0; // Indicate failure
    }
    *output = temp;

    // Copy the received data into the output buffer
    memcpy(*output, contents, realsize);
    (*output)[realsize] = '\0'; // Null-terminate the string

    return realsize;
}

// Function to read the API key from the .env file
char* get_api_key(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        perror("Error opening .env file");
        return NULL;
    }

    char line[256];
    char* api_key = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "API_KEY=%ms", &api_key) == 1) {
            break; // API key successfully extracted
        }
    }

    fclose(file);
    return api_key;
}

// Function to perform an HTTP request and save the response to a file
char *executeHttpRequest(const char *output_filename) {
    CURL *curl;
    CURLcode result;
    FILE *file;
    char *response = NULL;

    // Initialize libcurl
    curl = curl_easy_init();
    if (curl == NULL) {
        //logError("Failed to initialize libcurl", NULL);
        fprintf(stderr, "Failed to initialize libcurl\n");
        return NULL;
    }
    const char* env_file = ".env";
    char* api_key = get_api_key(env_file);
    if (api_key == NULL) {
        //logError("API key not found in .env file", NULL);
        fprintf(stderr, "API key not found in .env file\n");
        curl_easy_cleanup(curl);
        return NULL;
    }
    char url[256];
    sprintf(url, "https://api.openweathermap.org/data/2.5/weather?q=Karachi&appid=%s", api_key);
    // Set the request URL for the weather data
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // Disable SSL verification (for testing only)
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    // Open a file for writing the response
    file = fopen(output_filename, "a+");
    if (file == NULL) {
        //logError("Failed to open file for writing", NULL);
        fprintf(stderr, "Failed to open file for writing\n");
        curl_easy_cleanup(curl);
        return NULL;
    }

    // Set the write callback function to write to the response buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dataWriteHandler);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
    }
    free(api_key);

    // Check and limit the number of lines in the file
    int lineCount = 0;
    char buffer[MAX_BUFFER_SIZE];

    // Count the number of lines in the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineCount++;
    }

    // If the line count exceeds or equals the maximum allowed lines, trim the file
    if (lineCount >= MAX_LINES) {
        // Rewind to the beginning of the file
        fseek(file, 0, SEEK_SET);

        FILE *tempFile = fopen("temp.txt", "w");
        if (tempFile == NULL) {
            perror("Error opening temporary file");
            fclose(file);
            return;
        }

        // Skip the first line in the original file
        fgets(buffer, sizeof(buffer), file); // Read and discard the first line

        // Copy the remaining lines to the temporary file
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            fprintf(tempFile, "%s", buffer);
        }

        fclose(tempFile);
        fclose(file);

        // Replace the original file with the trimmed file
        remove(output_filename);
        rename("temp.txt", output_filename);

        // Reopen the trimmed file for appending
        file = fopen(output_filename, "a");
        if (!file) {
            perror("Error reopening file");
            return;
        }
    } else {
        // Close and reopen the file in append mode if trimming is not required
        fclose(file);
        file = fopen(output_filename, "a");
        if (!file) {
            perror("Error reopening file for appending");
            return;
        }
    }

    // Append the new response to the file
    fprintf(file, "%s\n", response);

    // Clean up and free resources
    fclose(file);
    curl_easy_cleanup(curl);

    return response;
}