#ifndef SoundFileBuilder_hpp
#define SoundFileBuilder_hpp
#include "all.h"
#include "SoundFile.hpp"
using namespace std;

/*
 This class is responsible for creating SoundFile and WavFile objects for various cases.
 */
class SoundFileBuilder {
    private:
        bool isCS229Heading(string line);
        bool shouldIgnoreLine(vector<string> lineVector);
        bool areSoundFileDataValuesInitialzed(int bitRes, int numChannels, int sampleRate);
        bool addStartDataToSoundFile(SoundFile** soundFile, istream& input, unsigned int multiplyValue);
    public:
        SoundFileBuilder();
        SoundFile* buildSoundFileFromInput(string input = "", unsigned int multiplyValue = 1);
        bool isValidFileType(string fileName, bool printOutput=true);
};

#endif /* SoundFileBuilder_hpp */
