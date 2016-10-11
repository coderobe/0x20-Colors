//
// Created by coderobe on 11.10.16.
//

#include <portaudio.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "audio.h"
#include "state.h"

Audio::Audio(GlobalState* state){
    this->state = state;
    //this->read_file("loop_TheClockmaker.ogg");
}

Audio::~Audio(){
    delete this->state;
}

// Private functions

bool Audio::play_song(int song_id) {
    //this->state->current_song_id = song_id;
    //this->state->current_song = this->state->songs[song_id];

    Pa_StartStream(this->state->audio_stream);
    //Pa_Sleep(1000*1000);
    //Pa_StopStream(this->state->audio_stream);

    return true;
}

int Audio::init(){
    // Initialize PortAudio
    int err = Pa_Initialize();
    PaStream* stream = this->state->audio_stream;
    // Open default output device as stereo stream
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100,
                              paFramesPerBufferUnspecified,
                              &Audio::stream_callback, &this->state
    );
    return err;
}

int Audio::stream_callback(const void* input, void* output,
                           unsigned long frameCount,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData
){
    float* out = (float*)output;
    unsigned int i;
    for(i = 0; i < frameCount; i++){
        *out++ = 0.05f;
        *out++ = 1.0f;
    }
    fprintf(stderr, "blah %i\n", i);
    return paContinue;
}

int Audio::read_file(string filename){
    OggVorbis_File vf;
    int eof = 0;
    int current_section;
    char pcmout[4096];
    vector<long> buffer;
    FILE* file = fopen(filename.c_str(), "r");
    if(ov_open(file, &vf, NULL, 0) < 0){
        fprintf(stderr, "File is not a valid OGG bitstream");
        return -1;
    }
    char** ptr = ov_comment(&vf, -1)->user_comments;
    vorbis_info* vi = ov_info(&vf, -1);
    while(*ptr){
        fprintf(stderr, "%s\n", *ptr);
        ++ptr;
    }
    fprintf(stdout, "Bitstream is %d channel, %ldHz\n", vi->channels, vi->rate);
    fprintf(stdout, "Decoded length: %ld samples\n", (long)ov_pcm_total(&vf, -1));
    fprintf(stdout, "Encoded by %s\n", ov_comment(&vf, -1)->vendor);

    while(!eof){
        long ret = ov_read(&vf, pcmout, sizeof(pcmout), 0, 2, 1, &current_section);
        if(ret == 0){
            eof = 1;
        }else if(ret < 0){
            fprintf(stderr, "Read-error in the OGG bitstream");
        }else{
            ret = ret+1;
            for(long i = ret; i > 0; i--){
                long x = ret - i;
                buffer.push_back(pcmout[x]);
            }
            //fwrite(pcmout, 1, ret, stdout);
        }
    }

    ov_clear(&vf);
    fprintf(stdout, "Decoded %s\n", filename.c_str());

    SongBuffer* sb = new SongBuffer();
    sb->buffer = buffer;
    sb->filename = filename;
    this->state->songs.push_back(sb);

    return 0;
}