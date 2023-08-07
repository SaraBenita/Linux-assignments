#include "libUtility.h"

bool isValidAirport(string &str)
{
	if (str.length() == 4 && isAlphabeticWord(str))
	{
		str = toUpperWord(str);
		return true;
	}
    errno=EINVAL;
	return false;
}

bool isValidIcao24(string &str)
{
	if (str.length() == 6 && isNumbersAndAlphaWord(str))
	{
		str = toUpperWord(str);
		return true;
	}
    errno=EINVAL;
	return false;
}

bool isNumbersAndAlphaWord(const string &str)
{
	int i;

	for (i = 0; i < str.length(); i++)
	{
		if (!isalpha(str[i]) && !isdigit(str[i]))
			return false;
	}
	return true;
}

bool isAlphabeticWord(const string &str)
{
	int i;

	for (i = 0; i < str.length(); i++)
	{
		if (!isalpha(str[i]))
			return false;
	}
	return true;
}

bool isUpperCaseWord(const string &str)
{
	int i;

	for (i = 0; i < str.length(); i++)
	{
		if (!isupper(str[i]))
			return false;
	}
	return true;
}

string toUpperWord(string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]))
			str[i] = toupper(str[i]);
	}
	return str;
}

vector<Airport> loadDatabase(vector<string> airportNames)
{
	int i;
	int size = airportNames.size();
	vector<Airport> airportsArr;
	airportsArr.resize(size);
	for (i = 0; i < size; i++)
	{
		airportsArr[i] = getAirportData(airportNames[i]);
	}
	return airportsArr;
}

vector<Aircraft> loadAircraftDatabase(vector<string> airportNames)
{
	int i;
	int size = airportNames.size();
	vector<Aircraft> aircraftsArr;
	aircraftsArr.resize(size);
	for (i = 0; i < size; i++)
	{
		string check = airportNames[i];
		aircraftsArr[i] = getAircraftData(airportNames[i]);
	}
	return aircraftsArr;
}

// the wanted "get_flights_by_airport_name" function
Airport getAirportData(string airportName)
{
	string buildScriptCall;
	int i;
	char buffer[256];

	buildScriptCall = "./Script.sh " + airportName;
	const char *runScript = buildScriptCall.c_str();
	system(runScript);

	if (IsScriptSuccess(airportName))
		return readFlightsData(airportName);
	else
	return Airport();
	
}

Aircraft getAircraftData(string aircraftName)
{
	string buildScriptCall;
	int i;

	buildScriptCall = "./Script.sh " + aircraftName;
	const char *runScript = buildScriptCall.c_str();
	system(runScript);
	if (IsScriptSuccess(aircraftName))
		return readFlightsByAirCraft(aircraftName);
	else
	return Aircraft();
}

bool IsScriptSuccess(string folderName) 
{
    namespace fs = filesystem;
    fs::path folderPath("Api Data/"+folderName);
	if ( fs::exists(folderPath) && fs::is_directory(folderPath) )
		return true;
    return false;
}


Airport readFlightsData(string airportStr)
{
	string arrivals = "Api Data/" + airportStr + "/" + airportStr + ".arv";
	string departures = "Api Data/"+ airportStr + "/" + airportStr + ".dpt";

	vector<Flight> arrivalsArr;
	vector<Flight> departuresArr;

	ifstream arrivalsFile(arrivals);
	ifstream departuresFile(departures);

	checkFile(arrivalsFile);
	checkFile(departuresFile);

	arrivalsArr = readSingleFile(arrivalsFile);
	departuresArr = readSingleFile(departuresFile);

	arrivalsFile.close();
	departuresFile.close();

	Airport airport(airportStr,arrivalsArr, departuresArr);
	return airport;
}

Aircraft readFlightsByAirCraft(string aircraftStr)
{

	string flightsFileName = "Api Data/" + aircraftStr + "/" + aircraftStr + ".flt";

	vector<Flight> flightsArr;

	ifstream flightsFile(flightsFileName);

	checkFile(flightsFile);

	flightsArr = readSingleFile(flightsFile);

	flightsFile.close();

	Aircraft aircraft(aircraftStr,flightsArr);
	return aircraft;
}

vector<Flight> readSingleFile(ifstream &file)
{
	string currentLine;
	vector<Flight> flightsArr;

	getline(file, currentLine);
	while (getline(file, currentLine))
	{
		flightsArr.push_back(createFlight(currentLine));
	}

	return flightsArr;
}

Flight createFlight(string str)
{
	const char *constLine = str.c_str();
	char *&line = const_cast<char *&>(constLine);
	int counter = 0;
	char *arr[NUM_OF_FIELDS];

	char *token = std::strtok(line, ",");

	while (token != nullptr && counter <= 5)
	{
		arr[counter] = token;
		token = std::strtok(nullptr, ",");
		counter++;
	}
	Flight flight(arr[0], atoi(arr[1]), arr[2], atoi(arr[3]), arr[4], arr[5]);

	return flight;
}

void checkFile(ifstream &file)
{
	if (!file.is_open())
		cout << "File failed to open!" << endl;
}

void PrintError()
{
    cout << "Error: " << strerror(errno) << " (error number: " << errno << ")" << endl;
}