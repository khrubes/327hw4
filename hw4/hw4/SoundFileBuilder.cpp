#include "SoundFileBuilder.hpp"
#include "util.hpp"
#include "SoundFileLogger.hpp"
#include <istream>
const int NON_INITIALIZED_INT = -9999;

SoundFileBuilder::SoundFileBuilder(){}

/*
    Builds a #SoundFile object from input in the form of a CS229 file.
    @param input, the name of the file to create the SoundFile from, or "" by default. If no file name is provided, a SoundFile is built by reading from stdin.
    @param multiplyValue the optional value to multiply SoundFile::channels sample data by.
    @return a pointer to a newly constructed SoundFile, or NULL if there was an error processing the input.
*/
SoundFile* SoundFileBuilder::buildSoundFileFromInput(string input /* default = "" */, unsigned int multiplyValue /* default = 1 */){

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

/*
 Plots the # samples calculated by @param sampleRate and @param totalSoundLength in the form of @param waveform
 Applies amplitudes to the sound wave depending on the current stage in the ADSR envelope, @param volumePeak, and @param bitdepth.
 This function assumes all input parameters are valid, the calling program is responsible for checking.
 @return a SoundFile built from the provided ADSR envelope.
*/
SoundFile* SoundFileBuilder::buildSoundFileFromADSREvelope(float attackTime, float decayTime, float sustainTime, float releaseTime, float totalSoundLength, float frequency, float volumePeak, float pf, int bitdepth, int sampleRate, string waveForm) {
    
    int numSamples = floor( totalSoundLength * sampleRate );
    SoundFile* soundFile = new SoundFile("" /* file name */, bitdepth, 1 /* num channels */, sampleRate, numSamples);
    vector<signed int> channel;
    channel.reserve(numSamples);
    for (int i = 0; i < numSamples; i++) {
        float currentTime = this->getXValue(i, numSamples, totalSoundLength);
        channel.push_back(this->getSampleValue(waveForm, currentTime, frequency, i, sampleRate, pf, volumePeak));
    }
    for (int sampleIndex = 0; sampleIndex < channel.size(); sampleIndex++) {
        float currentTime = this->getXValue(sampleIndex, numSamples, totalSoundLength);
        channel[sampleIndex]*= this->getAmplitudeValue(currentTime, attackTime, decayTime, sustainTime, releaseTime, totalSoundLength);
        channel[sampleIndex]*= pow(2, bitdepth) - 1;
    }
    soundFile->setChannels(vector< vector<signed int> >(1, channel));
    return soundFile;
}

/*
 @return an x value data point determined by @param sampleNum, and the length of the sound.
*/
float SoundFileBuilder::getXValue(int sampleNum, int totalNumSamples, float lengthOfSound){
    float result = (lengthOfSound / (totalNumSamples)) * (sampleNum + 1); // +1 used to avoid initial 0 value
    return result;
}

/*
 @return a sample value for the corresponding @param currentTime and the current sample number we are on @param sampleNum
 */
float SoundFileBuilder::getSampleValue(string waveFormType, float currentTime, float frequency, int sampleNum, int sampleRate, float pf, float maxVolume){
    float toReturn;
    if (waveFormType.compare("--sin")==0) {
        toReturn = this->getSinWaveValue(currentTime, 1/frequency);
    } else if(waveFormType.compare("--triangle")==0){
        toReturn = this->getTriangleWaveValue(currentTime, 1/frequency);
    }else if(waveFormType.compare("--sawtooth")==0){
        toReturn = this->getSawtoothWaveValue(currentTime, 1/frequency);
    }else if(waveFormType.compare("--pulse")==0){
        toReturn = this->getPulseWaveValue(currentTime, sampleNum, sampleRate, frequency, pf, maxVolume);
    }
    return toReturn;
}

/*
 @return a sin wave function depending on @param currentTime and @param period
 */
float SoundFileBuilder::getSinWaveValue(float currentTime, float period){
    float sinWaveValue = sin( period * currentTime);
    return sinWaveValue;
}

/*
 @return a triangle wave function depending on @param currentTime and @param period
 Follows the formula for a triangle wave from https://en.wikipedia.org/wiki/Triangle_wave, with period p - (1/ "-f")
 */
float SoundFileBuilder::getTriangleWaveValue(float currentTime, float period){
    //* this->getAmplitudeValue(currentTime)
    float triangleWaveValue = (2/M_PI) * asin(  sin( ((2*M_PI) / period) * currentTime )  );
    return triangleWaveValue;
}

/*
 @return a sawtooth wave function depending on @param currentTime and @param period.
 Follows the formula for a sawtooth wave from https://en.wikipedia.org/wiki/Sawtooth_wave, with period p - (1/ "-f") and amplitude a given by getAmplitudeValue(currentTime)
 */
float SoundFileBuilder::getSawtoothWaveValue(float currentTime, float period){
    float x = (currentTime * M_PI) / period;
    float cotX = cos(x)/sin(x);
    float sawToothWaveValue = -1 * (2/M_PI) * atan( cotX );
    return sawToothWaveValue;
}


/*
 @return a pulse wave function depending on @param sampleNum, @param sampleRate, @param frequency, @param pf (fraction the wave is up) and @param maxVolume
 */
float SoundFileBuilder::getPulseWaveValue(float currentTime, int sampleNum, int sampleRate, float frequency, float pf, float maxVolume){
    if ( (sampleNum/sampleRate)*frequency - floor((1/sampleRate) - frequency) < pf ) {
        return maxVolume;
    } else {
        return -1 * maxVolume;
    }
}


/*
    @param currentTime the current x value we are generating amplitude value to multiply by separately calculated y value for.
    Other params represent values from the ADSR envelope.
    @return a float value to scale the function by, depending on the current stage in the a d s r envelope.
 */
float SoundFileBuilder::getAmplitudeValue(float currentTime, float attackTime, float decayTime, float sustainTime, float releaseTime, float totalSoundLength){
    float toReturn;
    if (currentTime <=  attackTime) {
        toReturn = (currentTime / attackTime);
    }else if (currentTime <= attackTime + decayTime){
        toReturn = 1 - ((currentTime - attackTime) / decayTime);
    }else if (currentTime <= attackTime + decayTime + sustainTime){ //decayseconds will be a value UP TO end of decay
        toReturn = this->lastAmplitudeValue;
    }else{ // we are in the release phase
        toReturn = this->lastAmplitudeValue * ((currentTime-sustainTime)/totalSoundLength);
    }
    this->lastAmplitudeValue = toReturn;
    return toReturn;
}
//
///*
// @return an amplitude value building up to peak volume depending on @param currenTime.
// ex: if currentTime is the same time where peak volume should occur, then the returned result is 1.
// */
//float SoundFileBuilder::getAttackAmplitudeValue(float currentTime, float attackTime){
//        float attackAmplitudeValue = peakvelocity * (currentTime/attackTime);
//        return attackAmplitudeValue;
//    }
//}
//
///*
// @return an amplitude value building down from peakVolumte depending on @param currenTime.
// ex: if currentTime is the same time where peakVolume should occur, then the returned result is peakVolume.
// */
//float SoundFileBuilder::getDecayAmplitudeValue(float currentTime){
//    float decayAmplitudeValue = this->getAttackAmplitudeValue(stof(switchArgumentMap["-a"]) - currentTime);
//    return decayAmplitudeValue;
//}
//
///*
// @return an amplitude value building down from sustatining depending on @param currenTime.
// ex: if currentTime is the same time where the sound should end, then the returned result is 0.
// */
//float SoundFileBuilder::getReleaseAmplitudeValue(float currentTime){
//    float releaseAmplitudeValue = lastSampleValue * (1 - (currentTime/stof(switchArgumentMap["-t"])));
//    return releaseAmplitudeValue;
//}
//
//
//
///*
// Recursive version of #SoundFileBuilder::getPulseWave
// @return a pulse wave function depending on @param currentTime and @param sampleNum
// */
//float SoundFileBuilder::getPulseWaveRecurisive(float currentTime, int sampleNum){
//    if (sampleNum == 0) {
//        return 0;
//    }else {
//        return ((2/(sampleNum * M_PI)) * sin( (M_PI * sampleNum * stof(switchArgumentMap["--pf"]))/(1/stof(switchArgumentMap["-f"]))) *
//                cos( ((2 * M_PI * stof(switchArgumentMap["--pf"])) / (1/stof(switchArgumentMap["-f"]))) * currentTime) ) +
//        this->getPulseWaveRecurisive(currentTime, sampleNum - 1);
//    }
//}
