# Flight Scanner

## Overview
This project implements a flight scanner that fetches, processes, and analyzes flight data from the OpenSky API. The project is divided into three parts:
1. **Data Retrieval**: Using a Bash script to fetch flight data for given airports.
2. **Data Processing**: Using C programs to analyze the flight data.
3. **Build Automation**: Using a Makefile to compile and manage the programs.

---

## Assignment Breakdown

### 1. Bash Script (`flightScanner.sh`)
The Bash script retrieves flight data for a list of airports provided as input parameters. The data includes arrivals and departures from the last 24 hours and is saved in a structured format.

#### Features
- Fetches flight data in JSON format and converts it to CSV.
- For each airport:
  - Creates a directory named after the airport (e.g., `LLBG/`).
  - Saves two files:
    - `<airport_name>.dpt`: Departure flight details.
    - `<airport_name>.arv`: Arrival flight details.
- Handles errors such as invalid airport codes or API failures.
- **Constraints**:
  - Does not use the `jq` utility for JSON parsing.
  - Includes only relevant columns for further processing.

#### Example Usage
./flightScanner.sh LLBG EGGW EGLL EGKK
# This will:
Create directories LLBG/, EGGW/, etc.
Save flight data in the respective .dpt and .arv files for each airport.

## 2. C++ Programs
Four C++ programs process the flight data fetched by the Bash script. All programs use a shared library (libflights.so) for core functionalities.

## Shared Library (libflights.so)
The library contains reusable functions such as:

load_db: Load flight data from files.
get_flights_by_airport_name: Retrieve flights by airport.
get_flights_by_callsign: Retrieve flights by aircraft callsign.
rerun_script: Re-execute the Bash script to update the database.

# Standalone Programs
1. Flights by Airports:
Lists all flights departing from specified airports along with their details.
### Usage:
./flights_by_airport LLBG

2. Full Airport Schedule:
Displays a sorted schedule of arrivals and departures for a specific airport.
### Usage:
./full_schedule LLBG

3. Flights by Aircraft:
Retrieves all flights (departures and arrivals) for specific aircraft by their ICAO24 codes.
### Usage:
./flights_by_aircraft ABC123 DEF456

4. Update Database:
Re-runs the Bash script to update the database with the latest flight data.
### Usage:
./update_db

## 3. Makefile
The Makefile automates the compilation process for the C programs and the shared library.

#Features
Compiles the shared library (libflights.so) and the four C programs into executables.
Provides a make clean target to delete compilation outputs.

## Technologies Used
## 1. Bash:
For fetching flight data and converting JSON to CSV.
Handling HTTP requests to the OpenSky API.
## 2.C++ Programming:
For processing and analyzing flight data efficiently.
Shared library design for modularity.
## 3. Makefile:
Automates the build process and ensures consistency.
## 4. Networking:
OpenSky API integration for flight data retrieval.

A readme file is attached in the directory, providing detailed instructions on how to run the assignment.
