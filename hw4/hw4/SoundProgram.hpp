#ifndef SoundProgram_hpp
#define SoundProgram_hpp
#include "SoundFile.hpp"
#include "SoundFileBuilder.hpp"
using namespace std;
/*
    This class is an abstract representation for a Sound Program
*/
class SoundProgram {
    protected:
        /*
         A SwitchFunction is pointer to a function which implements the functionality of a given switch.
         @param argument the string argument passed in with the switch
         @return false if there was an error performing the SwitchFunction
         */
        typedef bool (SoundProgram::*SwitchFunction)(string argument);
    
        /* Member Variables */
        unordered_map<string, SwitchFunction> switchFunctionMap;
        unordered_map<string, string> switchArgumentMap;
        vector<SoundFile*> soundFiles;
        string outPutFileName;
        SoundFileBuilder *soundFileBuilder;
        
        virtual string getProgramName() = 0;
        virtual string getProgramDescription() = 0;
        virtual bool isValidSwitchArgumentPair(string switchArg, string paramValue);
        virtual void initSwitchArgumentMap(vector<string>* arguments);
        virtual void initSoundFiles(vector<string> arguments);
        virtual void outputSoundFile(SoundFile* soundFile);
        void initSwitchFunctionMap();
        bool isValidSwitch(string switchArg, bool withParams);
        void concantenateSoundFiles(SoundFile* toConcantenateInto, vector<SoundFile*> soundFiles, int numSamplesForChannel);
    
        void runSwitches();

        virtual SwitchFunction getSwitchFunction(string switchArg);
    
        /* Switch Functions*/
        bool hSwitch(string argument);
        bool oSwitch(string argument);
        virtual vector<string> getValidSwitches(bool withParams);
    
    public:
        SoundProgram();
        void setOutputFileName(string fileName);
        virtual string getSwitchDescription(string switchName);
        virtual void runProgram(vector<string> fileArguments) = 0;
};

#endif /* SoundProgram_hpp */
