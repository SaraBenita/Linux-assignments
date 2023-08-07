#include "update.h"

int main()
{
    vector<string> airportNames = findAirportNames();
    loadDatabase(airportNames);
}

vector<string> findAirportNames()
{
    vector<string> airportNames;
    DIR *dir;
    struct dirent *ent;
    string path = ".";
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
                removeDir(fileName);
            }
        }
        closedir(dir);
    }
    else
    {
        cerr << "Could not open directory" << endl;
    }
    return airportNames;
}

void removeDir(string folderName)
{
    string removeDir="rm -r "+folderName;
    const char *removeFolder=removeDir.c_str();
    system(removeFolder);
}