#include "SndGen.hpp"
#include <cmath> //TODO delete all.h?

long SndGen::getXValue(int iterationNum){
//TODO check for sampleRate = 0;
    return iterationNum * (this->durationInSeconds / this->sampleRate);
}

long SndGen::calculateSustainVolume(long percentage){
    return percentage * this->durationInSeconds;
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
    this->initSwitchFunctionMap();
    this->initSoundFiles(fileArguments);
    runSwitches();
}

/*
    @param currentTime the current x value we are generating a y value for.
    @return a long value to scale the function by, depending on the current stage in the a d s r envelope.
*/
long SndGen::getAmplitudeValue(long currentTime){
    if (currentTime <= this->attackSeconds) {
        return this->getAttackAmplitudeValue(currentTime);
    }else if (currentTime <= (this->attackSeconds + this->decaySeconds)){
        return this->getDecayAmplitudeValue(currentTime);
    }else if (currentTime <= (this->attackSeconds + this->decaySeconds + this->sustainSeconds)){ //decayseconds will be a value UP TO end of decay
        return this->lastSampleValue;
    }else{ // we are in the release phase
        if (durationInSeconds < (this->attackSeconds + this->decaySeconds + this->sustainSeconds + this->releaseSeconds)) { //todo swap out for getting key from map
            return 0; //No sound should be produced if the duration is less than the release time.
        }
        return this->getDecayAmplitudeValue(currentTime);
    }
}

//TODO what to do with peakvolume as a percentage
long SndGen::getAttackAmplitudeValue(long currentTime){
    if (currentTime == 0) {
        return 1;
    } else {
        return peakVolume * (currentTime/attackSeconds);
    }
}

long SndGen::getDecayAmplitudeValue(long currentTime){
    return this->getAttackAmplitudeValue(attackSeconds - currentTime);
}

long SndGen::getReleaseAmplitudeValue(long currentTime){
    return lastSampleValue * (1 - (currentTime/this->durationInSeconds));
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

long SndGen::getSinWaveValue(long currentTime, int iterationNum){
    return this->getAmplitudeValue(currentTime) * sin( stol(switchArgumentMap["-f"]) * this->getXValue(iterationNum));
}

long SndGen::getTriangleWaveValue(long currentTime, int iterationNum){
    long xValue = this->getXValue(iterationNum);
    if (xValue == 0) {
        return 0;
    }else{
        return ((2 * this->getAmplitudeValue(currentTime)) / M_PI) * asin( sin( ((2*M_PI)/stol(switchArgumentMap["-f"])) * this->getXValue(iterationNum)));
    }
}
long SndGen::getSawtoothWaveValue(long currentTime, int iterationNum){
    return -1 * ((2 * this->getAmplitudeValue(currentTime))/M_PI) * atan( 1/(tan( (this->getXValue(iterationNum) * M_PI) / stol(switchArgumentMap["-f"]) )));
}

long SndGen::getPulseWaveValue(long currentTime, int iterationNum){
    return (stol(switchArgumentMap["--pf"])/stol(switchArgumentMap["-f"])) + this->getPulseWaveRecurisive(currentTime, iterationNum);
}

long SndGen::getPulseWaveRecurisive(long currentTime, int iterationNum){
    if (iterationNum == 0) {
        return 0;
    }else {
        return ((2/(iterationNum * M_PI)) * sin( (M_PI * iterationNum * stol(switchArgumentMap["--pf"]))/stol(switchArgumentMap["-f"])) *
        cos( ((2 * M_PI * stol(switchArgumentMap["--pf"])) / stol(switchArgumentMap["-f"])) * this->getXValue(iterationNum)) ) +
        this->getPulseWaveRecurisive(currentTime, iterationNum - 1);
    }
}







