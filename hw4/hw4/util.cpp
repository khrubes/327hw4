#include "util.hpp"
/*
    A set of util functions not specific to any class
*/

string toLowerCase(string token){
    transform(token.begin(), token.end(), token.begin(), ::tolower);
    return token;
}

/*
    @param line, the line to build the vector from
    @return a vector of strings, each index representing a string in the line with a space as delimiter.
 */
vector<string> getStringVectorFromLine(string line){
    vector<string> stringsInLine;
    string token;
    istringstream iss(line);
    while(getline(iss, token, ' ')){
        token = toLowerCase(token);
        stringsInLine.push_back(token);
    }
    return stringsInLine;
}

/*  
    @param i the int we want to store our string value into
    @param str the string we want to convert
    @return true if str can be represented as an int
*/
bool stringToInt(int &i, string str) {
    const char* s = str.c_str();
    char c;
    std::stringstream ss(s);
    ss >> i;
    if (ss.fail() || ss.get(c)) {
        // not an int or partially an int with other values left over
        return false;
    }
    return true;
}