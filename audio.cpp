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
    this->read_file("test.ogg");
}

Audio::~Audio(){
    delete this->state;
}

// Private functions

bool Audio::read_file(string filename){
    OggVorbis_File vf;
    int eof = 0;
    int current_section;
    char pcmout[4096];
    FILE* file = fopen(filename.c_str(), "r");
    if(ov_open(file, &vf, NULL, 0) < 0){
        fprintf(stderr, "File is not a valid OGG bitstream");
        return false;
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
            fwrite(pcmout, 1, ret, stdout);
        }
    }

    ov_clear(&vf);
    fprintf(stdout, "Done\n");

    return true;
}