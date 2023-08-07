#include "libUtility.h"
#include <zip.h>
#include <filesystem>
#include <sys/stat.h>

namespace fs = std::filesystem;

#define WRITE 1
#define READ 0

#define NOT_FOUND -1
#define MAX_LEN_OF_ARG 7

#define ARRIVAL_FLIGHTS 1
#define FULL_SCHEDULE 2
#define AIRCRAFTS_FLIGHTS 3
#define UPDATE 4
#define ZIP_DB 5
#define GET_CHILD_PID 6
#define EXIT 7

void PrintMenu();
void MakePipes(int *pipes);
pid_t SplitUsingFork();
int GetChoice();
vector<string> GetData(int choice);
void PrintError();
vector<string> GetNames(bool isAirport);
void WriteToPipe(int pipeToWrite, int choice, vector<string> &inputNames);
vector<string> ReadFromPipe(int pipeToRead, int *outChoice);
bool isValidChoice(const string &str);
void ExecuteChoice(int choiceSignal, vector<string> &inputNames, vector<Airport> airportsDB, vector<Aircraft> aircraftsDB);
void ZipData();
void handleSIGINT(int signal);
void printFullScheduleFlights(map<int, Flight> &flightsMap, string argName);
void getAirplaneDataByIcao24(vector<string> &icao24Args, vector<Aircraft> &aircraftsDB);
void printAirPlaneData(map<int,Flight> &database, string aircraftName);
map<int, Flight> sortAirportFlights(const Airport &database);
map<int, Flight> sortAircraftFlights(const Aircraft &database);
void getFullFlightsSCheduleFunctionByAirport(vector<string> &airportsArgs, vector<Airport> &airportsDB);
bool compareByDepartureTime(const Flight &flight1, const Flight &flight2);
void getArrivalsDataByAirport(vector<string> &airportsArgs, vector<Airport> &airportsDB);
void printArrivalsData(vector<Airport> &database);
void updateDatabase();
vector<string> findAirportNames();
vector<string> findAircraftsNames();
void removeDir(string folderName);
int createSignalByUserChoice(int userChoice);
void makeFatherPart(int *fatherToSonPipe, int *sonToFatherPipe, pid_t childPid);
void makeChildPart(int *fatherToSonPipe);
void informChildOnGracefulExit(pid_t childPid);
void handleSIGUSR1(int signal);
string readStringFromPipe(int pipefd);
void zipDB();
int isAirportInDB(vector<Airport> &airportsDB, string airportName);
int isAicraftInDB(vector<Aircraft> &aircraftDB, string aircraftName);
void zip_folder(zip_t *archive, const std::string &folderPath, const std::string &parentFolder);
void extractDB();
void printDateFromUnix(int unixNumber);