#include "SoundProgram.hpp"
SoundProgram::SoundProgram(){
    this->soundFileBuilder = new SoundFileBuilder();
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
    for (int i = 0; (i < arguments.size()) || (arguments.size()==0) ; i++) {
        SoundFile* soundFile = this->soundFileBuilder->buildSoundFileFromInput(arguments.size()==0 ? "" : arguments[i]);
        if (soundFile) {
            this->soundFiles.push_back(soundFile);
        } else {
            //something other than a switch was found in @arguments
            string illegalArgument = arguments.size()==0 ? "" : ": " + arguments[i];
            fprintf(stderr,"Illegal argument%s\n", illegalArgument.c_str());
            exit(0);

        }
        if (arguments.size()==0) {
            break; //no file arguments, we only needed to build a file from stdin and need this hack.
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
            
            if ( isValidSwitch(argument, false /* with parameters = false */) ) {
                this->switchArgumentMap.insert(make_pair(argument, ""));
                iter = (*arguments).erase(iter);
            } else if ( isValidSwitch(argument, true /* with parameters = true */) ){
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
        return &SoundProgram::hSwitch;
    }else if(switchArg.compare("-o")==0){
        return &SoundProgram::oSwitch;
    }else{
        return NULL;
    }
}


/*
 Iterates through switches in #switchArgumentMap and calls the corresponding functions in #switchArgumentMap.
 */
void SoundProgram::runSwitches(){
    for (const auto &pair : this->switchArgumentMap) {
        SwitchFunction switchFunction = getSwitchFunction(pair.first);
        if (switchFunction) {
            bool success = (this->*switchFunction)(pair.second);
            if (!success) {
                fprintf(stderr, "An error has occured while running switch %s", (pair.first).c_str());
            }
            if (pair.first.compare("-h")==0) {
                exit(0); // after the help screen is displayed, the program should to terminate.
            }
        }
    }
}

/*
 @param string outputFileName the output file name
 @return true if @outputFileName is a valid file
 */
bool SoundProgram::oSwitch(string outputFileName){ //TODO move this to SndCat
    if(!this->soundFileBuilder->isValidFileType(outputFileName, false /*print output*/)){
        return false;
    }
    this->setOutputFileName(outputFileName);
    return true;    //TODO use for -o
}


/*
    Prints the help screen which describes the program, its usage, and all legal switches, then exits the program.
 */
bool SoundProgram::hSwitch(string argument = NULL){
    cout << this->getProgramName() << " Help Screen" << endl;
    cout << " - - - - - " << endl;
    cout << this->getProgramDescription() << endl;
    cout << "- Legal Switches -" << endl;
    vector <string> validSwitches = getValidSwitches(false /* with params = false */);
    vector<string> validSwtichesWithParams = getValidSwitches(true);
    validSwitches.insert(validSwitches.end(), validSwtichesWithParams.begin(), validSwtichesWithParams.end());
    for (auto &switchName : validSwitches) {
        cout << switchName << ": " << getSwitchDescription(switchName) << endl;
    }
    return true;
}

/*
    @param bool @withParams if the vector of switches to return are the type that contain parameters.
    @return a vector of strings representing valid switches for this program. (ex. "-h")
*/
vector<string> SoundProgram::getValidSwitches(bool withParams){
    vector<string> switches;
    if (!withParams) { // there are no switches with parameters for a basic sound program.
        switches.push_back("-h");
    }
    return switches;
}

/*
    @param switchArg, the switch argument to test
    @param withParams, if the switch argument provided is to be compared with other switches that take parameters.
    @return true if @switchArg is a valid switch specified by @withParams
*/
bool SoundProgram::isValidSwitch(string switchArg, bool withParams){
    vector<string> validSwitches = getValidSwitches(withParams /* with parameters */);
    return (validSwitches.size()!=0 && find(validSwitches.begin(), validSwitches.end(), switchArg)!=validSwitches.end());
}

/*
    @param switchName, the switch to return a description of.
    @return a description of what the switch referred to by @switchName does, to be used in #hSwitch
*/
string SoundProgram::getSwitchDescription(string switchName){ //TODO add more descriptions.
    string description = "";
    
    if (switchName.compare("-h")==0) {
        description+= "The -h switch prints the help screen. You already know that though, since you're currently reading the help screen.";
    } else if (switchName.compare("-o")==0){
        description+= "The -o 'filename' switch sets the output file of the program.";
    }
    
    return description;
}

////    vector<string> validNoParamSwitches;
////    validNoParamSwitches.push_back("-h");         TODO use the switch strings you typed here to save time
////    validNoParamSwitches.push_back("--sine");
////    validNoParamSwitches.push_back("--triangle");
////    validNoParamSwitches.push_back("--sawtooth");
////    validNoParamSwitches.push_back("--pulse");
////    
////    return(find(validNoParamSwitches.begin(), validNoParamSwitches.end(), switchArg) != validNoParamSwitches.end());
//}

//    
////    vector<string> validParamSwitches;
////    validParamSwitches.push_back("-o");
////    validParamSwitches.push_back("-f");
////    validParamSwitches.push_back("-t");
////    validParamSwitches.push_back("-v");
////    validParamSwitches.push_back("-a");
////    validParamSwitches.push_back("-d");
////    validParamSwitches.push_back("-s");
////    validParamSwitches.push_back("-r");
////    validParamSwitches.push_back("--bits");
////    validParamSwitches.push_back("--sr");
////    validParamSwitches.push_back("--pf");
////    
////    return(find(validParamSwitches.begin(), validParamSwitches.end(), switchArg) != validParamSwitches.end());
//}

