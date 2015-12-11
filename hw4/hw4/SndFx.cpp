#include "SndFx.hpp"
SndFx::SndFx() : SndGen(){}

string SndFx::getProgramName(){
    return "SndFx";
}

string SndFx::getProgramDescription(){
    return "This program produces a sound with a combination of special effects, such as -backwards, -pitchup x, -pitchdown x, -fadeinout, and -echo numtimes.";
}

void SndFx::runProgram(vector<string> fileArguments){
    this->initSwitchArgumentMap(&fileArguments);
    this->initSwitchFunctionMap();
    this->initSoundFiles(fileArguments);
    runSwitches();
    
    for (auto& soundfile : this->soundFiles) {
        this->outputSoundFile(soundfile);
    }
}

/*
 Calls SoundProgram::initSwitchArgumentMap, and then removes all found switches from #requiredArgumentsVector.
 @param arguments the arguments passed in from the command line.
 */
void SndFx::initSwitchArgumentMap(vector<string>* arguments){
    SoundProgram::initSwitchArgumentMap(arguments);
}


/*
 @return true if @param switchArg and it's corresponding @param paramValue are a valid pair.
 For the case of SndGen, all switches with parameters have numerical argument values, so we include the check that this is a numerical value.
 */
bool SndFx::isValidSwitchArgumentPair(string switchArg, string paramValue){
    return SndGen::isValidSwitchArgumentPair(switchArg, paramValue);
}

/*
 @param bool @withParams if the vector of switches to return are the type that contain parameters.
 @return a vector of strings representing valid switches for this program. (ex. "-h")
 */
vector<string> SndFx::getValidSwitches(bool withParams){
    vector<string> switches = SoundProgram::getValidSwitches(withParams);
    if (!withParams) {
        switches.push_back("-backwards");
        switches.push_back("-fadeinout");
    }else {
        switches.push_back("-pitchup");
        switches.push_back("-pitchdown");
        switches.push_back("-echo");
    }
    return switches;
}


/*
    @param switchArg, the switch to return a description of.
    @return a description of what the switch referred to by @switchArg does
 */
string SndFx::getSwitchDescription(string switchArg){
    string description = SoundProgram::getSwitchDescription(switchArg);
    if (description.compare("")==0) {
        if (switchArg.compare("-backwards")==0) {
            description = "Reverses the given SoundFile.";
        } else if (switchArg.compare("-fadeinout")==0) {
            description = "Fades the volume of the given SoundFile in and out.";
        } else if (switchArg.compare("-pitchup")==0) {
            description = "Brings the pitch of the SoundFile up by a factor x, x>1";
        } else if (switchArg.compare("-pitchdown")==0) {
            description = "Brings the pitch of the SoundFile down by a factor x, x<1";
        } else if (switchArg.compare("-echo")==0) {
            description = "Repeats the SoundFile numtimes, fading in and out each time.";
        }
    }
    return description;
}

/*
 @param switchArg the switch which determines which function to return
 @return a SwitchFunction, a pointer to a function which implements the given switch
 */
SoundProgram::SwitchFunction SndFx::getSwitchFunction(string switchArg){
    if (SoundProgram::getSwitchFunction(switchArg)) {
        return SoundProgram::getSwitchFunction(switchArg);
    }
    if (switchArg.compare("-backwards")==0) {
        return (SoundProgram::SwitchFunction)&SndFx::backwardsSwitch;
    }else if (switchArg.compare("-fadeinout")==0) {
        return (SoundProgram::SwitchFunction)&SndFx::fadeinoutSwitch;
    }else if (switchArg.compare("-pitchup")==0) {
        return (SoundProgram::SwitchFunction)&SndFx::pitchupSwitch;
    }else if (switchArg.compare("-pitchdown")==0) {
        return (SoundProgram::SwitchFunction)&SndFx::pitchdownSwitch;
    }else if (switchArg.compare("-echo")==0) {
        return (SoundProgram::SwitchFunction)&SndFx::echoSwitch;
    }
    return NULL;
}



/*
   Reverses the given SoundFile.
   @return true if the reverse operation was performed successfully
 */
bool SndFx::backwardsSwitch(string argument = NULL){
    for (auto& soundFile : this->soundFiles) {
        for (auto& channel : *soundFile->getChannels()) {
            reverse(channel.begin(),channel.end());
        }
    }
    return true;
}

/*
    Fades the volume of the given SoundFile in and out.
    @return true if the fadeinout operation was performed successfully
 */
bool SndFx::fadeinoutSwitch(string argument = NULL){
    for (auto& soundFile : this->soundFiles) {
        for (auto& channel : *(*soundFile).getChannels()) {
            
            for (int sampleIndex = 0; sampleIndex< channel.size(); sampleIndex++) {
                
                float amplitudeValue = getAmplitudeValue(sampleIndex, channel);
                float fadedValue =  (float)channel  [sampleIndex] * amplitudeValue;
                channel[sampleIndex] = fadedValue;
            }
        }
    }
    return true;
}

/*
    Brings the pitch of the SoundFile up
    @param argument a string representation of the factor to bring the pitch up by @param argument
    @return true of @param argument is valid and the pitchup operation was performed successfully
 */
bool SndFx::pitchupSwitch(string argument){
    for (auto& soundFile : this->soundFiles) {
        try {
            (*soundFile) = (*soundFile) + stoi(argument);
        }
        catch(...){
            fprintf(stderr, "Only int values allowed for pitchup.\n");
            return false;
        }
    }
    return true;
}

/*
    Brings the pitch of the SoundFile down by @param argument
 */
bool SndFx::pitchdownSwitch(string argument){
    for (auto &soundFile : this->soundFiles) {
        try {
            (*soundFile) =  (*soundFile) + (-1 * stoi(argument));
        }
        catch(...){
            fprintf(stderr, "Only int values allowed for pitchup.\n");
            return false;
        }
    }
    return true;
}

/*
    Repeats the SoundFile numtimes.
 */
bool SndFx::echoSwitch(string argument){
    try {
        int numTimesToEcho = stoi(argument);
        for (auto& soundFile : this->soundFiles) {
            int numSamplesInCurrentSoundFileChannel = (*(soundFile->getChannels()))[0].size();
            int numSamplesForEchoSoundFile = numSamplesInCurrentSoundFileChannel * numTimesToEcho;
            this->concantenateSoundFiles(soundFile, vector<SoundFile*>(numTimesToEcho, soundFile), numSamplesForEchoSoundFile);
        }
    } catch (...) {
        fprintf(stderr, "Invalid value for echo: %s, only integer values are allowed.", argument.c_str());
    }
    return true;
}

/*
 @param currentIndex the current x value we are generating amplitude value to multiply by separately calculated y value for.
 @param channel to produce the fadeinout effect on. The sound scales up to the correct amplitude about halfway, and then back down,
    essentially turning the sound into one large triangle wave.
 @return a float value to scale the function by.
 */
float SndFx::getAmplitudeValue(float currentIndex, vector<signed int> channel){
    if (currentIndex <= ( channel.size()/2 )) {
        return (currentIndex / channel.size());
    } else {
        return 1 - (currentIndex / channel.size());
    }
}

