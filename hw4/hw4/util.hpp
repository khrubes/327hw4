#ifndef util_hpp
#define util_hpp
#include "all.h"
#include "SoundFile.hpp"
using namespace std;

string toLowerCase(string token);
string getFirstTokenFromLine(string line);
vector<string> getStringVectorFromLine(string line);
bool parseAndStoreStringIntoInt(int &i, string str);
bool isValidSample(SoundFile* soundFile, int& sampleValueToStoreInto, string sampleStringFromFile, signed int multiplyValue);
vector<string> charArrayToStringVector(int argc, const char* argv[]);
bool isNumeric(string& input);
#endif /* util_hpp */
