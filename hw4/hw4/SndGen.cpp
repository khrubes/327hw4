#include "SndGen.hpp"
#include "util.hpp"
#include <cmath>
#include "stdio.h"

/*
    Calls SoundProgram::initSwitchArgumentMap, and then removes all found switches from #requiredArgumentsVector.
    @param arguments the arguments passed in from the command line.
 */
void SndGen::initSwitchArgumentMap(vector<string>* arguments){
    this->setWaveFormType(arguments);
    SoundProgram::initSwitchArgumentMap(arguments);
    for(auto iterator = this->switchArgumentMap.begin(); iterator != switchArgumentMap.end(); iterator++) {
        if( find(requiredArgumentsVector.begin(), requiredArgumentsVector.end(), iterator->first) != requiredArgumentsVector.end() ) {
            requiredArgumentsVector.erase(find(requiredArgumentsVector.begin(), requiredArgumentsVector.end(), iterator->first));
        }
    }
}

void SndGen::setWaveFormType(vector<string>* arguments){
    for (int i=0; i<arguments->size(); i++) {
        if (isWaveFormSwitch( (*arguments)[i] ) ) {
            this->waveFormType = (*arguments)[i];
        }
    }
}

/*
    @return an x value data point determined by @param iterationNum, and the length of the sound.
*/
float SndGen::getXValue(int iterationNum){
//TODO check for sampleRate = 0;
    float lengthOfSound = stol(switchArgumentMap["-t"]);
    float result = (float)(((float)iterationNum) / lengthOfSound);
    return result;
}

/*
    Calculates the length in seconds the sound file should be in the "sustain" phase, dependent on the length of the sound.
*/
void SndGen::calculateSustainVolume(){
    switchArgumentMap["-s"] = stol(switchArgumentMap["-s"]) * stol(switchArgumentMap["-t"]);
}

SndGen::SndGen() : SoundProgram(){}

string SndGen::getProgramName(){
    return "SndGen";
}

string SndGen::getProgramDescription(){
    return "This program produces a sound of a specified frequency and waveform using a simple ADSR envelope.";
}

/*
    This program produces a sound of a specified frequency and waveform using a simple ADSR envelope.
 */
void SndGen::runProgram(vector<string> fileArguments){
    this->initRequiredArgumentsVector();
    this->initSwitchArgumentMap(&fileArguments);
    
    if (!hasValidInputsToRunProgram()) {
        exit(0);
    }
    
    this->initSwitchFunctionMap();
    runSwitches();
    
    int numSamples = floor( stol(switchArgumentMap["-t"]) * stol(switchArgumentMap["--sr"]) );
    SoundFile* generatedSoundFile = new SoundFile(this->outPutFileName, stol(switchArgumentMap["--bits"]), 1, stol(switchArgumentMap["--sr"]), numSamples);
    for (int i=0; i < numSamples; i++) {
        ((*(generatedSoundFile->getChannels()))[0]).push_back(getSampleValue(getXValue(i), i ));
    }
    this->outputSoundFile(generatedSoundFile);
}

/*
    @param currentTime the current x value we are generating amplitude value to multiply by separately calculated y value for.
    @return a float value to scale the function by, depending on the current stage in the a d s r envelope.
*/
float SndGen::getAmplitudeValue(float currentTime){
    if (currentTime <= stol(switchArgumentMap["-a"])) {
        return this->getAttackAmplitudeValue(currentTime);
    }else if (currentTime <= (stol(switchArgumentMap["-a"]) + stol(switchArgumentMap["-d"]))){
        return this->getDecayAmplitudeValue(currentTime);
    }else if (currentTime <= (stol(switchArgumentMap["-a"]) + stol(switchArgumentMap["-d"]) + stol(switchArgumentMap["-s"]))){ //decayseconds will be a value UP TO end of decay
        return this->lastSampleValue;
    }else{ // we are in the release phase
        if (stol(switchArgumentMap["-t"]) < (stol(switchArgumentMap["-a"]) + stol(switchArgumentMap["-d"]) + stol(switchArgumentMap["-s"]) + stol(switchArgumentMap["-r"]))) {
            return 0; //No sound should be produced if the duration is less than the release time.
        }
        return this->getDecayAmplitudeValue(currentTime);
    }
}


/*
    @return an amplitude value building up to peakVolumte depending on @param currenTime.
    ex: if currentTime is the same time where peakVolume should occur, then the returned result is peakVolume.
*/
float SndGen::getAttackAmplitudeValue(float currentTime){
    if (currentTime == 0) {
        return 1;
    } else {
        float peakvelocity = stol(switchArgumentMap["-v"]);
        float attackTime = stol(switchArgumentMap["-a"]);
        float attackAmplitudeValue = peakvelocity * (currentTime/attackTime);
        return attackAmplitudeValue;
    }
}

/*
    @return an amplitude value building down from peakVolumte depending on @param currenTime.
    ex: if currentTime is the same time where peakVolume should occur, then the returned result is peakVolume.
 */
float SndGen::getDecayAmplitudeValue(float currentTime){
    float decayAmplitudeValue = this->getAttackAmplitudeValue(stol(switchArgumentMap["-a"]) - currentTime);
    return decayAmplitudeValue;
}

/*
    @return an amplitude value building down from sustatining depending on @param currenTime.
    ex: if currentTime is the same time where the sound should end, then the returned result is 0.
*/
float SndGen::getReleaseAmplitudeValue(float currentTime){
    float releaseAmplitudeValue = lastSampleValue * (1 - (currentTime/stol(switchArgumentMap["-t"])));
    return releaseAmplitudeValue;
}

/*
    @return a sample value for the corresponding @param currentTime and the current sample number we are on @param iterationNum
*/
float SndGen::getSampleValue(float currentTime, int iterationNum){
    float toReturn;
    if (this->waveFormType.compare("--sin")==0) {
        return this->getSinWaveValue(currentTime, iterationNum);
    } else if(this->waveFormType.compare("--triangle")==0){
        return this->getTriangleWaveValue(currentTime, iterationNum);
    }else if(this->waveFormType.compare("--sawtooth")==0){
        return this->getSawtoothWaveValue(currentTime, iterationNum);
    }else if(this->waveFormType.compare("--pulse")==0){
        return this->getPulseWaveValue(currentTime, iterationNum);
    }
    lastSampleValue = toReturn;
    return toReturn;
}

/*
    @return a sin wave function depending on @param currentTime and @param iterationNum
*/
float SndGen::getSinWaveValue(float currentTime, int iterationNum){
    float frequency = stol(switchArgumentMap["-f"]);
    float sinWaveValue = this->getAmplitudeValue(currentTime) * sin( frequency * this->getXValue(iterationNum) );
    return sinWaveValue;
}

/*
    @return a triangle wave function depending on @param currentTime and @param iterationNum
 */
float SndGen::getTriangleWaveValue(float currentTime, int iterationNum){
    float xValue = this->getXValue(iterationNum);
    if (xValue == 0) {
        return 0;
    }else{
        float triangleWaveValue = ((2 * this->getAmplitudeValue(currentTime)) / M_PI) * asin( sin( ((2*M_PI)/stol(switchArgumentMap["-f"])) * this->getXValue(iterationNum)));
        return triangleWaveValue;
    }
}

/*
    @return a sawtooth wave function depending on @param currentTime and @param iterationNum
 */
float SndGen::getSawtoothWaveValue(float currentTime, int iterationNum){
    float sawToothWaveValue = -1 * ((2 * this->getAmplitudeValue(currentTime))/M_PI) * atan( 1/(tan( (this->getXValue(iterationNum) * M_PI) / stol(switchArgumentMap["-f"]) )));
    return sawToothWaveValue;
}

/*
    @return a pulse wave function depending on @param currentTime and @param iterationNum
 */
float SndGen::getPulseWaveValue(float currentTime, int iterationNum){
    float pulsewWaveValue = (stol(switchArgumentMap["--pf"])/stol(switchArgumentMap["-f"])) + this->getPulseWaveRecurisive(currentTime, iterationNum);
    return pulsewWaveValue;
}

/*  
    Recursive version of #SndGen::getPulseWave
    @return a pulse wave function depending on @param currentTime and @param iterationNum
 */
float SndGen::getPulseWaveRecurisive(float currentTime, int iterationNum){
    if (iterationNum == 0) {
        return 0;
    }else {
        return ((2/(iterationNum * M_PI)) * sin( (M_PI * iterationNum * stol(switchArgumentMap["--pf"]))/stol(switchArgumentMap["-f"])) *
        cos( ((2 * M_PI * stol(switchArgumentMap["--pf"])) / stol(switchArgumentMap["-f"])) * this->getXValue(iterationNum)) ) +
        this->getPulseWaveRecurisive(currentTime, iterationNum - 1);
    }
}


/*
 @param bool @withParams if the vector of switches to return are the type that contain parameters.
 @return a vector of strings representing valid switches for this program. (ex. "-h")
 */
vector<string> SndGen::getValidSwitches(bool withParams){
    vector<string> switches = SoundProgram::getValidSwitches(withParams);
    if (!withParams) {
        switches.push_back("-h");
        switches.push_back("--sin");
        switches.push_back("--triangle");
        switches.push_back("--sawtooth");
        switches.push_back("--pulse");
    }else {
        switches.push_back("-o");
        switches.push_back("-f");
        switches.push_back("-t");
        switches.push_back("-v");
        switches.push_back("-a");
        switches.push_back("-d");
        switches.push_back("-s");
        switches.push_back("-r");
        switches.push_back("--bits");
        switches.push_back("--sr");
        switches.push_back("--pf");
    }
    return switches;
}


/*
    @param switchArg, the switch to return a description of.
    @return a description of what the switch referred to by @switchArg does, to be used in #hSwitch
 */
string SndGen::getSwitchDescription(string switchArg){
    string description = SoundProgram::getSwitchDescription(switchArg);
    if (description.compare("")==0) {
        if (switchArg.compare("--bits")==0) {
            description = "n : use a bit depth of n";
        } else if (switchArg.compare("--sr")==0) {
            description = "n : use a sample rate of n";
        } else if (switchArg.compare("-f")==0) {
            description = "use a frequency of r Hz";
        } else if (switchArg.compare("-t")==0) {
            description = "r : total duration of r seconds";
        } else if (switchArg.compare("-v")==0) {
            description = "p : Peak volume. 0 ≤ p ≤ 1";
        } else if (switchArg.compare("-a")==0) {
            description = "r : attack time of r seconds";
        } else if (switchArg.compare("-d")==0) {
            description = "r : decay time of r seconds";
        } else if (switchArg.compare("-s")==0) {
            description = "p : sustain volume. 0 ≤ p ≤ 1";
        } else if (switchArg.compare("-r")==0) {
            description = "r : release time of r seconds";
        } else if (switchArg.compare("--sine")==0) {
            description = "generate a sine wave";
        } else if (switchArg.compare("--triangle")==0) {
            description = "generate a triangle wave";
        } else if (switchArg.compare("--sawtooth")==0) {
            description = "generate a sawtooth wave";
        } else if (switchArg.compare("--pulse")==0) {
            description = "generate a pulse wave";
        } else if (switchArg.compare("--pf")==0) {
            description = "Fraction of the time the pulse wave is “up”";
        }
    }
    return description;
}

/*
    Initializes #SndGen::requiredArgumentsVector with strings representing arguments required to run SndGen. 
    As arguments are processed from the input, their matching values will be removed from requiredArgumentsVector. 
    If there are remaining arguments, this means we are lacking enough information to run SndGen.
*/
void SndGen::initRequiredArgumentsVector(){
    this->requiredArgumentsVector.push_back("--bits");
    this->requiredArgumentsVector.push_back("--sr");
    this->requiredArgumentsVector.push_back("-a");
    this->requiredArgumentsVector.push_back("-d");
    this->requiredArgumentsVector.push_back("-s");
    this->requiredArgumentsVector.push_back("-r");
    this->requiredArgumentsVector.push_back("-t");
    this->requiredArgumentsVector.push_back("-f");
    this->requiredArgumentsVector.push_back("-v");
}

/*
    @return true if all required arguments are provided to SndGen and a waveform has been specified.
*/
bool SndGen::hasValidInputsToRunProgram(){
    if (this->requiredArgumentsVector.size()!=0){
        for (int i=0; i<this->requiredArgumentsVector.size(); i++) {
            fprintf(stderr, "Missing required argument for SndGen: %s\n" , (this->requiredArgumentsVector[i]).c_str());
        }
        return false;
    }
    
    if (this->waveFormType.compare("")==0) {
            fprintf(stderr, "No wave form type (sin, triangle, sawtooth, pulse) specified.\n");
        return false;
    }else if (this->waveFormType.compare("pulse")==0 && ( (this->switchArgumentMap.find("--pf"))==this->switchArgumentMap.end())){
            fprintf(stderr, "No percent up value (--pf) specified for Pulse waves require a ");
        return false;
    }
    
    if (stol(switchArgumentMap["-t"]) == 0) {
        fprintf(stderr, "0 is probably an invalid Sample Rate");
        exit(0);
    }
    
    return true;
}

/*
    @return true if @param switchArg and it's corresponding @param paramValue are a valid pair.
    For the case of SndGen, all switches with parameters have numerical argument values, so we include the check that this is a numerical value.
 */
bool SndGen::isValidSwitchArgumentPair(string switchArg, string paramValue){
    if (!SoundProgram::isValidSwitchArgumentPair(switchArg, paramValue)) {
        return false;
    }
    return switchArg.compare("-o")==0 || isNumeric(paramValue); //all other parameters for SndGen have numeric values aside from -o
}

/*
    @return true is @param argument is "--sin" "--sawtooth" "--triangle" or "pulse"
*/
bool SndGen::isWaveFormSwitch(string argument){
    return (argument.compare("--sin")==0 || argument.compare("--sawtooth")==0 || argument.compare("--triangle")==0 || argument.compare("--pulse")==0);
}

