#include "flightsService.h"

int main()
{
    int choice,pipeToWrite,pipePermissions=0666,pipeToRead;
    fs::path cwd = fs::current_path();
    fs::path writePathFS=cwd/"sharedFolder"/"userInput";
    const char* writePath = writePathFS.c_str();
     fs::path readPathFS=cwd/"sharedFolder"/"output";
    const char* readPath = readPathFS.c_str();
    vector<string> inputNames;

    while (true)
    {
        PrintMenu();
        choice = GetChoice();

        if (choice >= 1 && choice <= 4)
        {
            inputNames = GetData(choice); // put in if in case no need for data
            cout << "In cases where some of the data is stored on the server, the process could take a few moments." << endl;
            cout << "Please wait..." << endl;
        }

        pipeToWrite=getPipeToWrite(writePath);
        WriteToPipe(pipeToWrite,choice,inputNames);
        pipeToRead=getPipeToRead(readPath);
        printDataFromPipe(pipeToRead);
        if (choice==6)
        {
            cout<<"Thanks for using me, bye :)"<<endl;
            exit(0);
        }
    }
    
}

void PrintMenu()
{
    cout << "Please choose one of the following commands" << endl;
    cout << "1 - Fetch airports data" << endl;
    cout << "2 - Print airports incoming flights" << endl;
    cout << "3 - Print airports full flights schedule" << endl;
    cout << "4 - Print aircraft full flights schedule" << endl;
    cout << "5 - Zip DB Files" << endl;
    cout << "6 - Exit" << endl;
    cout << "Please type your choice <1,2,3,4,5,6>" << endl;
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

vector<string> GetData(int choice)
{
    vector<string> inputNames;
    bool isAirport = true;

    if (choice >= 1 && choice <= 3)
    {
        inputNames = GetNames(isAirport);
    }
    else if (choice == 4)
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
    cout << "Please write at least one valid argument (seperate multiple arguments by enter)\nTo finish type 'E' (end)" << endl;
    getline(cin, arg);
    while (!isFinish || allValidNames.empty())
    {
        if (arg == "E" && !allValidNames.empty())
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

bool isValidChoice(const string &str)
{
    if (str == "1" || str == "2" || str == "3" || str == "4" || str == "5" || str == "6")
        return true;
    return false;
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

void printDataFromPipe(int pipefd)
{
    const int bufferSize=1024;
    char buffer[bufferSize];
    ssize_t bytesRead;
    bool isBlocking=true;
    int flags;

    while ((bytesRead = read(pipefd, buffer, sizeof(buffer)-1)) > 0)
    {
        buffer[bytesRead]='\0';
        cout<<buffer;

        if (buffer[bytesRead-1]=='\0')
        {
            break;
        }

    /*
    if (isBlocking)
        {
        flags = fcntl(pipefd, F_GETFL, 0);
        fcntl(pipefd, F_SETFL, flags | O_NONBLOCK);
        isBlocking=false;
        }
    */
    }

   close(pipefd);
}

