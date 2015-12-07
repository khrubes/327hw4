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
    @param i the int we want to store @str value into
    @param str the string we want to convert
    @return true if str can be represented as an int
*/
bool parseAndStoreStringIntoInt(int &i, string str) {
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

/*
    @param values a #SoundFile* vector to compute the median number of channels from.
    @return the median number of samples computed from all #SoundFiles
 */
int getMedianNumChannels(vector<SoundFile*> values){
    vector<int> channels;
    for (int i = 0; i<values.size(); i++) {
        channels.push_back( values[i]->getNumChannels() );
    }
    return channels[ channels.size() /2 ];
}

/*
    Converts a c string array into a vector of strings.
    @param argc the number of arguments
    @param argv the c string array of arguments
    @return a vector<string> containing the c strings in argv
*/
vector<string> charArrayToStringVector(int argc, const char* argv[]){
    vector<string> arguments;
    for (int i = 1; i < argc; i++) { // skip over program name
        arguments.push_back(argv[i]);
    }
    return arguments;
}

/*
    @return true if @param input is a string representing a valid number.
*/
bool isNumeric(string& input) {
    size_t indexOfDecimal = input.find(".");
    if (indexOfDecimal!=string::npos){
        //strip the decimal from input
        string temp = input.substr(0, indexOfDecimal);
        temp+= input.substr(indexOfDecimal+1, input.length());
        input = temp;
    }
    return std::all_of(input.begin(), input.end(), ::isdigit);
}
