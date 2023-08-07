#include "arrivals.h"

int main(int argc, char* argv[])
{
	bool isIcao24=false;
	int size = argc - 1; //argv[0] is address
	vector<Airport> database;
	vector<string> validInput=convertArgvToStringArr(isIcao24,argv, size);
	database=loadDatabase(validInput);
	printData(database);
}

void printData(vector<Airport>& database)
{
	int i,j=1;
	int size=database.size();
	string airportName;
	for (i=0;i<size;i++)
	{
		airportName=database[i].GetName();
		if (airportName!="")
			cout<<"The arriving flights for airport "<<airportName<<" are: "<<endl;
		for (auto& flight : database[i].GetArrivalsArr())
		{
			cout <<j<<". "<< "Flight #" << flight.GetCallSign() << "arriving from " << flight.GetOriginICOA() << ", took off at ";
			flight.printDateFromUnix(flight.GetDepartureTime());
			cout << " landed at ";
			flight.printDateFromUnix(flight.GetArrivalTime());
			cout<<endl;
			j++;
		}
		j=1;
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

