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

const int SoundFile::getBitRes(){
    return this->bitRes;
}

const int SoundFile::getNumChannels(){
    return this->numChannels;
}

const int SoundFile::getNumSamples(){
    return this->numSamples;
}

const int SoundFile::getSampleRate(){
    return this->sampleRate;
}

const string SoundFile::getFileName(){
    return this->fileName;
}

const string SoundFile::getFileType(){
    string dot = ".";
    size_t index = this->fileName.find(dot);
    if (index==string::npos){
        fprintf(stderr, "Error: SoundFile fileName is not a valid file type.");
        return NULL;
    }
    return fileName.substr(index, this->fileName.length() );
}
