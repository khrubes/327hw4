#include "SndGen.hpp"
#include <cmath> //TODO delete all.h?

/*
    Calls SoundProgram::initSwitchArgumentMap, and then removes all found switches from #requiredArgumentsVector.
    @param arguments the arguments passed in from the command line.
 */
void SndGen::initSwitchArgumentMap(vector<string>* arguments){
    SoundProgram::initSwitchArgumentMap(arguments);
    for (int i=0; i<arguments->size(); i++) {
        if (isWaveFormSwitch( (*arguments)[i] ) ) {
            this->waveFormType = (*arguments)[i];
        }
    }
    for(auto iterator = this->switchArgumentMap.begin(); iterator != switchArgumentMap.end(); iterator++) {
        if( find(requiredArgumentsVector.begin(), requiredArgumentsVector.end(), iterator->first) != requiredArgumentsVector.end() ) {
            requiredArgumentsVector.erase(find(requiredArgumentsVector.begin(), requiredArgumentsVector.end(), iterator->first));
        }
    }
}

/*
    @return an x value data point determined by the sample rate, @param iterationNum, and the length of the sound.
*/
long SndGen::getXValue(int iterationNum){
//TODO check for sampleRate = 0;
    return iterationNum * (stol(switchArgumentMap["-t"]) / stol(switchArgumentMap["--sr"]));
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
    this->initSwitchArgumentMap(&fileArguments);
    
    if (!hasValidInputsToRunProgram()) {
        exit(0);
    }
    
    this->initSwitchFunctionMap();
    this->initSoundFiles(fileArguments);
    runSwitches();
    
    int numSamples = floor( stol(switchArgumentMap["-t"]) * stol(switchArgumentMap["--sr"]) );
    SoundFile* generatedSoundFile = new SoundFile(this->outPutFileName, stol(switchArgumentMap["--bits"]), 1, stol(switchArgumentMap["--sr"]), numSamples);
    vector<signed int> channel = (*(generatedSoundFile->getChannels()))[0];
    for (int i=0; i < numSamples; i++) {
        channel.push_back(getSampleValue(getXValue(i), i ));
    }
}

/*
    @param currentTime the current x value we are generating amplitude value to multiply by separately calculated y value for.
    @return a long value to scale the function by, depending on the current stage in the a d s r envelope.
*/
long SndGen::getAmplitudeValue(long currentTime){
    if (currentTime <= stol(switchArgumentMap["-a"])) {
        return this->getAttackAmplitudeValue(currentTime);
    }else if (currentTime <= (stol(switchArgumentMap["-a"]) + stol(switchArgumentMap["-d"]))){
        return this->getDecayAmplitudeValue(currentTime);
    }else if (currentTime <= (stol(switchArgumentMap["-a"]) + stol(switchArgumentMap["-d"]) + stol(switchArgumentMap["-s"]))){ //decayseconds will be a value UP TO end of decay
        return this->lastSampleValue;
    }else{ // we are in the release phase
        if (stol(switchArgumentMap["-t"]) < (stol(switchArgumentMap["-a"]) + stol(switchArgumentMap["-d"]) + stol(switchArgumentMap["-s"]) + stol(switchArgumentMap["-r"]))) { //todo swap out for getting key from map
            return 0; //No sound should be produced if the duration is less than the release time.
        }
        return this->getDecayAmplitudeValue(currentTime);
    }
}

//TODO what to do with peakvolume as a percentage
/*
    @return an amplitude value building up to peakVolumte depending on @param currenTime.
    ex: if currentTime is the same time where peakVolume should occur, then the returned result is peakVolume.
*/
long SndGen::getAttackAmplitudeValue(long currentTime){
    if (currentTime == 0) {
        return 1;
    } else {
        return stol(switchArgumentMap["-v"]) * (currentTime/stol(switchArgumentMap["-a"]));
    }
}

/*
    @return an amplitude value building down from peakVolumte depending on @param currenTime.
    ex: if currentTime is the same time where peakVolume should occur, then the returned result is peakVolume.
 */
long SndGen::getDecayAmplitudeValue(long currentTime){
    return this->getAttackAmplitudeValue(stol(switchArgumentMap["-a"]) - currentTime);
}

/*
    @return an amplitude value building down from sustatining depending on @param currenTime.
    ex: if currentTime is the same time where the sound should end, then the returned result is 0.
*/
long SndGen::getReleaseAmplitudeValue(long currentTime){
    return lastSampleValue * (1 - (currentTime/stol(switchArgumentMap["-t"])));
}

long SndGen::getSampleValue(long currentTime, int iterationNum){
    long toReturn;
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
long SndGen::getSinWaveValue(long currentTime, int iterationNum){
    return this->getAmplitudeValue(currentTime) * sin( stol(switchArgumentMap["-f"]) * this->getXValue(iterationNum));
}

/*
    @return a triangle wave function depending on @param currentTime and @param iterationNum
 */
long SndGen::getTriangleWaveValue(long currentTime, int iterationNum){
    long xValue = this->getXValue(iterationNum);
    if (xValue == 0) {
        return 0;
    }else{
        return ((2 * this->getAmplitudeValue(currentTime)) / M_PI) * asin( sin( ((2*M_PI)/stol(switchArgumentMap["-f"])) * this->getXValue(iterationNum)));
    }
}

/*
    @return a sawtooth wave function depending on @param currentTime and @param iterationNum
 */
long SndGen::getSawtoothWaveValue(long currentTime, int iterationNum){
    return -1 * ((2 * this->getAmplitudeValue(currentTime))/M_PI) * atan( 1/(tan( (this->getXValue(iterationNum) * M_PI) / stol(switchArgumentMap["-f"]) )));
}

/*
    @return a pulse wave function depending on @param currentTime and @param iterationNum
 */
long SndGen::getPulseWaveValue(long currentTime, int iterationNum){
    return (stol(switchArgumentMap["--pf"])/stol(switchArgumentMap["-f"])) + this->getPulseWaveRecurisive(currentTime, iterationNum);
}

/*  
    Recursive version of #SndGen::getPulseWave
    @return a pulse wave function depending on @param currentTime and @param iterationNum
 */
long SndGen::getPulseWaveRecurisive(long currentTime, int iterationNum){
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
        switches.push_back("--sine");
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
    }else if (this->waveFormType.compare("pulse")==0 && ( (this->switchArgumentMap.find("--pf"))==this->switchArgumentMap.end())){
            fprintf(stderr, "No percent up value (--pf) specified for Pulse waves require a ");
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
    char * p ;
    strtol(paramValue.c_str(), &p, 10) ;
    return (*p == 0) ;
}

/*
    @return true is @param argument is "--sin" "--sawtooth" "--triangle" or "pulse"
*/
bool SndGen::isWaveFormSwitch(string argument){
    return (argument.compare("--sin")==0 || argument.compare("--sawtooth")==0 || argument.compare("--triangle")==0 || argument.compare("--pulse")==0);
}

