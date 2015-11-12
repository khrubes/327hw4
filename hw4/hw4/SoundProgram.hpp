#ifndef SoundProgram_hpp
#define SoundProgram_hpp
#include "all.h"
#include "SoundFile.hpp"
#include "SoundFileBuilder.hpp"
using namespace std;
/*
    This class is an abstract representation for a Sound Program
*/
class SoundProgram {
    private:
        bool isValidSwitch(string switchArg, bool withParams);
        void initSwitchArgumentMap(vector<string>* arguments);
        void initSwitchFunctionMap();
    
    protected:
        virtual void runProgram() = 0;
        virtual string getProgramName() = 0;
        virtual string getProgramDescription() = 0;
        void runSwitches();
    
        vector<SoundFile*> soundFiles;
        string outPutFileName;
        void initSoundFiles(vector<string> arguments); //sndgen will need to handle logic for multi arguments, make virtual eventually
        /*
            A SwitchFunction is pointer to a function which implements the functionality of a given switch.
            @param argument the argument passed in with the switch
            @return false if there was an error performing the SwitchFunction
         */
        typedef bool (SoundProgram::*SwitchFunction)(string argument);
        SwitchFunction getSwitchFunction(string switchArg);
    
        /* Switch Functions*/
        bool hSwitch(string argument);
        bool oSwitch(string argument);
        vector<string> getValidSwitches(bool withParams);
        unordered_map<string, SwitchFunction> switchFunctionMap;
        unordered_map<string, string> switchArgumentMap;
    
    public:
        SoundProgram();
        SoundProgram(vector<string> arguments);
        void setOutputFileName(string fileName);
        SoundFileBuilder* soundFileBuilder;
        string getSwitchDescription(string switchName);
};

#endif /* SoundProgram_hpp */
