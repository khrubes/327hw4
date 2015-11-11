#ifndef SoundFileBuilder_hpp
#define SoundFileBuilder_hpp
#include "all.h"
#include "SoundFile.hpp"
using namespace std;

/*
 This class is responsible for creating SoundFile objects for various cases.
 */
class SoundFileBuilder {
    protected:
        bool isCS229Heading(string line);
        bool shouldIgnoreLine(vector<string> lineVector);
        bool areSoundFileDataValuesInitialzed(int bitRes, int numChannels, int sampleRate);
        bool addStartDataToSoundFile(SoundFile** soundFile, ifstream& file);
    public:
        SoundFileBuilder();
        SoundFile* buildSoundFileFromFileName(string fileName);
        bool isValidFileType(string fileName, bool printOutput=true);
};

#endif /* SoundFileBuilder_hpp */
