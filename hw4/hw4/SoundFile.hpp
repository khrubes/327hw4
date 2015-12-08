#ifndef SoundFile_hpp
#define SoundFile_hpp
#include "all.h"
using namespace std;

/*
    This class represents a SoundFile object, which contains data given by a .cs229 file.
    SoundFiles are created by createSoundFile.
*/

class SoundFile {
    private:
        int bitDepth;
        int numChannels;
        int numSamples;
        int sampleRate;
        string fileName;
        vector< vector<signed int> > channels;
    public:
        SoundFile(string fileName, int bitRes, int numChannels, int sampleRate, int numSamples);
        SoundFile();
        ~SoundFile();
        SoundFile* operator*(int multi);
        SoundFile* operator+(int plus);
    
        void setNumSamples(int numSamples);
        void setBitDepth(int bitdepth);
        void setNumChannels(int numChannels);
        void setSampleRate(int sampleRate);
        void setChannels(vector< vector<signed int> > channels);
    
        const int getBitDepth();
        const int getNumChannels();
        const int getNumSamples();
        const int getSampleRate();
        vector< vector<signed int> >* getChannels();
        const string getFileName();
        virtual const string getFileType();
        virtual void print(ostream& ouput);
};

#endif /* SoundFile_hpp */
