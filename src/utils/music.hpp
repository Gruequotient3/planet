#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <string>
#include <SDL_mixer.h>



class Music{
    private:
        Mix_Music *music;
    public:
        Music();
        ~Music();

        void Init(std::string path);

        void SetVolume(float volume, bool inverse);
};

#endif