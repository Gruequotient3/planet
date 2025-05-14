#ifndef MUSIC_CPP
#define MUSIC_CPP

#include "music.hpp"

#include "macro.hpp"

Music::Music() : music{nullptr} { }
Music::~Music(){
    if(music){
        if(Mix_PlayingMusic())
          Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
    Mix_CloseAudio();
    Mix_Quit();

}

void Music::Init(std::string path){
    int mixFlags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD, res;
    res = Mix_Init(mixFlags);
    if( (res & mixFlags) != mixFlags ) {
        fprintf(stderr, "Mix_Init: Erreur lors de l'initialisation de la bibliotheque SDL_Mixer\n");
        fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
        //exit(3); commenté car ne réagit correctement sur toutes les architectures
    }
    if(Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 1024) < 0)
        exit(4);
    if(!(music = Mix_LoadMUS(path.c_str()))) {
        fprintf(stderr, "Erreur lors du Mix_LoadMUS: %s\n", Mix_GetError());
        exit(5);
    }
    Mix_SetPostMix(NULL, NULL);
    if(!Mix_PlayingMusic())
        Mix_PlayMusic(music, 1);
}

void Music::SetVolume(float volume, bool inverse){
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    if (inverse)
        Mix_VolumeMusic(lerp((float)MIX_MAX_VOLUME, 0.0f, volume));
    else Mix_VolumeMusic(lerp(0.0f, (float)MIX_MAX_VOLUME, volume));
}




#endif