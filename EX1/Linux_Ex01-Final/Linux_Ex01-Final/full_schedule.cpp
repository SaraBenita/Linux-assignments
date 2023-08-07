#include "full_schedule.h"

int main(int argc, char* argv[])
{
	bool isIcao24=false;
	int size;
	if (argc>2) //only care about the 1st argument
		size=1;
	else
		size=argc-1;
	map<int, Flight> flightsMap;
	vector<Airport> database;
	vector<string> validInput=convertArgvToStringArr(isIcao24,argv, size);
	if (validInput.size()>0)
	{
	database=loadDatabase(validInput);
	flightsMap=sortFlights(database[0]);
	printFlights(flightsMap, validInput[0]);
	}
	
	
}

map<int, Flight> sortFlights(const Airport& database)
{
	map<int, Flight> flightsMap;

		for (auto flight : database.GetArrivalsArr())
		{
			flightsMap.insert({flight.GetDepartureTime(),flight});
		}
		for (auto flight : database.GetDeparturesArr())
		{
			flightsMap.insert({flight.GetArrivalTime(),flight});
		}

	return flightsMap;

}


void printFlights(map<int,Flight>& flightsMap, string airportName)
{
	string departString = "departing to ", arrivalString = "arriving from ";
	int i=1;
	 for (auto it = flightsMap.begin(); it != flightsMap.end(); ++it) 
	 {
        
	 	cout <<i<<". "<< "Flight #" << it->second.GetCallSign() << " ";
		if (it->second.GetOriginICOA() == airportName)
		{
			cout << departString << " " << it->second.GetDestinationICOA() << " at "; //<<flight.GetArrivalTime();
			it->second.printDateFromUnix(it->second.GetArrivalTime());
		}
		else if (it->second.GetDestinationICOA() == airportName)
		{
			cout << arrivalString << " " << it->second.GetOriginICOA() << " at "; //<<flight.GetDepartureTime();
			it->second.printDateFromUnix(it->second.GetDepartureTime());
		}
		i++;
		cout << endl;
     }
}


bool compareByDepartureTime(const Flight& flight1, const Flight& flight2)
{
	return flight1.GetDepartureTime() < flight2.GetDepartureTime();
}

vector<string> convertArgvToStringArr(bool isIcao24, char *argv[], int size)
{
	vector<string> arr;
	bool isValidInput = false;
	string currentArg;
	for (int i = 1; i <= size; i++)
	{
		currentArg = argv[i];
		// i-1 because argv starts from 1 (0 is address)
		if (isIcao24 == true)
		{
			isValidInput = isValidIcao24(currentArg);
			if (isValidInput)
				arr.push_back(currentArg);
		}
		else
		{
			isValidInput = isValidAirport(currentArg);
			if (isValidInput)
				arr.push_back(currentArg);
		}

		if (!isValidInput)
			cout << currentArg << " is not a valid input!" << endl;
	}
	return arr;
}
