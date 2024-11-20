#!/bin/bash

# Script to execute main.c every hour to fetch weather data

# Path to compiled executable of C_EnviroTrack.c
MAIN_EXECUTABLE="./C_EnviroTrack.exe"

# Check if the main executable exists
if [ ! -f "$MAIN_EXECUTABLE" ]; then
    echo "Error: $MAIN_EXECUTABLE not found. Please compile main.c first."
    exit 1
fi

# Infinite loop to run the executable every hour
while true; do
    # Run the main executable
    $MAIN_EXECUTABLE

    # Wait for one hour before the next execution
    sleep 3600
done
