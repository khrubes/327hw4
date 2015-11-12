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
        /* Functions */
        virtual void runProgram() = 0;
        virtual string getProgramName() = 0;
        virtual string getProgramDescription() = 0;
        virtual void initSoundFiles(vector<string> arguments);
        void runSwitches();
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
    
        /* Member variables */
        unordered_map<string, SwitchFunction> switchFunctionMap;
        unordered_map<string, string> switchArgumentMap;
        vector<SoundFile*> soundFiles;
        string outPutFileName;
    
    public:
        SoundProgram();
        SoundProgram(vector<string> arguments);
        void setOutputFileName(string fileName);
        string getSwitchDescription(string switchName);
    
        SoundFileBuilder* soundFileBuilder;
};

#endif /* SoundProgram_hpp */
