//
// Created by coderobe on 09.10.16.
//

#ifndef INC_0XNATIVE_EFFECT_MANAGER_H
#define INC_0XNATIVE_EFFECT_MANAGER_H

#include <string>
#include <vector>
#include <bass.h>
#include <malloc.h>
#include <stdlib.h>
#include "util.h"

using namespace std;

class EffectManager{
    public:
        EffectManager(struct nk_color* background){
            this->background = background;
            audio *track = new audio();
            track->title = "Madeon - Finale (loop)";
            track->filename = "loop_Finale.mp3";
            play_song(track);
        }
        ~EffectManager(){
            delete this->background;
        }
    private:
        struct audio {
            long long sample_id = -1;
            string title = "";
            string filename = "";
        };
        struct nk_color *background;
        BOOL playing = false;
        bool in_buildup = false;
        vector<string> command_queue;
        string beat_string;
        string beat_string_source;
        vector<HSTREAM> audio_samples;
        HSTREAM audio_buffer = 0;
        BASS_CHANNELINFO audio_info;
        BOOL bass_initialized = false;
        bool play_song(audio* track){
            if(!bass_initialized){
                bass_initialized = BASS_Init(-1, audio_info.freq, audio_info.flags, 0, NULL);
                if(!bass_initialized){
                    basserror("init");
                }
            }

            if(track->sample_id == -1){
                if(track->filename == ""){
                    return false;
                }else{
                    track->sample_id = read_file(track->filename);
                }
            }
            printf("%s\n", track->title.c_str());

            QWORD sample_len = BASS_ChannelGetLength(audio_samples[track->sample_id], BASS_POS_BYTE);
            BASS_CHANNELINFO sample_info;
            BASS_ChannelGetInfo(audio_samples[track->sample_id], &sample_info);
            void* sample = malloc(sample_len);
            BASS_ChannelGetData(audio_samples[track->sample_id], sample, BASS_DATA_AVAILABLE);
            basserror("stream read");
            if(audio_buffer != 0) BASS_StreamFree(audio_buffer);
            basserror("stream free");
            audio_buffer = BASS_StreamCreate(
                sample_info.freq, sample_info.chans, sample_info.flags, STREAMPROC_PUSH, NULL
            );
            basserror("stream setup");
            BASS_StreamPutData(audio_buffer, sample, (DWORD)sample_len);
            basserror("stream copy");

            BASS_ChannelGetInfo(audio_buffer, &audio_info);
            basserror("getting info");

            playing = BASS_ChannelPlay(audio_buffer, false);
            basserror("playing");

            return true;
        }
        long long read_file(string filename){
            printf("Decoding %s\n", filename.c_str());

            HSTREAM astream = BASS_StreamCreateFile(
                    false, filename.c_str(), 0, 0,
                    BASS_STREAM_PRESCAN|BASS_ASYNCFILE
            );

            basserror("decoder");

            audio_samples.push_back(astream);

            printf("Track ID: %lu\n", audio_samples.size()-1);

            return (long long)audio_samples.size()-1;
        }
        bool do_beat(char beat_char){
            // todo: implement
            return false;
        }
        void timer_thread() {
            while (playing) {
                if(beat_string.size() > 0){
                    do_beat(beat_string.at(0));
                    beat_string = beat_string.erase(0, 1);
                    if(beat_string.size() < beat_string_source.size()){
                        beat_string += beat_string_source;
                    }
                }
            }
        }
};
#endif //INC_0XNATIVE_EFFECT_MANAGER_H
