#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#define NUM_OF_FIELDS 6

#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <map>
#include <dirent.h>
#include <regex>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

class Flight
{
private:
	string icao24;
	int arrivalTime;
	string originICOA;
	int departureTime;
	string destinationICOA;
	string callSign;

public:
	Flight() = default;
	Flight(string new_icao24, int new_departureTime, string new_originICOA, int new_arrivalTime, string new_destinationICOA,
		   string new_callSign) : icao24(new_icao24), departureTime(new_departureTime), originICOA(new_originICOA), arrivalTime(new_arrivalTime),
								  destinationICOA(new_destinationICOA), callSign(new_callSign)
	{
	}

	const string &GetIcao24() const
	{
		return icao24;
	}

	const int &GetArrivalTime() const
	{
		return arrivalTime;
	}

	const string &GetOriginICOA() const
	{
		return originICOA;
	}

	const int &GetDepartureTime() const
	{
		return departureTime;
	}

	const string &GetDestinationICOA() const
	{
		return destinationICOA;
	}

	const string &GetCallSign() const
	{
		return callSign;
	}

	void PrintFlight()
	{
		cout << "ICAO24: " << icao24 << endl;
		cout << "Arrival Time: " << arrivalTime << endl;
		cout << "Origin ICOA: " << originICOA << endl;
		cout << "Departure Time: " << departureTime << endl;
		cout << "Destination ICOA: " << destinationICOA << endl;
		cout << "Call Sign: " << callSign << endl;
	}
};

class Aircraft
{
private:
	string name;
	vector<Flight> flightsArr;

public:
	Aircraft()
	{
		name = "";
		flightsArr.resize(0);
	}
	Aircraft(string new_name, vector<Flight> new_flightsArr)
	{
		name = new_name;
		flightsArr = new_flightsArr;
	}
	const string &GetName() const
	{
		return name;
	}
	const vector<Flight> &GetFlightsArr() const
	{
		return flightsArr;
	}
};

class Airport
{
private:
	string name;
	vector<Flight> arrivalsArr;
	vector<Flight> departuresArr;

public:
	Airport(string new_name, vector<Flight> new_arrivalsArr, vector<Flight> new_departuresArr)
	{
		name = new_name;
		arrivalsArr = new_arrivalsArr;
		departuresArr = new_departuresArr;
	}
	Airport()
	{
		name = "";
		arrivalsArr.resize(0);
		departuresArr.resize(0);
	}
	const string &GetName() const
	{
		return name;
	}

	const vector<Flight> &GetArrivalsArr() const
	{
		return arrivalsArr;
	}

	const vector<Flight> &GetDeparturesArr() const
	{
		return departuresArr;
	}

	void PrintFlights()
	{
		cout << "Arrivals: " << endl;
		for (Flight f : arrivalsArr)
		{
			f.PrintFlight();
			cout << endl;
		}

		cout << "Departures: " << endl;
		for (Flight f : departuresArr)
		{
			f.PrintFlight();
			cout << endl;
		}
	}
};

bool isValidAirport(string &str);
bool isValidIcao24(string &str);
bool isNumbersAndAlphaWord(const string &str);
bool isAlphabeticWord(const string &str);
bool isUpperCaseWord(const string &str);
bool IsScriptSuccess(string folderName);
string toUpperWord(string str);
vector<Airport> loadDatabase(vector<string> airportNames);
vector<Aircraft> loadAircraftDatabase(vector<string> airportNames);
Airport getAirportData(string airportName);
Aircraft getAircraftData(string aircraftName);
Airport readFlightsData(string airportStr);
Aircraft readFlightsByAirCraft(string aircraftStr);
vector<Flight> readSingleFile(ifstream &file);
Flight createFlight(string str);
void checkFile(ifstream &file);
void PrintError();