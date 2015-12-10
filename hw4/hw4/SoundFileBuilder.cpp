#include "SoundFileBuilder.hpp"
#include "util.hpp"
#include "SoundFileLogger.hpp"
#include <istream>
const int NON_INITIALIZED_INT = -9999;

SoundFileBuilder::SoundFileBuilder(){}

/*
    @param input, the name of the file to create the SoundFile from, or "" by default. If no file name is provided, a SoundFile is built by reading from stdin.
    @param multiplyValue the optional value to multiply SoundFile::channels sample data by.
    @return a pointer to a newly constructed SoundFile, or NULL if there was an error processing the input.
*/
SoundFile* SoundFileBuilder::buildCS229SoundFileFromInput(string input /* default = "" */, unsigned int multiplyValue /* default = 1 */){

    SoundFile* soundFile = NULL;
    //SoundFile member variables to initialize
    int bitRes = NON_INITIALIZED_INT;
    int numChannels = NON_INITIALIZED_INT;
    int numSamples = NON_INITIALIZED_INT; //the only nonrequired field
    int sampleRate = NON_INITIALIZED_INT;
    
    ifstream file(input);
    if (file && !isValidFileType(input) ) return NULL; //failure

    string line;
    bool isFirstLine = true;
    while ( getline (file.is_open() ? file : cin, line) ) {
            vector<string> lineVector = getStringVectorFromLine(line);
            if (shouldIgnoreLine(lineVector)) {
                continue;
            }
            if (isFirstLine && !isCS229Heading(lineVector[0])) {
                cout << ".cs229 files must begin with CS229" << endl;
                return NULL; //failure
            }
            if (isFirstLine){
                isFirstLine = false;
                continue;
            }
            
            // Check if parsed header value in this line is a valid, check the value after it and store the its value.
            // ex.  samples 4
            //      bitReS 2048
            if (lineVector[0].compare("samples")==0) {
                if(!parseAndStoreStringIntoInt(numSamples, lineVector[1])) return NULL;
            }else if(lineVector[0].compare("channels")==0) {
                if(!parseAndStoreStringIntoInt(numChannels, lineVector[1])) return NULL;
            }else if(lineVector[0].compare("bitres")==0) {
                if(!parseAndStoreStringIntoInt(bitRes, lineVector[1])) return NULL;
            }else if(lineVector[0].compare("samplerate")==0) {
                if(!parseAndStoreStringIntoInt(sampleRate, lineVector[1])) return NULL;
            }else if(lineVector[0].compare("startdata")==0) {
                
                //We are in the StartData section now, check if the necessary variables have been provided and instantiate a SoundFile
                if (!areSoundFileDataValuesInitialzed(bitRes, numChannels, sampleRate)) {
                    return NULL; //failure
                }
                soundFile = new SoundFile(input, bitRes, numChannels, sampleRate, numSamples);
                break;
            } else {
                // the header type isn't recognized, there is some sort of error in the file.
                cout << "Invalid header: " << lineVector[0] << " this file cannot be parsed." << endl;
            }
    }
    if(!addStartDataToSoundFile(&soundFile, file, multiplyValue)){
        //there was an error parsing the startdata
        delete soundFile;
        return NULL;
    }
    if(file){
        file.close();
    }
    *soundFile = ((SoundFile)*soundFile) * multiplyValue;
    SoundFileLogger::logInstance(soundFile);
    return soundFile;
}

bool SoundFileBuilder::isValidFileType(string fileName, bool printOutput){
    const string cs229FileExtenstion = ".cs229";
    //const string wavFileExtenstion = ".wav";
    size_t found = fileName.find(cs229FileExtenstion);
    if (found==string::npos){ //&& fileName.find(wavFileExtenstion)==string::npos
        if (printOutput) {
            cout << "File must be of type .cs229 or .wav" << endl;
        }
        return false; //failure
    }
    return true;
}

/*
    @return true if @heading is some form of cs229
 */
bool SoundFileBuilder::isCS229Heading(string heading){
    const string cs229LowerCase = "cs229";
    return (cs229LowerCase.compare(heading) == 0);
}

/*
    Function used in buildSoundFileFromFileName to determine if the line contained by @lineVector should be skipped while reading the file.
    @return true if the line is blank or begins with a #comment
 */
bool SoundFileBuilder::shouldIgnoreLine(vector<string> lineVector){
    return (lineVector.size()== 0 || lineVector[0].compare(0, 1, "#") == 0);
}

/*
    It's pretty clear what this one does imo.
*/
bool SoundFileBuilder::areSoundFileDataValuesInitialzed(int bitRes, int numChannels, int sampleRate){
    if (bitRes == NON_INITIALIZED_INT || bitRes<=0){
        fprintf(stderr, "A valid BitRes value must be provided.\n");
        return false;
    }
    if (numChannels == NON_INITIALIZED_INT){
        fprintf(stderr, "A value for Channels must be provided.\n");
        return false;
    }else if(numChannels <= 0){
        fprintf(stderr, "Invalid number of Channels: %d.\n", numChannels);
        return false;
    }
    
    if (sampleRate == NON_INITIALIZED_INT || sampleRate<=0){
        fprintf(stderr, "A valid Samplerate value must be provided.\n");
    }
    return true;
}

/*  
    This function reads in sample data from @input until it reaches "<EOF>"
    @param soundFile, a pointer to a SoundFile pointer which will be filled with @input startdata
    @param file, the file to read startdata from
    @return true if StartData was successfully added to the SoundFile.
*/
bool SoundFileBuilder::addStartDataToSoundFile(SoundFile** soundFile, istream& input, unsigned int multiplyValue){
    if (!soundFile || !(*soundFile)) {
        return false;
    }
    string line;
    int numSamples = 0;
    while ( getline(input,line) ) {
        vector<string> lineVector = getStringVectorFromLine(line);
        for (int i = 0; i < lineVector.size(); i++) {
            if (lineVector[i].compare("<EOF>") == 0 || lineVector[i].compare("<eof>") == 0) {
                break;
            }
            int sample;
            if(!isValidSample(*soundFile, sample, lineVector[i], multiplyValue)){
                fprintf(stderr, "Sample %s in StartData section is not a valid sample.\n", (lineVector[i]).c_str());
                return false;
            }else{
                //messy dereferencing of the pointer to the #SoundFile
                (*((*soundFile)->getChannels()))[i].push_back(sample); //channels[i] is a vector<signed int> which holds data for one channel
                numSamples++;
            }
        }
    }
    (*soundFile)->setNumSamples(numSamples); // set the number of samples because this is not a required value for .cs229 files
    return true;
}