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
        int bitRes;
        int numChannels;
        int numSamples;
        int sampleRate;
        string fileName;

    public:
        SoundFile(string fileName, int bitRes, int numChannels, int sampleRate, int numSamples);
        ~SoundFile();
        vector< vector<signed int> > channels;
        void setNumSamples(int numSamples);
};


#endif /* SoundFile_hpp */
