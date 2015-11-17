#include "SndMix.hpp"
#include "SndCat.hpp"
#include "util.hpp"

void SndMix::runProgram(){
    //check for swtiches
    runSwitches();
    // call SndCat to mix all soundfiles together 
}

/*
    @param string outputFileName the output file name.
    @return true if @outputFileName is a valid file.
 */
bool SndMix::oSwitch(string outputFileName){
    return SndCat::oSwitch(outputFileName);
}

string SndMix::getProgramName(){
    return "SndMix";
}

string SndMix::getProgramDescription(){
    return "This program reads all sound files passed as arguments, and “mixes” them into a single"
    "sound file. The program is invoked as"
    "sndmix [switches] mult1 file1 ...multn filen"
    "where the sample data of filei is multiplied by multi (a real value between -10 and 10)";
}

/*  
    @override SoundProgram::initSoundFiles
    @param arguments the file arguments to iterate over and store SoundFile object representations of in #soundFiles.
    Switches have already been removed from @arguments in #initSwitchArgumentMap,
    if the argument cannot be made into a #SoundFile, an error will be printed to stderr.
*/
void SndMix::initSoundFiles(vector<string> arguments){
    vector<string>::iterator iter = arguments.begin();
    while (iter!=arguments.end()) {
        string multiArg = *iter;
        int multiplier = getMultiplyValueFromSndMixArg(multiArg);
        try {
            iter++;
            string fileName = *iter;
            SoundFile* soundFile = this->soundFileBuilder->buildSoundFileFromInput(fileName, multiplier);
            this->soundFiles.push_back(soundFile);
        }
        catch (...){
            fprintf(stderr,  "No matching file to multi arg mult%i", multiplier);
        }
        iter++;
    }
}

/*
    @param str the string argument to parse the multiplier value from.
    @return an int multiplier value to be used by #SndMix. If an invalid value cannot be parsed from @str, an error is printed to stderr and the program exits.
 */
int SndMix::getMultiplyValueFromSndMixArg(string str) {
    int multValue;
    const string mult = "mult";
    size_t found = str.find(mult);
    if (found!=string::npos){
        string parsedValue = str.substr(found + mult.length(), str.length());
        if (!parseAndStoreStringIntoInt(multValue, parsedValue)){
            fprintf(stderr, "Non int multiplier value passed to SndMix: %s", parsedValue.c_str());
        }
    } else {
        fprintf(stderr, "Invalid argument passed to SndMix: %s", str.c_str());
        exit(0);
    }
    return multValue;
}