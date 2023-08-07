#include <dbService.h>

int main()
{
    int choice, pipeToRead;
    fs::path cwd = fs::current_path();
    fs::path writePathFS=cwd/"sharedFolder"/"output";
    const char* writePath = writePathFS.c_str();
     fs::path readPathFS=cwd/"sharedFolder"/"userInput";
    const char* readPath = readPathFS.c_str();
    vector<Airport> airportsOfDB;
    vector<Aircraft> aircraftsOfDB;
    vector<string> inputNames;
   

    while(true)
    {
        remove(writePath);
        remove(readPath);
        extractDB();
        CreateNamedPipe(writePath);
        CreateNamedPipe(readPath);
        pipeToRead=getPipeToRead(readPath);
        inputNames=readFromPipe(pipeToRead,&choice);
        getLocalData(airportsOfDB,aircraftsOfDB);
        ExecuteChoice(choice,inputNames,airportsOfDB,aircraftsOfDB);
        if (choice!=ZIP_DB) //I want it to zip at any iteration to maintain data updated,
                            //but not do it twice when the user asks
        {
            zipDB();
        }
    }
        
}

void extractDB()
{
    const char *zipFileName = "Api Data.zip";
    const char *destinationFolder = "Api Data";

    // Check if the zip file exists
    struct stat zipFileStat;
    if (stat(zipFileName, &zipFileStat) != 0)
    {
        return;
    }

    // Remove the existing destination folder if it exists
    struct stat destinationFolderStat;
    if (stat(destinationFolder, &destinationFolderStat) == 0 && S_ISDIR(destinationFolderStat.st_mode))
    {
        std::string removeCommand = "rm -rf '";
        removeCommand = removeCommand + destinationFolder + "'";
        if (system(removeCommand.c_str()) != 0)
        {
            std::cout << "Failed to remove the existing destination folder." << std::endl;
            return;
        }
    }

    // Extract the zip file to the destination folder
    std::string extractCommand = "unzip -q -o ";
    extractCommand = extractCommand + "'" + zipFileName + "'";
    extractCommand += " -d '";
    extractCommand = extractCommand + destinationFolder + "'";
    if (system(extractCommand.c_str()) != 0)
    {
        std::cout << "Failed to extract the zip file." << std::endl;
        return;
    }
}

int getPipeToRead(const char* readPath)
{
    int pipe;

    pipe=open(readPath, O_RDONLY);

    if (pipe==-1)
    {
        errno=ENOENT;
        PrintError();
        exit(errno);
    }
    
    return pipe;
}

int getPipeToWrite(const char* writePath)
{
    int pipe;

    pipe=open(writePath, O_WRONLY);

    if (pipe==-1)
    {
        errno=ENOENT;
        PrintError();
        exit(errno);
    }
    
    return pipe;
}

vector<string> readFromPipe(int pipeToRead, int *outChoice)
{
    vector<string> inputNames;
    char arg[MAX_LEN_OF_ARG];
    int sizeOfArg, numOfArgs;

    read(pipeToRead, outChoice, sizeof(int));
    read(pipeToRead, &numOfArgs, sizeof(int));
    for (int i = 0; i < numOfArgs; i++)
    {
        read(pipeToRead, &sizeOfArg, sizeof(int));
        read(pipeToRead, arg, sizeOfArg);
        arg[sizeOfArg] = '\0';
        inputNames.push_back((string)arg);
    }
    close(pipeToRead);
    return inputNames;
}

void getLocalData(vector<Airport>& airportsOfDB, vector<Aircraft>& aircraftsOfDB)
{
    vector<string> airportNames;
    vector<string> aircraftNames;

    airportNames = findAirportNames();
    aircraftNames = findAircraftsNames();

    for (auto airportName : airportNames)
    {
        airportsOfDB.push_back(readFlightsData(airportName));
    }
    for (auto aircraftName : aircraftNames)
    {
        aircraftsOfDB.push_back(readFlightsByAirCraft(aircraftName));
    }
}

vector<string> findAirportNames()
{
    vector<string> airportNames;
    DIR *dir;
    struct dirent *ent;
    string path = "./Api Data";
    dir = opendir(path.c_str());
    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string fileName = ent->d_name;
            regex pattern("[a-zA-Z]{4}");
            if (fileName.size() == 4 && regex_match(fileName, pattern) && isUpperCaseWord(fileName))
            {
                airportNames.push_back(fileName);
            }
        }
        closedir(dir);
    }
    return airportNames;
}

vector<string> findAircraftsNames()
{
    vector<string> aircraftNames;
    DIR *dir;
    struct dirent *ent;
    string path = "./Api Data";
    dir = opendir(path.c_str());
    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string fileName = ent->d_name;
            regex pattern("[A-Z0-9]{6}");
            if (fileName.size() == 6 && regex_match(fileName, pattern))
            {
                aircraftNames.push_back(fileName);
            }
        }
        closedir(dir);
    }
    return aircraftNames;
}

void ExecuteChoice(int choice, vector<string> &inputNames, vector<Airport>& airportsDB, vector<Aircraft>& aircraftsDB)
{
    fs::path cwd = fs::current_path();
    fs::path writePathFS=cwd/"sharedFolder"/"output";
    const char* writePath = writePathFS.c_str();
    int pipeToWrite;
    char nullTerminator='\0';

    pipeToWrite=getPipeToWrite(writePath);
    dup2(pipeToWrite, STDOUT_FILENO);
    switch (choice)
    {
    case FETCH_AIRPORTS_DATA:
        loadDatabase(inputNames);
        break;    
    case PRINT_ARRIVAL_FLIGHTS:
        getArrivalsDataByAirport(inputNames, airportsDB);
        break;
    case PRINT_FULL_SCHEDULE:
        getFullFlightsSCheduleFunctionByAirport(inputNames, airportsDB);
        break;
    case PRINT_AIRCRAFTS_FLIGHTS:
        getAirplaneDataByIcao24(inputNames, aircraftsDB);
        break;
    case ZIP_DB:
        zipDB();
        cout<<"Data has been zipped successfully"<<endl;
        break;
    case EXIT:
        cout<<"DB Service is dying..."<<endl;
    }
    write(pipeToWrite,&nullTerminator,1);
    close(pipeToWrite);
    if (choice==EXIT)
    {
        exit(0);
    }
}

void getAirplaneDataByIcao24(vector<string> &icao24Args, vector<Aircraft> &aircraftsDB)
{
    int index;
    for (auto aircraftName : icao24Args)
    {
        vector<string> arg;
        map<int, Flight> flightsMap;
        arg.clear();
        arg.push_back(aircraftName);
        index = isAicraftInDB(aircraftsDB, aircraftName);
        vector<Aircraft> database;
        database.clear();
        if (index != NOT_FOUND)
        {
            database.push_back(aircraftsDB[index]);
        }
        else
        {
            database = loadAircraftDatabase(arg);
        }
        flightsMap = sortAircraftFlights(database[0]);
        printAirPlaneData(flightsMap,aircraftName);
    }
}

void printAirPlaneData(map<int,Flight> &database, string aircraftName)
{
    int i = 1;

        if (aircraftName != "")
            cout << "Flights found for aircraft " << aircraftName << " are:" << endl;
        for (auto &pair : database)
        {
            Flight flight=pair.second;
            cout << i << ". " << flight.GetIcao24() << " departed from " << flight.GetOriginICOA() << " at ";
            printDateFromUnix(flight.GetDepartureTime());
            cout << " arrived in " << flight.GetDestinationICOA() << " at ";
            printDateFromUnix(flight.GetArrivalTime());
            cout << endl;
            i++;
        }
        i = 1;
        cout << endl;
}

void getFullFlightsSCheduleFunctionByAirport(vector<string> &airportsArgs, vector<Airport> &airportsDB)
{
    int index;
    for (auto airportName : airportsArgs)
    {
        vector<string> arg;
        arg.clear();
        arg.push_back(airportName);
        map<int, Flight> flightsMap;
        index = isAirportInDB(airportsDB, airportName);
        vector<Airport> database;
        database.clear();

        if (index != NOT_FOUND)
        {
            database.push_back(airportsDB[index]);
        }
        else
        {
            database = loadDatabase(arg);
        }
        flightsMap = sortAirportFlights(database[0]);
        printFullScheduleFlights(flightsMap, airportName);
    }
}

map<int, Flight> sortAirportFlights(const Airport &database) 
{
    map<int, Flight> flightsMap;

    for (auto flight : database.GetArrivalsArr())
    {
        flightsMap.insert({flight.GetDepartureTime(), flight});
    }
    for (auto flight : database.GetDeparturesArr())
    {
        flightsMap.insert({flight.GetArrivalTime(), flight});
    }

    return flightsMap;
}

map<int, Flight> sortAircraftFlights(const Aircraft &database)
{
    map<int, Flight> flightsMap;

    for (auto flight : database.GetFlightsArr())
    {
        flightsMap.insert({flight.GetDepartureTime(), flight});
    }

    return flightsMap;
}

void printFullScheduleFlights(map<int, Flight> &flightsMap, string argName)
{
    string departString = "departing to ", arrivalString = "arriving from ";
    int i = 1;
    if (!flightsMap.empty())
    {
        cout << "Schedule for airport " + argName + ":"<<endl;
    }

    for (auto it = flightsMap.begin(); it != flightsMap.end(); ++it)
    {

        cout << i << ". "
             << "Flight #" << it->second.GetCallSign() << " ";
        if (it->second.GetOriginICOA() == argName)
        {
            cout << departString << " " << it->second.GetDestinationICOA() << " at "; 
            printDateFromUnix(it->second.GetArrivalTime());
        }
        else if (it->second.GetDestinationICOA() == argName)
        {
            cout << arrivalString << " " << it->second.GetOriginICOA() << " at "; 
            printDateFromUnix(it->second.GetDepartureTime());
        }
        i++;
        cout << endl;
    }
}

void zip_folder(zip_t *archive, const std::string &folderPath, const std::string &parentFolder)
{
    DIR *dir = opendir(folderPath.c_str());
    if (dir)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            std::string entryName = entry->d_name;
            if (entryName != "." && entryName != "..")
            {
                std::string fullPath = folderPath + "/" + entryName;

                // Get the file/directory information
                struct stat st;
                if (stat(fullPath.c_str(), &st) == 0)
                {
                    if (S_ISDIR(st.st_mode))
                    {
                        // Recursively zip subdirectories
                        std::string subFolderName = parentFolder + entryName + "/";
                        zip_folder(archive, fullPath, subFolderName);
                    }
                    else
                    {
                        // Add files to the zip archive
                        std::string entryPath = parentFolder + entryName;
                        zip_source_t *source = zip_source_file(archive, fullPath.c_str(), 0, -1);
                        zip_file_add(archive, entryPath.c_str(), source, ZIP_FL_OVERWRITE);
                    }
                }
            }
        }
        closedir(dir);
    }
}

void zipDB()
{
    std::string folderName = "Api Data";
    std::string zipFileName = "Api Data.zip";

    // Check if the folder exists
    DIR *dir = opendir(folderName.c_str());
    if (dir)
    {
        closedir(dir);

        // Open the zip archive for writing
        zip_t *archive = zip_open(zipFileName.c_str(), ZIP_CREATE | ZIP_TRUNCATE, NULL);
        if (archive)
        {
            // Add the folder and its contents to the zip archive
            zip_folder(archive, folderName, "");

            // Close the zip archive
            zip_close(archive);
        }
        else
        {
            std::cout << "Failed to create the zip archive." << std::endl;
        }
    }
    else
    {
        std::cout << "No data found to zip." << std::endl;
    }
}

bool compareByDepartureTime(const Flight &flight1, const Flight &flight2)
{
    return flight1.GetDepartureTime() < flight2.GetDepartureTime();
}

void getArrivalsDataByAirport(vector<string> &airportsArgs, vector<Airport> &airportsDB)
{
    int index;
    for (auto airportName : airportsArgs)
    {
        vector<string> arg;
        arg.clear();
        arg.push_back(airportName);
        index = isAirportInDB(airportsDB, airportName);
        vector<Airport> database;
        database.clear();
        if (index != NOT_FOUND)
        {
            database.push_back(airportsDB[index]);
        }
        else
        {
            database = loadDatabase(arg);
        }
        printArrivalsData(database);
    }
}

int isAirportInDB(vector<Airport> &airportsDB, string airportName)
{
    for (int i = 0; i < airportsDB.size(); i++)
    {
        if (airportsDB[i].GetName() == toUpperWord(airportName))
        {
            return i;
        }
    }
    return NOT_FOUND;
}

int isAicraftInDB(vector<Aircraft> &aircraftDB, string aircraftName)
{
    for (int i = 0; i < aircraftDB.size(); i++)
    {
        if (aircraftDB[i].GetName() == toUpperWord(aircraftName))
        {
            return i;
        }
    }
    return NOT_FOUND;
}

void printArrivalsData(vector<Airport> &database)
{
    int i, j = 1;
    int size = database.size();
    string airportName;
    for (i = 0; i < size; i++)
    {
        airportName = database[i].GetName();
        if (airportName != "")
            cout << "The arriving flights for airport " << airportName << " are: " << endl;
        for (auto &flight : database[i].GetArrivalsArr())
        {
            cout << j << ". "
                 << "Flight #" << flight.GetCallSign() << "arriving from " << flight.GetOriginICOA() << ", took off at ";
            printDateFromUnix(flight.GetDepartureTime());
            cout << " landed at ";
            printDateFromUnix(flight.GetArrivalTime());
            cout << endl;
            j++;
        }
        j = 1;
        cout << endl;
    }
}

void removeDir(string folderName)
{
    string removeDir = "rm -r " + folderName;
    const char *removeFolder = removeDir.c_str();
    system(removeFolder);
}

void printDateFromUnix(int unixNumber)
{
    time_t time = (time_t)unixNumber;
    tm *tmTime = gmtime(&time);
    cout << tmTime->tm_mday << "." << tmTime->tm_mon + 1 << "." << tmTime->tm_year + 1900;
    cout << " ";
    if (to_string((int)tmTime->tm_hour).length() == 1)
        cout << "0";
    cout << tmTime->tm_hour << ":";
    if (to_string((int)tmTime->tm_min).length() == 1)
        cout << "0";
    cout << tmTime->tm_min;
}

void CreateNamedPipe(const char* filePath)
{
    int pipePermissions=0666;

    if (mkfifo(filePath,pipePermissions)==-1 && errno!=EEXIST)
    {
       errno=ENOENT;
       PrintError();
    }
}
