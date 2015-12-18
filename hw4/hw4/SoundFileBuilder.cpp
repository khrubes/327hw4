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
 Applies amplitudes to the sound wave depending on the current stage in the ADSR envelope, @param maxAmplitude, and @param bitdepth.
 This function assumes all input parameters are valid, the calling program is responsible for checking.
 @return a SoundFile built from the provided ADSR envelope.
*/
SoundFile* SoundFileBuilder::buildSoundFileFromADSREvelope(float attackTime, float decayTime, float sustainValue, float releaseTime, float totalSoundLength, float frequency, float maxAmplitude, float pf, int bitdepth, int sampleRate, string waveForm) {
    
    int numSamples = floor( totalSoundLength * sampleRate );
    SoundFile* soundFile = new SoundFile("" /* file name */, bitdepth, 1 /* num channels */, sampleRate, numSamples);
    vector<long> channel;
    channel.reserve(numSamples);
    
    for (int i = 0; i < numSamples; i++) {
        float currentTime = this->getXValue(i, numSamples, totalSoundLength);
        float sampleValue = this->getSampleValue(waveForm, currentTime, frequency, i, sampleRate, pf, maxAmplitude);
        float amplitude = this->getAmplitudeValue(maxAmplitude, currentTime, attackTime, decayTime, sustainValue, releaseTime, totalSoundLength);
        sampleValue*=amplitude;
        float bitdepthVal = pow(2, bitdepth-1) - 1;
        sampleValue*=bitdepthVal;        
        channel.push_back(sampleValue);
    }
    soundFile->setChannels(vector< vector<long> >(1, channel));
    return soundFile;
}

/*
 @return an x value data point determined by @param sampleNum, and the length of the sound.
*/
float SoundFileBuilder::getXValue(int sampleNum, int totalNumSamples, float lengthOfSound){
    float result = (lengthOfSound / (totalNumSamples)) * (sampleNum);
    return result;
}

/*
 @return a sample value for the corresponding @param currentTime and the current sample number we are on @param sampleNum
 */
float SoundFileBuilder::getSampleValue(string waveFormType, float currentTime, float frequency, int sampleNum, int sampleRate, float pf, float maxVolume){
    float toReturn;
    if (waveFormType.compare("--sin")==0) {
        toReturn = this->getSinWaveValue(currentTime, frequency);
    } else if(waveFormType.compare("--triangle")==0){
        toReturn = this->getTriangleWaveValue(currentTime, frequency);
    }else if(waveFormType.compare("--sawtooth")==0){
        toReturn = this->getSawtoothWaveValue(currentTime, frequency);
    }else if(waveFormType.compare("--pulse")==0){
        toReturn = this->getPulseWaveValue(currentTime, sampleNum, sampleRate, frequency, pf, maxVolume);
    }
    return toReturn;
}

/*
 @return a sin wave function depending on @param currentTime and @param period
 */
float SoundFileBuilder::getSinWaveValue(float currentTime, float period){
    float sinWaveValue = sin( period * currentTime * M_PI);
    return sinWaveValue;
}

/*
 @return a triangle wave function depending on @param currentTime and @param period
 Follows the formula for a triangle wave from https://en.wikipedia.org/wiki/Triangle_wave
 */
float SoundFileBuilder::getTriangleWaveValue(float currentTime, float period){
    float tOverA = (currentTime * (2.0 * period) );
    float floorVal = floor(tOverA + 0.5);
    float absVal = abs ( 2 * ( tOverA - floorVal ) );
    float triangleWaveValue = 2 * absVal -1;
    return triangleWaveValue;
}

/*
 @return a sawtooth wave function depending on @param currentTime and @param period.
 Follows the formula for a sawtooth wave from https://en.wikipedia.org/wiki/Sawtooth_wave with period p - (1/ "-f") and amplitude a given by getAmplitudeValue(currentTime)
 */
float SoundFileBuilder::getSawtoothWaveValue(float currentTime, float period){
    float tOverA = (currentTime * (2 * period));
    float sawToothWaveValue = 2 * (tOverA - floor(.5 + tOverA));
    return sawToothWaveValue;
}


/*
    @return a pulse wave function depending on @param sampleNum, @param sampleRate, @param frequency, @param pf (fraction the wave is up) and @param maxVolume
 */
float SoundFileBuilder::getPulseWaveValue(float currentTime, int sampleNum, int sampleRate, float frequency, float pf, float maxVolume){
    float fraction = (currentTime - frequency) - floor (currentTime - frequency );
    if ( fraction < (pf*frequency) ) {
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
float SoundFileBuilder::getAmplitudeValue(float peakAmplitude, float currentTime, float attackTime, float decayTime, float sustainValue, float releaseTime, float totalSoundLength){
    float toReturn;
    if (currentTime <=  attackTime) {
        //attack phase
        toReturn = this->getAmplitudeAtPoint(currentTime, attackTime, peakAmplitude, 0, 0); // creates a function through (0,0) (attackTime, peakAmplitude)
        
    }else if (currentTime <= attackTime + decayTime){
        //decay phase
        toReturn = this->getAmplitudeAtPoint(currentTime, attackTime + decayTime, sustainValue, attackTime, peakAmplitude);
        
    }else if (currentTime <=  ( totalSoundLength - releaseTime )){
        //sustain phase
        toReturn = this->lastAmplitudeValue;
        
    }else{
        // release phase
        toReturn = this->getAmplitudeAtPoint(currentTime, totalSoundLength, 0, totalSoundLength - releaseTime, sustainValue);
    }
    
    this->lastAmplitudeValue = toReturn;
    return toReturn;
}

/*
    Computes the amplitude at point @param currentX by creating a point through (x1, y1) and (x0, y0) and evaluating the function at currentX.
*/
float SoundFileBuilder::getAmplitudeAtPoint(float currentX, float x1, float y1, float x0, float y0){
    float slope = this->getSlope(y1, y0, x1, x0);
    float yIntercept = this->getYIntercept(slope, x1, y1);
    return (slope * currentX) +  yIntercept;
}


float SoundFileBuilder::getSlope(float y1, float y0, float x1, float x0) {
    return (y1 - y0) / (x1 - x0);
}

float SoundFileBuilder::getYIntercept(float slope, float xValue, float yValue) {
    return yValue - (slope * xValue);
}