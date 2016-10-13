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
#include <zconf.h>
#include <fcntl.h>
#include "audio.h"
#include "state.h"

Audio::Audio(GlobalState* state){
    this->state = state;
    this->read_file("loop_TheClockmaker.ogg");
}

Audio::~Audio(){
    delete this->state;
}

//!
//! \brief Audio::play_song starts the audio stream
//! \param song_id Song ID
//! \return true if the audio stream was successfully started
//!
bool Audio::play_song(int song_id) {
    this->state->current_song_id = song_id;
    this->state->current_song = new SongBuffer();
    this->state->current_song->buffer = this->state->songs[song_id]->buffer;
    this->state->current_song->currentPosition = 0;
    this->state->current_song->size = this->state->songs[song_id]->size;
    this->state->current_song->filename = this->state->songs[song_id]->filename;
    this->state->current_song->title = this->state->songs[song_id]->filename;

    auto err = Pa_StartStream(this->state->audio_stream);
    //Pa_Sleep(1000*1000);
    //Pa_StopStream(this->state->audio_stream);

    return err == paNoError;
}

int Audio::init(){
    // Initialize PortAudio
    // Hack to disable annoying Portaudio logging when initializing
    fflush(stderr);
    int bak = dup(2);
    int tmp = open("/dev/null", O_WRONLY);
    dup2(tmp, 2);
    close(tmp);
    int err = Pa_Initialize();
    fflush(stderr);
    dup2(bak, 2);
    close(bak);

    PaDeviceIndex device = Pa_GetDefaultOutputDevice();
    PaStreamParameters parameters;
    parameters.device = device;
    if(parameters.device == paNoDevice){
        return -1;
    }
    const PaDeviceInfo* device_info = Pa_GetDeviceInfo(device);
    if(device_info != 0)
        printf("Output device: %s\n", device_info->name);
    parameters.channelCount = 2; // Stereo output
    parameters.sampleFormat = paFloat32; // 32bit floating point output
    parameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowOutputLatency;
    parameters.hostApiSpecificStreamInfo = NULL;

    // Open default output device as stereo stream
    err = Pa_OpenStream(&this->state->audio_stream, NULL, &parameters, 44100,
                              paFramesPerBufferUnspecified, paClipOff,
                              &Audio::stream_callback, this
    );
    return err;
}

int Audio::stream_callback(const void* input, void* output,
                           unsigned long frameCount,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData
){
    return ((Audio*)userData)->stream(input, output, frameCount, timeInfo, statusFlags);
}

int Audio::stream(const void* inputBuffer, void* outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo* timeInfo,
                  PaStreamCallbackFlags statusFlags
){
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;

    float* out = (float*)outputBuffer;
    unsigned int i;
    SongBuffer* sb = this->state->current_song;

    if (sb->size - sb->currentPosition < 2 ) {
        sb->currentPosition = 0;
    }

    for (i = 0; i < framesPerBuffer; i++) {
        *out++ = sb->buffer[sb->currentPosition++];
        *out++ = sb->buffer[sb->currentPosition++];
    }

    return paContinue;
}

// Private functions

int Audio::read_file(string filename){
    float** buffer;
    long len = 0;
    long frate = 0;
    unsigned int channels = 0;
    vector<float> tmpbuf;
    float* outbuf;
    int bitstream;
    OggVorbis_File vf;
    vorbis_info *info;

    FILE* file = fopen(filename.c_str(), "r");
    if(ov_open(file, &vf, NULL, 0) < 0){
        fprintf(stderr, "File is not a valid OGG bitstream\n");
        return -1;
    }

    while (true) {
        len = ov_read_float(&vf, &buffer, 1024, &bitstream);
        if (len == 0) {
            break;
        }
        if (len < 0) {
            fprintf(stderr, "Read-error in the OGG bitstream, ignoring\n");
            continue;
        }

        info = ov_info(&vf, bitstream);
        frate = info->rate;
        channels = info->channels;

        for (int j = 0; j < channels; j++) {
            for (int i = 0; i < len; i++) {
                tmpbuf.push_back(buffer[j][i]);
            }
        }
    }

    ov_clear(&vf);
    fprintf(stdout, "Decoded %s\n", filename.c_str());
    fprintf(stdout, "Rate: %ld -- Channels: %u\n", frate, channels);

    outbuf = (float*)malloc(tmpbuf.size() * sizeof(float));
    for (int i = 0; i < tmpbuf.size(); i++) {
        outbuf[i] = tmpbuf.at(i);
    }
    // TODO: free(outbuf) when done with playing

    SongBuffer* sb = new SongBuffer();
    sb->buffer = outbuf;
    sb->currentPosition = 0;
    sb->size = tmpbuf.size();
    sb->filename = filename;
    this->state->songs.push_back(sb);

    fclose(file);
    return 0;
}
