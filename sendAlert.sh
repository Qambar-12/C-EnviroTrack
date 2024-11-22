#!/bin/bash

DANGER_LEVEL=3
LEVEL_DESCRIPTION="HIGH"
DATE=$(date +"%d-%m-%Y")
TIME=$(date +"%H:%M")
echo "Anomaly detected in weather data.\nYour System needs attention"
echo "Date : $DATE"
echo "Time : $TIME"
echo "Danger Level : $DANGER_LEVEL ($LEVEL_DESCRIPTION)"



## Email configuration
#TO="<Email>@gmail.com"
#SUBJECT="***Anomaly in weather***"
#BODY="An anomaly is detected in the weather data at:\n\nDate: $DATE\nTime: $TIME\n\n        ***ANOMALY***        \n\nDanger Level: $DANGER_LEVEL\n\nPlease find the attached report of weather data"
#ATTACHMENT_FILE="<Add path of process.csv>"
#
## Path to the password file
#PASSWORD_FILE="password.txt"
#
## Check if the password file exists
#if [ ! -f "$PASSWORD_FILE" ]; then
#    echo "Error: Password file not found. Please create 'password.txt' containing the SMTP password."
#    exit 1
#fi
#
## Read the SMTP password from the file
#SMTP_PASSWORD=$(cat "$PASSWORD_FILE")
#
## Send the email using sendemail
#sendemail -f "<Email>@gmail.com" \
#          -t "$TO" \
#          -u "$SUBJECT" \
#          -m "$BODY" \
#          -a "$ATTACHMENT_FILE" \
#          -s "smtp.gmail.com:587" \
#          -o tls=yes \
#          -xu "<Email>@gmail.com" \
#          -xp "$SMTP_PASSWORD"