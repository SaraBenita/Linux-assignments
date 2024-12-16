# Flight Service

## Overview
This project implements a multi-process flight service program, designed to manage user interactions and database operations. The program is divided into a **main process** and a **child process** for efficient handling of operations like retrieving flight details, updating the database, and zipping files.

---

## Features
### Supported Options
1. **Retrieve Arrival Flights**:
   - Outputs all arrival flights and their details for a given list of airport ICAO codes.
2. **Full Airport Schedule**:
   - Displays a sorted schedule of departures and arrivals for a specific airport.
3. **Flights by Aircraft**:
   - Retrieves all flights (departures and arrivals) for specific aircraft using their ICAO24 codes.
4. **Update Database**:
   - Re-runs the Bash script from the first assignment to update flight data.
5. **Zip Database**:
   - Compresses the database files using the **libzip** library.
6. **Return Child Process PID**:
   - Displays the PID of the child process.
7. **Graceful Exit**:
   - Ensures proper zipping of database files and terminates both processes upon receiving `SIGUSR1` or `SIGINT`.

---

## Architecture
### Main Process
- Creates the child process on initialization.
- Handles user input and displays results.
- Sends operation codes to the child process for execution.

### Child Process
- Waits for opcodes from the main process.
- Executes the requested operations (e.g., retrieving flight data, zipping files).
- Manages data in memory for fast access.

---

## Technologies Used
1. **C Programming**:
   - Multi-process management using `fork`.
   - Communication through pipes.
   - Signal handling for graceful exit.
2. **libzip**:
   - Handles zipping and unzipping of database files.
3. **CMake**:
   - Used to build and manage the project.
4. **Linux Process Management**:
   - Handles process creation and communication efficiently.

---

## How to Run the Program
1. **Clone the Repository**:
   git clone <repository-url>
   cd <repository-directory>
2. **Build the Project**:
   Use CMake to compile the program: make
3. **Run the Program**:
Execute the program to start the main process: ./flight_service
4. **Interact with the Program**:
- Use the menu options to perform operations:
    -Retrieve flight data for airports.
    -Retrieve schedules or flights by aircraft.
    -Update the database or zip the data files.
5. **Graceful Exit**:
-To terminate the program, send SIGUSR1 or SIGINT (Ctrl+C):
  -The child process will zip the database and exit.

### Notes:
- Ensure that libzip is installed on your system.
- Use a Linux environment for compatibility with process and signal management.
- Handle invalid input gracefully to avoid runtime errors.
- Manage large datasets efficiently using in-memory structures.

A readme file is attached to the directory, providing detailed instructions on how to run the assignment.








