#include "libUtility.h"
#include <zip.h>

namespace fs=std::filesystem;

#define NOT_FOUND -1
#define MAX_LEN_OF_ARG 7
#define FETCH_AIRPORTS_DATA 1
#define PRINT_ARRIVAL_FLIGHTS 2
#define PRINT_FULL_SCHEDULE 3
#define PRINT_AIRCRAFTS_FLIGHTS 4
#define ZIP_DB 5
#define EXIT 6

void extractDB();
int getPipeToRead(const char* readPath);
int getPipeToWrite(const char* writePath);
vector<string> readFromPipe(int pipeToRead, int *outChoice);
string readStringFromPipe(int pipefd);
void getLocalData(vector<Airport>& airportsOfDB, vector<Aircraft>& aircraftsOfDB);

void ExecuteChoice(int choice, vector<string> &inputNames, vector<Airport>& airportsDB, vector<Aircraft>& aircraftsDB);
vector<string> findAircraftsNames();
vector<string> findAirportNames();
void getAirplaneDataByIcao24(vector<string> &icao24Args, vector<Aircraft> &aircraftsDB);
void printAirPlaneData(map<int,Flight> &database, string aircraftName);
void getFullFlightsSCheduleFunctionByAirport(vector<string> &airportsArgs, vector<Airport> &airportsDB);
map<int, Flight> sortAirportFlights(const Airport &database);
map<int, Flight> sortAircraftFlights(const Aircraft &database);
void printFullScheduleFlights(map<int, Flight> &flightsMap, string argName);
void zip_folder(zip_t *archive, const std::string &folderPath, const std::string &parentFolder);
void zipDB();
bool compareByDepartureTime(const Flight &flight1, const Flight &flight2);
void getArrivalsDataByAirport(vector<string> &airportsArgs, vector<Airport> &airportsDB);
int isAirportInDB(vector<Airport> &airportsDB, string airportName);
int isAicraftInDB(vector<Aircraft> &aircraftDB, string aircraftName);
void printArrivalsData(vector<Airport> &database);
void removeDir(string folderName);
void printDateFromUnix(int unixNumber);
void CreateNamedPipe(const char* filePath);