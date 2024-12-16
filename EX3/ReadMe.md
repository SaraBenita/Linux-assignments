# Flight Scanner Microservices

## Overview
This project extends the Flight Scanner application into a microservices architecture using Docker. The program consists of two containers:
1. **dbService**: Manages the database operations and executes child process tasks.
2. **flightService**: Handles user interactions, sends opcodes to `dbService`, and processes the responses.

The containers communicate via a **named pipe**, and the application supports enhanced functionality such as fetching airport data from the internet.

---

## Features
### Microservices Design
- **dbService**:
  - Runs the child process in a container.
  - Waits for opcodes from `flightService` via a named pipe.
- **flightService**:
  - Runs the main process in a container.
  - Handles user commands, sends opcodes to `dbService`, and prints the results.

### Fetch Airport Data
- The "Update DB" option was modified to fetch airport data from the internet for a given list of airport ICAOs.
- The Bash script was adapted to pass ICAO codes as arguments.

### Restartability
- Ensures that both containers can be restarted without breaking the application.

### Graceful Shutdown
- Properly terminates both containers upon shutdown.

---

## Technologies Used
1. **Docker**:
   - Containerized the application with two separate images: `dbService` and `flightService`.
   - Uploaded images to Docker Hub for easy deployment.
2. **Named Pipes**:
   - Used for inter-process communication between the containers.
3. **Bash Scripting**:
   - Automated the setup and initialization of the containers.
4. **C Programming**:
   - Modified the original program for containerized execution.
5. **Networking**:
   - Integrated airport data fetching via an online API.

---

## How to Run the Project
1. Clone the repository:
   git clone <repository-url>
   cd <repository-directory>
2. Build the Docker images (if not already available on Docker Hub):
   docker build -t dbservice ./dbService
   docker build -t flightservice ./flightService
3. Use the provided Bash script to initialize the setup:
  ./initialize_setup.sh
  **This script will:**
  -Pull the Docker images from Docker Hub (if not already present).
  -Run both dbService and flightService containers.
4. Interact with the application:
   Follow the menu options in the flightService container to perform various operations.
5. Shut down the application:
   Use the script or send a termination signal to ensure both containers are stopped gracefully.

  ### Notes
- Ensure Docker is installed and running on your system.
- The application is designed for Linux environments.
- Handle edge cases such as invalid inputs or API failures during data fetching.
