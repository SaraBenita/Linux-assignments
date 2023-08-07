#include "libUtility.h"

namespace fs=std::filesystem;

void PrintMenu();
int GetChoice();
vector<string> GetData(int choice);
vector<string> GetNames(bool isAirport);
bool isValidChoice(const string &str);
void WriteToPipe(int pipeToWrite, int choice, vector<string> &inputNames);
int getPipeToWrite(const char* writePath);
int getPipeToRead(const char* readPath);
void printDataFromPipe(int pipefd);


