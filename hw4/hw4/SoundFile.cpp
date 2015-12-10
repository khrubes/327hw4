#include "SoundFile.hpp"

SoundFile::SoundFile(){
}
/*
    Constructor for a SoundFile. Assumes all input params are valid.
    Instantiates channels with numChannels # of vectors to hold chanel data.
 */
SoundFile::SoundFile(string fileName, int bitRes, int numChannels, int sampleRate, int numSamples){
    if(fileName.compare("")==0){
        fileName = "SoundFile read from stdin";
    }
    this->fileName = fileName;
    this->bitDepth = bitRes;
    this->numChannels = numChannels;
    this->numSamples = numSamples;
    this->sampleRate = sampleRate;
    this->channels.reserve(numChannels);
    
    for (int i=0; i < numChannels; i++) {
        vector<signed int> *channel = new vector<signed int>();
        this->channels.push_back(*channel);
    }
}

/*  
    Iterates over the sample data in #channels and multiplies it by multi.
    @param multi the value to multiply sample data by. Must be a real value between -10 and 10
    @return a modified reference to the current #SoundFile, or an unmodified one if multi is invalid.
 
*/
SoundFile SoundFile::operator*(int multi){
    if (multi < -10 || multi > 10) {
        fprintf(stderr, "Invalid multiplying value \"%i\" for file %s.", multi, this->getFileName().c_str());
        return *this;
    }
    for(auto& channel : this->channels){
        for (int i = 0; i < channel.size(); i++){
            channel[i]*= multi;
        }
    }
    return *this;
}

/*
 Iterates over the sample data in #channels and adds it by plus.
 @param plus the value to add to the sample data. Must not add over the 2^getBitDepth
 @return a modified reference to the current #SoundFile, or an unmodified one if plus is invalid.
*/
SoundFile SoundFile::operator+(int plus){
    for(auto& channel : this->channels){
        for (int i = 0; i < channel.size(); i++){
            if (abs(channel[i]+plus) <= pow(2, this->getBitDepth())) {
                channel[i]+= plus;
            }
        }
    }
    return *this;
}

/*
    Prints the contents of this SoundFile to @param output.
    Assumes that is output is a file, it has already been opened and will be closed after calling this function.
*/
void SoundFile::print(ostream& output){
    output << "CS229" << endl;
    output << "Samples \t" << (this->getNumSamples() / this->getNumChannels()) << endl;
    output << "Channels \t" << this->getNumChannels() << endl;
    output << "BitRes \t" << this->getBitDepth() << endl;
    output << "SampleRate \t" << this->getSampleRate() << endl;
    output << "StartData" << endl;
  
    for(int row=0; row < ((*(this->getChannels()))[0]).size(); row++) {
        for (int channelNumber=0; channelNumber<this->getNumChannels(); channelNumber++) {
            output << ((*(this->getChannels()))[channelNumber])[row] << " ";
        }
        output << endl;
    }
    output <<  "<EOF>\n" << endl;
}

void SoundFile::setNumSamples(int numSamples){
    this->numSamples = numSamples;
}

void SoundFile::setBitDepth(int bitdepth){
    this->bitDepth = bitdepth;

}
void SoundFile::setNumChannels(int numChannels){
    this->numChannels = numChannels;
}

void SoundFile::setSampleRate(int sampleRate){
    this->sampleRate = sampleRate;
}

void SoundFile::setChannels(vector< vector<signed int> > channels){
    this->channels = channels;
}

const int SoundFile::getBitDepth(){
    return this->bitDepth;
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


vector< vector<signed int> >* SoundFile::getChannels(){
    return &this->channels;
}