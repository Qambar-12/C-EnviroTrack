# C-EnviroTrack
A C-based project for real-time environmental data monitoring. Features include API integration, file-based data storage, shell scripts for task automation, dynamic memory allocation for efficient data handling, and real-time alerts using Linux system calls. Modular design with header files ensures readability and maintainability.
Key Features:
Data Retrieval: The system connects to a free API to collect real-time data, such as temperature and humidity.

Data Processing & Analysis: Using C programming, the system processes the data and analyzes it for any unusual patterns or trends.

Data Storage: It saves both the raw and processed data into files.

Automation & Integration: Shell scripts automate tasks like data retrieval, processing, and report generation, all integrated with the C program.

Optimization & Efficiency: Pointers and dynamic memory allocation are used to make data handling more efficient.

Real-time Alerts: The system sends real-time alerts using Linux system commands to notify users about critical readings.

Code Organization & Documentation: Detailed documentation is provided, and the code is organized into modular files with header files to improve clarity and maintenance.

System Requirements:

A C compiler (e.g., GCC)

Linux OS (Ubuntu recommended)

Windows Subsystem for Linux (WSL) version 2, for Windows users to run Linux environments

CURL: Required to retrieve real-time data from the API.

cJSON: Required for parsing JSON data from the API.

cron: Needed to schedule automated tasks for data retrieval and report generation.

Once these tools are cloned and installed, the system will operate as intended.

Contributors:

Muhammad Qambar Hussain(CS-23125)
Asadullah Nizami(CS-23092)
AliRaza Baloch(CS-23130)
