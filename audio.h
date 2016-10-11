//
// Created by coderobe on 11.10.16.
//

#ifndef INC_0X20_COLORS_AUDIO_H
#define INC_0X20_COLORS_AUDIO_H

#include <string>
#include "state.h"

using namespace std;

class Audio{
    public:
        Audio(GlobalState *state);
        ~Audio();
    private:
        GlobalState* state;
        bool read_file(string filename);
};

#endif //INC_0X20_COLORS_AUDIO_H
