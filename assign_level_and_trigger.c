#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process_weather_data.h"
#include "assign_level_and_trigger.h"
//#include "logError.h"

// Function to assign danger level based on processed data
int assign_level(ProcessedData data,const char *output_file)
{
    int level ;
    if (data.heat_index > 43 || data.dew_point > 25 ) {
        level = 3; // High danger
    } else if (data.heat_index > 35 || data.dew_point > 20 ) {
        level = 2; // Moderate danger

    } else {
        level = 1; // Low danger
    }
    // Open the CSV file
    FILE *output = fopen(output_file, "a+");
    if (!output)
    {
        //logError("Error opening output file", NULL);
        return level;
    }
    if (level == 3) {
        fprintf(output, "   HIGH\n");
    } else if (level == 2) {
        fprintf(output, "   MODERATE\n");
    } else {
        fprintf(output, "   LOW\n");
    }
    return level;
}
// Function to trigger alerts if danger level exceeds threshold
void trigger_alert(int danger_level, const char *script, const char *date, const char *time) {
    if (danger_level == 3) { // High danger
        char command[256];
        snprintf(command, sizeof(command), "%s \"%d\" \"%s\" \"%s\"", script, danger_level, date, time);

        int result = system(command);
        if (result == -1) {
            //logError("Failed to execute alert script", NULL);
        }
    }
}
