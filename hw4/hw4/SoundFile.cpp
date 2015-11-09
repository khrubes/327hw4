#include "SoundFile.hpp"

/*
    Constructor for a SoundFile. Assumes all input params are valid.
    Instantiates channels with numChannels # of vectors to hold chanel data.
 */
SoundFile::SoundFile(string fileName, int bitRes, int numChannels, int sampleRate, int numSamples){
    this->fileName = fileName;
    this->bitRes = bitRes;
    this->numChannels = numChannels;
    this->numSamples = numSamples;
    this->sampleRate = sampleRate;
    this->channels.reserve(numChannels);
    
    for (int i=0; i < numChannels; i++) {
        vector<signed int> *channel = new vector<signed int>();
        this->channels.push_back(*channel);
    }
}

SoundFile::~SoundFile(){
    cout << "soundfile destructor, you probably have a shit ton of memory leaks" << endl;
}

void SoundFile::setNumSamples(int numSamples){
    this->numSamples = numSamples;
}
