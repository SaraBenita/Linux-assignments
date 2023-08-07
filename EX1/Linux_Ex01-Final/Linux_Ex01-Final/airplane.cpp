#include "airplane.h"

int main(int argc, char* argv[])
{
	int size = argc - 1; //1st argument is address
	vector<Aircraft> database;
	bool isIcao24=true;
	vector<string> validInput=convertArgvToStringArr(isIcao24,argv, size);
	database=loadAircraftDatabase(validInput);
	printData(database);
}

void printData(vector<Aircraft>& database)
{
	int i=1;
	string aircraftName;
	for (auto& aircraft : database)
	{
		aircraftName=aircraft.GetName();
		if (aircraftName!="")
			cout<<"Flights found for aircraft "<<aircraftName<<" are:"<<endl;
		for (auto& flight : aircraft.GetFlightsArr())
	{
		cout<<i<<". "<<flight.GetIcao24()<<" departed from "<<flight.GetOriginICOA()<<" at ";
		flight.printDateFromUnix(flight.GetDepartureTime());
		cout<<" arrived in "<<flight.GetDestinationICOA()<<" at ";
		flight.printDateFromUnix(flight.GetArrivalTime());
		cout<<endl;
		i++;
	}
	i=1;
	cout<<endl;
	}
	
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