//
// Created by coderobe on 09.10.16.
//

#ifndef INC_0XNATIVE_EFFECT_MANAGER_H
#define INC_0XNATIVE_EFFECT_MANAGER_H

#include <string>
#include <vector>
#include <bass.h>
#include <malloc.h>

using namespace std;

class EffectManager{
    public:
        EffectManager(struct nk_color *background){
            this->background = background;
            audio track;
            track.title = "Madeon - Finale (loop)";
            track.filename = "loop_Finale.mp3";
            play_song(track);
        }
        ~EffectManager(){
            delete this->background;
        }
    private:
        struct audio {
            unsigned long sample_id;
            string title;
            const char* filename;
        };
        struct nk_color *background;
        BOOL playing = false;
        bool in_buildup = false;
        vector<string> command_queue;
        string beat_string;
        string beat_string_source;
        vector<HSTREAM> audio_samples;
        HSTREAM audio_buffer;
        BASS_CHANNELINFO audio_info;
        BOOL bass_initialized = false;
        bool play_song(audio track){
            if(track.sample_id == NULL){
                if(track.filename == NULL){
                    return false;
                }else{
                    track.sample_id = read_file(track.filename);
                }
            }

            free(&audio_buffer);
            audio_buffer = (HSTREAM)malloc(sizeof(audio_samples[track.sample_id]));
            memcpy(&audio_buffer, &audio_samples[track.sample_id], sizeof(audio_samples[track.sample_id]));

            BASS_ChannelGetInfo(audio_buffer, &audio_info);

            while(!bass_initialized){
                bass_initialized = BASS_Init(-1, audio_info.freq, audio_info.flags, 0, NULL);
            }

            BASS_SetVolume(0.5f);

            playing = BASS_ChannelPlay(audio_buffer, false);

            return true;
        }
        unsigned long read_file(const char *filename){
            printf("Decoding %s\n", filename);

            HSTREAM astream = BASS_StreamCreateFile(
                    false, filename, 0, 0,
                    BASS_STREAM_DECODE
            );

            audio_samples.push_back(astream);

            return audio_samples.size();
        }
        bool do_beat(char beat_char){

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
