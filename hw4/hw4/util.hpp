#ifndef util_hpp
#define util_hpp
#include "all.h"
using namespace std;

string toLowerCase(string token);
string getFirstTokenFromLine(string line);
vector<string> getStringVectorFromLine(string line);
bool stringToInt(int &i, string str);

#endif /* util_hpp */
