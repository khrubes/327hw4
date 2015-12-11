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
    this->calculateSustainTime();
}

void SndGen::setWaveFormType(vector<string>* arguments){
    for (int i=0; i<arguments->size(); i++) {
        if (isWaveFormSwitch( (*arguments)[i] ) ) {
            this->waveFormType = (*arguments)[i];
        }
    }
}

/*
    Calculates the length in seconds the sound file should be in the "sustain" phase, dependent on the length of the sound.
*/
void SndGen::calculateSustainTime(){
    float sustainLength = (stof(switchArgumentMap["-t"])) -  (stof(switchArgumentMap["-a"]) + stof(switchArgumentMap["-d"]) + stof(switchArgumentMap["-r"]));
    switchArgumentMap["-s"] = to_string(sustainLength);
}

/*
    @return true if the provided ASDR envelope is valid compared to the given length of the Sound.
*/
bool SndGen::isValidADSREnvelope(){
    if (stof(switchArgumentMap["-t"]) < (stof(switchArgumentMap["-a"]) + stof(switchArgumentMap["-d"]) + stof(switchArgumentMap["-s"]) + stof(switchArgumentMap["-r"]))) {
        return false;
    }
    return true;
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
    float pf = (this->switchArgumentMap.find("--pf"))==this->switchArgumentMap.end() ? 0 : stof(switchArgumentMap["-pf"]);
    SoundFile* generated = this->soundFileBuilder->buildSoundFileFromADSREvelope(stof(switchArgumentMap["-a"]), stof(switchArgumentMap["-d"]), stof(switchArgumentMap["-s"]), stof(switchArgumentMap["-r"]), stof(switchArgumentMap["-t"]), stof(switchArgumentMap["-f"]), stof(switchArgumentMap["-v"]), pf, stoi(switchArgumentMap["--bits"]), stoi(switchArgumentMap["--sr"]), this->waveFormType);
    this->outputSoundFile(generated);
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
            fprintf(stderr, "No percent up value (--pf) specified for Pulse wave.\n");
        return false;
    }
    
    if (stof(switchArgumentMap["--sr"]) == 0) {
        fprintf(stderr, "0 is probably an invalid sample rate.\n");
        return false;
    }
    
    if (stof(switchArgumentMap["-v"]) > 1 ||  stof(switchArgumentMap["-v"]) < 0) {
        fprintf(stderr, "Invalid value for -v switch: %f \n", stof(switchArgumentMap["-v"]));
        return false;
    }
    
    if (!this->isValidADSREnvelope()) {
        fprintf(stderr, "Invalid ADSR evelope, provided time less than calculated duration from ADSR.\n");
        return false;
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

