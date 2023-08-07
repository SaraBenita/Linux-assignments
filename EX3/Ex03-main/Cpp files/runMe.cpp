#include "runMe.h"

int main()
{
    int fatherToSonPipe[2], sonToFatherPipe[2];
    int choice;
    pid_t childPid;

    MakePipes(fatherToSonPipe);
    MakePipes(sonToFatherPipe);

    childPid = SplitUsingFork();

    if (childPid == 0)
    {
        dup2(sonToFatherPipe[WRITE], STDOUT_FILENO);
        extractDB();
        signal(SIGUSR1, handleSIGUSR1);
        close(sonToFatherPipe[READ]);
        close(fatherToSonPipe[WRITE]);
        makeChildPart(fatherToSonPipe);
    }
    else // this is papa
    {
        signal(SIGINT, handleSIGINT);
        close(sonToFatherPipe[WRITE]);
        close(fatherToSonPipe[READ]);
        makeFatherPart(fatherToSonPipe, sonToFatherPipe, childPid);
    }
    return 0;
}

string readStringFromPipe(int pipefd)
{
    const int sizeToRead = 1024;
    char *buffer = new char[sizeToRead];
    int bufferSize = sizeToRead;

    string receivedString;

    ssize_t bytesRead;
    while ((bytesRead = read(pipefd, buffer, sizeToRead)) > 0)
    {
        receivedString.append(buffer, bytesRead);
        if (bytesRead == bufferSize)
        {
            bufferSize *= 2;
            char *resizedBuffer = new char[bufferSize];
            memcpy(resizedBuffer, buffer, bytesRead);
            delete[] buffer;
            buffer = resizedBuffer;
        }
    }

    delete[] buffer;

    close(pipefd);

    return receivedString;
}

void makeChildPart(int *fatherToSonPipe)
{
    vector<string> inputNames;
    int choice;
    vector<string> airportNames;
    vector<string> aircraftNames;
    vector<Airport> airportsOfDB;
    vector<Aircraft> aircraftsOfDB;

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

    inputNames = ReadFromPipe(fatherToSonPipe[READ], &choice);
    ExecuteChoice(choice, inputNames, airportsOfDB, aircraftsOfDB);
}

void makeFatherPart(int *fatherToSonPipe, int *sonToFatherPipe, pid_t childPid)
{

    int childExitStatus, choice, signalChoice;
    vector<string> inputNames;
    string toPrint;
    PrintMenu();
    choice = GetChoice();
    if (choice >= 1 && choice <= 4)
    {
        if (choice != UPDATE)
        {
            inputNames = GetData(choice); // put in if in case no need for data
        }
        cout << "In cases where some of the data is stored on the server, the process could take a few moments." << endl;
        cout << "Please wait..." << endl;
    }

    WriteToPipe(fatherToSonPipe[WRITE], choice, inputNames);

    if (choice == GET_CHILD_PID)
    {
        cout << "My child's pid is " << childPid << endl;
    }
    else if (choice == EXIT)
    {
        informChildOnGracefulExit(childPid);
    }

    waitpid(childPid, &childExitStatus, 0);

    toPrint = readStringFromPipe(sonToFatherPipe[READ]);
    cout << toPrint << endl;
    if (choice >= 1 && choice <= 4)
    {
        cout << "Updating database..." << endl;
        zipDB();
    }
    cout << "Child process terminated with signal: " << childExitStatus << endl;
}

void ExecuteChoice(int choice, vector<string> &inputNames, vector<Airport> airportsDB, vector<Aircraft> aircraftsDB)
{

    switch (choice)
    {
    case ARRIVAL_FLIGHTS:
        getArrivalsDataByAirport(inputNames, airportsDB);
        break;
    case FULL_SCHEDULE:
        getFullFlightsSCheduleFunctionByAirport(inputNames, airportsDB);
        break;
    case AIRCRAFTS_FLIGHTS:
        getAirplaneDataByIcao24(inputNames, aircraftsDB);
        break;
    case UPDATE:
        updateDatabase();
        break;
    case ZIP_DB:
        zipDB();
        break;
    }
}

void informChildOnGracefulExit(pid_t childPid)
{
    cout << "Performing exit, letting my child know that a graceful exit is requested" << endl;
    kill(childPid, SIGUSR1);
}

void handleSIGINT(int signal)
{
    cout << endl
         << "SIGINT signal received. Zipping database if available." << std::endl;
    zipDB();
    std::exit(signal);
}

void handleSIGUSR1(int signal)
{
    cout << "SIGUSR1 signal received from father. Zipping database if available" << endl;
    zipDB();
    std::exit(signal);
}

vector<string> ReadFromPipe(int pipeToRead, int *outChoice)
{
    vector<string> inputNames;
    char arg[MAX_LEN_OF_ARG];
    int sizeOfArg, numOfArgs;

    read(pipeToRead, outChoice, sizeof(int));
    read(pipeToRead, &numOfArgs, sizeof(int));
    for (int i = 0; i < numOfArgs; ++i)
    {
        read(pipeToRead, &sizeOfArg, sizeof(int));
        read(pipeToRead, arg, sizeOfArg);
        arg[sizeOfArg] = '\0';
        inputNames.push_back((string)arg);
    }
    return inputNames;
    close(pipeToRead);
}

void WriteToPipe(int pipeToWrite, int choice, vector<string> &inputNames)
{
    int size = inputNames.size(), sizeOfArg;
    char *str;
    write(pipeToWrite, &choice, sizeof(int));
    write(pipeToWrite, &size, sizeof(int));
    for (auto arg : inputNames)
    {
        sizeOfArg = arg.size();
        write(pipeToWrite, &sizeOfArg, sizeof(int));
        write(pipeToWrite, arg.c_str(), arg.size());
    }
    close(pipeToWrite);
}

vector<string> GetData(int choice)
{
    vector<string> inputNames;
    bool isAirport = true;

    if (choice >= 1 && choice <= 2)
    {
        inputNames = GetNames(isAirport);
    }
    else if (choice == 3)
    {
        isAirport = false;
        inputNames = GetNames(isAirport);
    }
    return inputNames;
}

vector<string> GetNames(bool isAirport)
{
    string arg;
    bool isFinish = false;
    vector<string> allValidNames;
    cout << "Note: the data you request will be fetched from the local database if possible,\nIf you wish to get updated data, use the 'Update Database' option in main menu.\n"
         << endl;
    cout << "Please write the arguments seperated by enter, to finish type 'E' (end)" << endl;
    getline(cin, arg);
    while (!(isFinish && !allValidNames.empty()))
    {
        if (arg == "E")
        {
            isFinish = true;
        }
        else
        {
            if (isAirport)
            {
                if (isValidAirport(arg))
                {
                    allValidNames.push_back(arg);
                }
                else
                {
                    PrintError();
                }
            }
            else
            {
                if (isValidIcao24(arg))
                {
                    allValidNames.push_back(arg);
                }
                else
                {
                    PrintError();
                }
            }
            getline(cin, arg);
        }
    }

    return allValidNames;
}

void PrintError()
{
    cout << "Error: " << strerror(errno) << " (error number: " << errno << ")" << endl;
}

int GetChoice()
{
    string choiceStr;
    getline(cin, choiceStr);

    while (!isValidChoice(choiceStr))
    {
        errno = EINVAL;
        PrintError();
        getline(cin, choiceStr);
    }

    return atoi(choiceStr.c_str());
}

bool isValidChoice(const string &str)
{
    if (str == "1" || str == "2" || str == "3" || str == "4" || str == "5" || str == "6" || str == "7")
        return true;
    return false;
}

void PrintMenu()
{
    cout << "Please choose one of the following commands" << endl;
    cout << "1 - Fetch airports incoming flights." << endl;
    cout << "2 - Fetch airports full flights schedule." << endl;
    cout << "3 - Fetch aircraft incoming flights." << endl;
    cout << "4 - Update DB" << endl;
    cout << "5 - Zip DB Files." << endl;
    cout << "6 - Get child process Pid." << endl;
    cout << "7 - Exit." << endl;
    cout << "Please type your choice <1,2,3,4,5,6,7>" << endl;
}

void MakePipes(int *pipes)
{
    if (pipe(pipes) == -1)
    {
        cout << "Failed to make pipe" << endl;
        std::exit(1);
    }
}

pid_t SplitUsingFork()
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        cout << "Fork procedure failed!" << endl;
        std::exit(1);
    }
    return pid;
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

            std::cout << "Database zipped successfully." << std::endl;
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

void updateDatabase()
{
    vector<string> airportNames = findAirportNames();
    vector<string> aircraftNames = findAircraftsNames();
    loadDatabase(airportNames);
    loadAircraftDatabase(aircraftNames);
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