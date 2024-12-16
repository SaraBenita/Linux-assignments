# Flight Scanner Assignments

## Overview
This repository contains three assignments that progressively extend the functionality of a **Flight Scanner** application. Each assignment builds upon the previous one, incorporating new technologies, architectural patterns, and advanced features. Together, these assignments demonstrate the evolution of a robust flight data management system, transitioning from a simple standalone application to a microservices-based solution using Docker.

---

## Assignments Breakdown

### 1. **Assignment 1: Flight Data Retrieval and Processing**
- **Description**:
  - Implemented a Bash script to fetch flight data (arrivals and departures) for a list of airports from the OpenSky API.
  - Developed C programs to process and analyze the retrieved data, including retrieving flights by airport or aircraft and generating schedules.
- **Key Technologies**:
  - Bash scripting for data retrieval and JSON-to-CSV conversion.
  - C++ programming for efficient data processing.
  - Makefile for build automation.

### 2. **Assignment 2: Multi-Process Flight Scanner**
- **Description**:
  - Extended the Flight Scanner into a multi-process application using **fork** and **pipes**.
  - Separated the program into a **main process** (handling user input) and a **child process** (managing the database).
  - Added functionality to zip/unzip the database using `libzip`.
- **Key Technologies**:
  - Multi-process programming in C with inter-process communication (IPC) using pipes.
  - Signal handling for graceful termination (`SIGUSR1`, `SIGINT`).
  - `libzip` for file compression.

### 3. **Assignment 3: Flight Scanner Microservices**
- **Description**:
  - Converted the Flight Scanner into a microservices architecture using Docker.
  - Containerized the main and child processes into two separate Docker containers (`flightService` and `dbService`).
  - Enabled container communication through a named pipe and added support for dynamic airport data fetching via an API.
- **Key Technologies**:
  - Docker for containerization.
  - Named pipes for inter-container communication.
  - Enhanced Bash scripting for setup and deployment.

---

## How to Use This Repository
1. Navigate to the folder for each assignment:
   - **Assignment 1**: `assignment1/`
   - **Assignment 2**: `assignment2/`
   - **Assignment 3**: `assignment3/`
2. Each folder contains a detailed `README.md` with specific instructions on building and running the project.
3. Follow the steps provided in each `README.md` to set up and execute the corresponding application.

---

## Key Highlights
- Evolution from a single-process script to a fully containerized microservices application.
- Use of advanced C programming techniques (multi-process management, IPC, shared libraries).
- Integration of external tools and technologies like Docker, Bash scripting, and libzip.
- Real-world applicability through interaction with the OpenSky API for flight data retrieval.

---

## Notes
- Ensure all required dependencies (e.g., Docker, GCC, libzip) are installed before running the assignments.
- Each assignment builds on the previous one, demonstrating incremental enhancements in functionality and architecture.
