#include "SoundProgram.hpp"
SoundProgram::SoundProgram(){
    cout << "SoundProgram constructor, should never be called" << endl;
}

/*
 This constructor takes in arguments passed in from the commandline, assigning switches to @switchArguments and creating SoundFiles for #soundFiles
*/
SoundProgram::SoundProgram(vector<string> arguments){
    this->soundFileBuilder = new SoundFileBuilder();
    initSwitchArgumentMap(&arguments);
    initSwitchFunctionMap();
    initSoundFiles(arguments);
}

void SoundProgram::setOutputFileName(string fileName){
    this->outPutFileName = fileName;
}

/*
    @param arguments the file arguments to iterate over and store SoundFile object representations of in #soundFiles.
        Switches have already been removed from @arguments in #initSwitchArgumentMap, 
        if the argument cannot be made into a #SoundFile, an error will be printed to stderr.
*/
void SoundProgram::initSoundFiles(vector<string> arguments){
    for (int i = 0; i < arguments.size(); i++) {
        SoundFile* soundFile = this->soundFileBuilder->buildSoundFileFromFileName(arguments[i]);
        if (soundFile) {
            this->soundFiles.push_back(soundFile);
        } else {
            //something other than a switch was found in @arguments
            fprintf(stderr,"Illegal argument: %s\n", arguments[i].c_str());
            exit(0);

        }
    }
}

/*
 Removes strings resembling switches "-*" and their following values "n" from @arguments and adds the pairs to #switchArgumentMap.
 This function also outputs an error message if an invalid switch was passed in.
 @param arguments the arguments passed in from the command line.
 */
void SoundProgram::initSwitchArgumentMap(vector<string>* arguments){
    const string switchChar = "-";
    vector<string>::iterator iter = (*arguments).begin();
    
    while(iter < (*arguments).end()){
        string argument = *iter;
        size_t foundSwitchChar = argument.find(switchChar);
        
        // if the argument resembles a switch (contains "-") and is not a file name
        if (foundSwitchChar!=string::npos && !(this->soundFileBuilder->isValidFileType(argument, false /*print output*/))) {
            
            if ( isValidSwitchWithNoParams(argument) ) {
                this->switchArgumentMap.insert(make_pair(argument, ""));
                iter = (*arguments).erase(iter);
            } else if ( isValidSwitchWithParams(argument) ){
                //this switch has a parameter, so the next element in @arguments is the value and we need to add it as a value to the hashmap
                try{
                    string switchParamArg = *(iter+1);
                    this->switchArgumentMap.insert(make_pair(argument, switchParamArg));
                    iter = (*arguments).erase(iter);
                    iter = (*arguments).erase(iter); //TODO see if there's a better way to remove things from a vector
                }
                catch(...){
                    cout << "No argument provided for " << argument << endl;
                    exit(1);
                }
            }else{
                cout << "Invalid switch \"" << argument << "\"" << endl;
                exit(1);
            }
        } else {
            iter++; //in the other case, we are removing elements from @arguments and want to leave iter as is.
        }
    }
}

/*
    Iterates through the switches provided to the program and maps the appropriate function to them, storing the pairs in #switchFunctionMap
*/
void SoundProgram::initSwitchFunctionMap(){
    for (const auto &pair : this->switchArgumentMap) {
        SwitchFunction switchFunction = getSwitchFunction(pair.first);
        if (switchFunction) {
            this->switchFunctionMap.insert(make_pair(pair.first, switchFunction));
        }
    }
}

/*
 @param switchArg the switch which determines which function to return
 @return a SwitchFunction, a pointer to a function which implements the given switch
 */
SoundProgram::SwitchFunction SoundProgram::getSwitchFunction(string switchArg){
    if (switchArg.compare("-h")==0) {
        return hSwitch;
    }else{
        cout << "TODO support more switches" << endl;
        return NULL;
    }
}

/*
 Prints the help menu. //TODO add relevant arguments
 */
bool SoundProgram::hSwitch(string argument = NULL){
    cout << "TODO implement help switch" << endl;
    return true;
}


/*
 @param switchArg, the switch argument to test
 @return true if @switchArg is a valid switch that has no parameters following it
 */
bool SoundProgram::isValidSwitchWithNoParams(string switchArg){
    return (switchArg.compare("-h")==0); // the only switch without a param shared by all #SoundPrograms is -h
    
//    vector<string> validNoParamSwitches;
//    validNoParamSwitches.push_back("-h");         TODO use the switch strings you typed here to save time
//    validNoParamSwitches.push_back("--sine");
//    validNoParamSwitches.push_back("--triangle");
//    validNoParamSwitches.push_back("--sawtooth");
//    validNoParamSwitches.push_back("--pulse");
//    
//    return(find(validNoParamSwitches.begin(), validNoParamSwitches.end(), switchArg) != validNoParamSwitches.end());
}

/*
 @param switchArg, the switch argument to test
 @return true if @switchArg is a valid switch that has a parameter following it.
 */
bool SoundProgram::isValidSwitchWithParams(string switchArg){
    return false; // there are no switches with parameters for a basic sound program. I could probably make this a pure virtual function but oh well //TODO
    
    
//    vector<string> validParamSwitches;
//    validParamSwitches.push_back("-o");
//    validParamSwitches.push_back("-f");
//    validParamSwitches.push_back("-t");
//    validParamSwitches.push_back("-v");
//    validParamSwitches.push_back("-a");
//    validParamSwitches.push_back("-d");
//    validParamSwitches.push_back("-s");
//    validParamSwitches.push_back("-r");
//    validParamSwitches.push_back("--bits");
//    validParamSwitches.push_back("--sr");
//    validParamSwitches.push_back("--pf");
//    
//    return(find(validParamSwitches.begin(), validParamSwitches.end(), switchArg) != validParamSwitches.end());
}

