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
        int init();
        bool play_song(int song_id);
        int stream(const void* inputBuffer, void* outputBuffer,
                   unsigned long framesPerBuffer,
                   const PaStreamCallbackTimeInfo* timeInfo,
                   PaStreamCallbackFlags statusFlags);
        static int stream_callback(const void *input,
                                   void *output,
                                   unsigned long frameCount,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData);
    private:
        GlobalState* state;
        int read_file(string filename);
};

#endif //INC_0X20_COLORS_AUDIO_H
