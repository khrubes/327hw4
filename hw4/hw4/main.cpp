#include "all.h"
#include "SoundFileBuilder.hpp"

int main(int argc, const char * argv[]) {
    SoundFileBuilder sfB;
    SoundFile* sfP = sfB.buildSoundFileFromFileName("/Users/kelseyhrubes/Documents/327/hw4/hw4/hw4/sf1.cs229");
    return 0;
}
