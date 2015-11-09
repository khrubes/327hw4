#include "SoundFileBuilder.hpp"
#include "util.hpp"
const int NON_INITIALIZED_INT = -9999;

SoundFileBuilder::SoundFileBuilder(){}

/*
 @param fileName, the name of the file to create the SoundFile from.
 @return a pointer to a newly constructed SoundFile, or NULL if there was an error processing the file.
*/
SoundFile* SoundFileBuilder::buildSoundFileFromFileName(string fileName){

    SoundFile* soundFile = NULL;
    //SoundFile member variables to initialize
    int bitRes = NON_INITIALIZED_INT;
    int numChannels = NON_INITIALIZED_INT;
    int numSamples = NON_INITIALIZED_INT; //the only nonrequired field
    int sampleRate = NON_INITIALIZED_INT;
    
    if ( !isValidFileType(fileName) ) return NULL; //failure
    
    ifstream file (fileName);
    if ( file.is_open() )
    {
        string line;
        bool isFirstLine = true;
        while ( getline (file,line) )
        {
            vector<string> lineVector = getStringVectorFromLine(line);
            if (shouldIgnoreLine(lineVector)) {
                continue;
            }
            if (isFirstLine && !containsCS229Heading(lineVector[0])) {
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
                if (!areSoundFileDataValuesInitialzed(bitRes, numChannels, sampleRate)) { //TODO check for invalid values like -1
                    return NULL; //failure
                }
                soundFile = new SoundFile(fileName, bitRes, numChannels, sampleRate, numSamples);
                break;
            } else {
                // the header type isn't recognized, there is some sort of error in the file.
                cout << "Invalid header: " << lineVector[0] << " this file cannot be parsed." << endl;
            }
        }
        if(!addStartDataToSoundFile(&soundFile, file)){
            //there was an error parsing the startdata
            free(soundFile);
        }
        file.close();
    }
    else cout << "Unable to open file " << fileName << endl;
    return soundFile;
}

bool SoundFileBuilder::isValidFileType(string fileName){ //TODO support .wav ?
    const string cs229FileExtenstion = ".cs229";
    size_t found = fileName.find(cs229FileExtenstion);
    if (found==string::npos){
        cout << "File must be of type .cs229" << endl;
        return false; //failure
    }
    return true;
}

/*
    @return true if @heading contains some form of cs229
 */
bool SoundFileBuilder::containsCS229Heading(string heading){
    const string cs229LowerCase = "cs229";
    return (cs229LowerCase.compare(heading) == 0);
}

/*
    Function used in buildSoundFileFromFileName to determine if the line contained by @lineVector should be skipped while reading the file.
    @return true if the line is blank or begins with a #comment
 */
bool SoundFileBuilder::shouldIgnoreLine(vector<string> lineVector){
    //string token = getFirstTokenFromLine(line);
    return (lineVector.size()== 0 || lineVector[0].compare(0, 1, "#") == 0);
}

/*
    It's pretty clear what this one does imo.
*/
bool SoundFileBuilder::areSoundFileDataValuesInitialzed(int bitRes, int numChannels, int sampleRate){
    if (bitRes == NON_INITIALIZED_INT){
        cout << "A BitRes value must be provided" << endl;
        return false;
    }
    if (numChannels == NON_INITIALIZED_INT){
        cout << "A Channels value must be provided" << endl;
    }
    if (sampleRate == NON_INITIALIZED_INT){
        cout << "A SampleRate value must be provided" << endl;
    }
    return true;
}

/*
    @param soundFile, a pointer to a SoundFile pointer which will be filled with @file startdata
    @param file, the file to read startdata from
    @return true if StartData was successfully added to the SoundFile
*/
bool SoundFileBuilder::addStartDataToSoundFile(SoundFile** soundFile, ifstream& file){
    string line;
    int numSamples = 0;
    while ( getline(file,line) ) {
        vector<string> lineVector = getStringVectorFromLine(line);
        for (int i = 0; i < (*soundFile)->channels.size(); i++) {
            signed int sample;
            if(!parseAndStoreStringIntoInt(sample, lineVector[i])){
                cout << "Sample \"" << lineVector[i] << "\"" << " in StartData section is not a valid sample." << endl;
                return false;
            }else{
                (*soundFile)->channels[i].push_back(sample); //channels[i] is a vector<signed int> which holds data for one channel
            }
        }
        numSamples++;
    }
    (*soundFile)->setNumSamples(numSamples); // set the number of samples because this is not a required value for .cs229 files
    return true;
}


