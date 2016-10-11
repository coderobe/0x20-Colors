//
// Created by coderobe on 09.10.16.
//

#ifndef INC_0XNATIVE_EFFECT_MANAGER_H
#define INC_0XNATIVE_EFFECT_MANAGER_H

#include <string>
#include <vector>
#include "bass.h"
#include <stdlib.h>
#include "util.h"
#include <thread>

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
        nk_color* background;
        BOOL playing = false;
        bool in_buildup = false;
        vector<string> command_queue;
        string beat_string;
        string beat_string_source;
        vector<HSTREAM> audio_samples;
        HSTREAM audio_buffer = 0;
        BASS_CHANNELINFO audio_info;
        BOOL bass_initialized = false;
        thread timer_thread;
        static void CALLBACK start_timer_proc(HSYNC handle, DWORD channel, DWORD data, void* user){
            *(nk_color *)user = nk_rgb(0, 100, 0);
        }
        bool play_song(audio* track){
            if(!bass_initialized){
                bass_initialized = BASS_Init(-1, 44100, 0, NULL, NULL);
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
            BASS_ChannelGetData(audio_samples[track->sample_id], sample, (DWORD)sample_len);
            basserror("stream read");
            if(audio_buffer != 0) BASS_StreamFree(audio_buffer);
            basserror("stream free");
            audio_buffer = BASS_StreamCreate(
                sample_info.freq, sample_info.chans, BASS_SAMPLE_LOOP, STREAMPROC_PUSH, NULL
            );
            basserror("stream setup");
            BASS_StreamPutData(audio_buffer, sample, (DWORD)sample_len);
            basserror("stream copy");

            BASS_ChannelGetInfo(audio_buffer, &audio_info);
            BASS_ChannelSetSync(audio_buffer, BASS_SYNC_POS, 0, start_timer_proc, (void *)background);
            BASS_ChannelSetAttribute(audio_buffer, BASS_ATTRIB_VOL, 0.1f);
            playing = BASS_ChannelPlay(audio_buffer, false);
            basserror("play_song");

            return true;
        }
        long long read_file(string filename){
            printf("Decoding %s\n", filename.c_str());

            /*HSTREAM astream = BASS_StreamCreateFile(
                    false, filename.c_str(), 0, 0,
                    BASS_STREAM_PRESCAN|BASS_ASYNCFILE
            );*/

            HSTREAM astream       = BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, BASS_STREAM_DECODE);
            if (!astream) astream = BASS_MusicLoad(FALSE, filename.c_str(), 0, 0, BASS_MUSIC_DECODE, 1);

            basserror("decoder");

            audio_samples.push_back(astream);

            printf("Track ID: %lu\n", audio_samples.size()-1);

            return (long long)audio_samples.size()-1;
        }
        bool do_beat(char beat_char){
            // todo: implement
            return false;
        }
        void timer() {
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
