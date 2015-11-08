#include "SoundFileBuilder.hpp"
#include "util.hpp"
const int NOT_INITIALIZED_INT = -9999;

SoundFileBuilder::SoundFileBuilder(){}

/*
 @param fileName, the name of the file to create the SoundFile from
 @return a pointer to a newly constructed SoundFile, or NULL if there was an error processing the file.
*/
SoundFile* SoundFileBuilder::buildSoundFileFromFileName(string fileName){

    SoundFile* soundFile = NULL;
    int bitRes = NOT_INITIALIZED_INT;
    int numChannels = NOT_INITIALIZED_INT;
    int numSamples = NOT_INITIALIZED_INT;
    int sampleRate = NOT_INITIALIZED_INT;
    
    if (!isValidFileType(fileName))return NULL; //failure
    
    ifstream myfile (fileName);
    if (myfile.is_open())
    {
        string line;
        int lineNumber = 0;
        while ( getline (myfile,line) )
        {
            vector<string> lineVector = getStringVectorFromLine(line);
            if (shouldIgnoreLine(lineVector)) {
                continue;
            }
            if (lineNumber == 0 && !containsCS229Heading(lineVector[0])) {
                cout << ".cs229 files must begin with CS229" << endl;
                return NULL; //failure
            }
            if (lineVector[0].compare("samples")==0) {
                if(!stringToInt(numSamples, lineVector[1])) return NULL;
            }else if(lineVector[0].compare("channels")==0) {
                if(!stringToInt(numChannels, lineVector[1])) return NULL;
            }else if(lineVector[0].compare("bitres")==0) {
                if(!stringToInt(bitRes, lineVector[1])) return NULL;
            }else if(lineVector[0].compare("samplerate")==0) {
                if(!stringToInt(sampleRate, lineVector[1])) return NULL;
            }else if(lineVector[0].compare("startdata")==0) {
                if (!areSoundFileDataValuesInitialzed(bitRes, numChannels, sampleRate)) { //TODO check for invalid values like -1
                    return NULL; //failure
                }
                soundFile = new SoundFile(fileName, bitRes, numChannels, sampleRate, numSamples);
                break;
            }

            lineNumber++;
        }
        
        // iterate over startdata and at it to soundFile.
        while (getline (myfile,line)){
            cout << line << endl;
        }
        
        myfile.close();
    }
    else cout << "Unable to open file " << fileName << endl;
    return soundFile;
}

bool SoundFileBuilder::isValidFileType(string fileName){
    const string cs229FileExtenstion = ".cs229";
    size_t found = fileName.find(cs229FileExtenstion);
    if (found==std::string::npos){
        cout << "File must be of type .cs229" << endl;
        return false; //failure
    }
    return true;
}

/*
    @return true if the first nonignored line contains some form of cs229
 */
bool SoundFileBuilder::containsCS229Heading(string heading){
    const string cs229LowerCase = "cs229";
    return (cs229LowerCase.compare(heading) == 0);
}

/*
    Function used in buildSoundFileFromFileName to determine if a line should be skipped while reading the file.
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
    if (bitRes == NOT_INITIALIZED_INT){
        cout << "A BitRes value must be provided" << endl;
        return false;
    }
    if (numChannels == NOT_INITIALIZED_INT){
        cout << "A Channels value must be provided" << endl;
    }
    if (sampleRate == NOT_INITIALIZED_INT){
        cout << "A SampleRate value must be provided" << endl;
    }
    return true;
}
