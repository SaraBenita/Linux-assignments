#include "libUtility.h"
#include <algorithm>

map<int, Flight> sortFlights(const Airport& database);
bool compareByDepartureTime(const Flight& flight1, const Flight& flight2);
void printFlights(map<int,Flight>& flightsMap, string airportName);
vector<string> convertArgvToStringArr(bool isIcao24, char *argv[], int size);
