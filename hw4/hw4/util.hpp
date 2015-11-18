#ifndef util_hpp
#define util_hpp
#include "all.h"
#include "SoundFile.hpp"
using namespace std;

string toLowerCase(string token);
string getFirstTokenFromLine(string line);
vector<string> getStringVectorFromLine(string line);
bool parseAndStoreStringIntoInt(int &i, string str);
int getMedianNumChannels(vector<SoundFile*> values);

#endif /* util_hpp */
