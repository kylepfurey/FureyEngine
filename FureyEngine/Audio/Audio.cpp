// .cpp
// SDL Audio Script
// by Kyle Furey

#include "Audio.h"

namespace FureyEngine {
    // CONSTRUCTORS

    // Creates a new audio instance from the given path that can be played on a channel.
    Audio::Audio(const std::string &Path)
        : AudioPath(Path) {
        // Initialize SDL_mixer
        if (TotalChunks == 0) {
            if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 2048) == -1) {
                throw std::runtime_error("ERROR: Mixer failed to initialize!\nMixer Error: " +
                                         std::string(Mix_GetError()));
            }
        }

        ++TotalChunks;

        // Load the sound file
        MyChunk = Mix_LoadWAV(Path.c_str());
        if (MyChunk == nullptr) {
            throw std::runtime_error("ERROR: Mixer failed to load a sound!\nMixer Error:" +
                                     std::string(Mix_GetError()));
        }
    }

    // DESTRUCTOR

    // Properly destroys this audio instance.
    Audio::~Audio() {
        // Destroys the audio chunk
        if (MyChunk != nullptr) {
            Mix_FreeChunk(MyChunk);
            MyChunk = nullptr;
        }

        --TotalChunks;

        // Close SDL_mixer if there are no more audio chunks remaining
        if (TotalChunks == 0) {
            // Stop and destroy the music
            if (Music != nullptr) {
                Mix_HaltMusic();
                Mix_FreeMusic(Music);
                Music = nullptr;
            }

            Mix_CloseAudio();
            Mix_Quit();
        }
    }

    // GETTERS

    // Returns the path to this audio instance's file.
    std::string Audio::Path() const {
        return AudioPath;
    }

    // Returns the Mix_Chunk of this audio instance.
    Mix_Chunk *Audio::Mix_Chunk() const {
        return MyChunk;
    }

    // Returns the pointer to the current Mix_Music of this program.
    Mix_Music *Audio::Mix_Music() {
        return Music;
    }

    // Returns the total number of audio chunks currently active.
    int Audio::Total() {
        return TotalChunks;
    }

    // SOUND

    // Returns whether audio is playing at the given channel.
    bool Audio::IsPlaying(const int &Channel) {
        return Mix_Playing(Channel) != 0;
    }

    // Plays this sound at the first available or given channel for the given number of loops.
    // Returns the channel this sound was played on or -1 if it failed.
    int Audio::Play(const int &Channel, const int &Loops) const {
        return Mix_PlayChannel(Channel, MyChunk, Loops);
    }

    // Stops the audio playing on the given channel.
    bool Audio::Stop(const int &Channel) {
        return Mix_HaltChannel(Channel) == 0;
    }

    // Returns whether the given channel is paused.
    bool Audio::IsPaused(const int &Channel) {
        return Mix_Paused(Channel);
    }

    // Pauses the given channel and returns whether it was successful.
    bool Audio::Pause(const int &Channel) {
        const bool Success = !Mix_Paused(Channel);
        Mix_Pause(Channel);
        return Success;
    }

    // Unpauses the given channel and returns whether it was successful.
    bool Audio::Resume(const int &Channel) {
        const bool Success = Mix_Paused(Channel);
        Mix_Resume(Channel);
        return Success;
    }

    // Returns the volume of this audio instance.
    float Audio::GetVolume() const {
        return static_cast<float>(MyChunk->volume) / MIX_MAX_VOLUME;
    }

    // Sets the volume of this audio instance.
    void Audio::SetVolume(const float &Volume) const {
        Mix_VolumeChunk(MyChunk, SDL_clamp(static_cast<int>(Volume * MIX_MAX_VOLUME), 0, MIX_MAX_VOLUME));
    }

    // Sets the volume of the given channel.
    void Audio::SetChannelVolume(const int &Channel, const float &Volume) {
        Mix_Volume(Channel, SDL_clamp(static_cast<int>(Volume * MIX_MAX_VOLUME), 0, MIX_MAX_VOLUME));
    }

    // Mutes the given audio channel.
    void Audio::MuteChannel(const int &Channel) {
        Mix_Volume(Channel, 0);
    }

    // Sets the global volume of each audio channel.
    void Audio::SetGlobalVolume(const float &Volume) {
        Mix_MasterVolume(SDL_clamp(static_cast<int>(Volume * MIX_MAX_VOLUME), 0, MIX_MAX_VOLUME));
    }

    // Mutes each audio channel.
    void Audio::MuteAll() {
        Mix_MasterVolume(0);
    }

    // Returns the Mix_Chunk playing at the given channel, if any.
    Mix_Chunk *Audio::GetChunk(const int &Channel) {
        return Mix_GetChunk(Channel);
    }

    // MUSIC

    // Returns whether music is currently loaded.
    bool Audio::IsMusicLoaded() {
        return Music != nullptr;
    }

    // Loads in music to be played separately at the given path.
    // NOTE: An instance of the Audio class must be present for the duration of your program.
    Mix_Music *Audio::LoadMusic(const std::string &Path) {
        // Stop and destroy the music if necessary
        if (Music != nullptr) {
            Mix_HaltMusic();
            Mix_FreeMusic(Music);
        }

        // Load new music
        Music = Mix_LoadMUS(Path.c_str());
        if (Music == nullptr) {
            throw std::runtime_error("ERROR: Mixer failed to load music!\nMixer Error:" +
                                     std::string(Mix_GetError()));
        }

        return Music;
    }

    // Returns whether music is currently playing.
    bool Audio::IsMusicPlaying() {
        if (Music == nullptr) {
            return false;
        }
        return Mix_PlayingMusic();
    }

    // Plays the currently loaded music and returns if it was successful.
    // You can specify how many times to loop or use LOOP_FOREVER for continuous looping.
    bool Audio::PlayMusic(const int &Loops) {
        if (Music == nullptr) {
            return false;
        }
        if (Mix_PlayingMusic()) {
            Mix_RewindMusic();
            return true;
        }
        return Mix_PlayMusic(Music, Loops) == 0;
    }

    // Returns whether music is currently fading in.
    bool Audio::IsMusicFadingIn() {
        if (Music == nullptr) {
            return false;
        }
        return Mix_FadingMusic() == MIX_FADING_IN;
    }

    // Fades in the currently loaded music for the given number of seconds and returns if it was successful.
    // You can specify how many times to loop or use LOOP_FOREVER for continuous looping.
    bool Audio::FadeInMusic(const double &Duration, const int &Loops) {
        if (Music == nullptr) {
            return false;
        }
        if (Mix_PlayingMusic()) {
            Mix_HaltMusic();
        }
        return Mix_FadeInMusic(Music, Loops, static_cast<int>(Duration * 1000)) == 0;
    }

    // Fades in the currently loaded music at the given position (in seconds)
    // for the given number of seconds and returns if it was successful.
    // You can specify how many times to loop or use LOOP_FOREVER for continuous looping.
    bool Audio::FadeInMusicAt(const double &Position, const double &Duration, const int &Loops) {
        if (Music == nullptr) {
            return false;
        }
        if (Mix_PlayingMusic()) {
            Mix_HaltMusic();
        }
        return Mix_FadeInMusicPos(Music, Loops, static_cast<int>(Duration * 1000), Position) == 0;
    }

    // Returns the duration of the music (in seconds) or -1 if no music is present.
    double Audio::GetMusicDuration() {
        if (Music == nullptr) {
            return -1;
        }
        return Mix_MusicDuration(Music);
    }

    // Returns the position of the music (in seconds) or -1 if no music is present.
    double Audio::GetMusicPosition() {
        if (Music == nullptr) {
            return -1;
        }
        return Mix_GetMusicPosition(Music);
    }

    // Sets the position of the music (in seconds) and returns if it was successful.
    bool Audio::SetMusicPosition(const double &Position) {
        if (Music == nullptr) {
            return false;
        }
        return Mix_SetMusicPosition(Position) == 0;
    }

    // Pauses the current music and returns whether it was successful.
    bool Audio::PauseMusic() {
        if (Music == nullptr) {
            return false;
        }
        const bool Success = !Mix_PausedMusic();
        Mix_PauseMusic();
        return Success;
    }

    // Resumes the current music and returns whether it was successful.
    bool Audio::ResumeMusic() {
        if (Music == nullptr) {
            return false;
        }
        const bool Success = Mix_PausedMusic();
        Mix_ResumeMusic();
        return Success;
    }

    // Returns the volume of the current music or -1 if no music is present.
    float Audio::GetMusicVolume() {
        if (Music == nullptr) {
            return -1;
        }
        return static_cast<float>(Mix_GetMusicVolume(Music)) / MIX_MAX_VOLUME;
    }

    // Sets the volume of the current music and returns if it was successful.
    bool Audio::SetMusicVolume(const float &Volume) {
        if (Music == nullptr) {
            return false;
        }
        Mix_VolumeMusic(SDL_clamp(static_cast<int>(Volume * MIX_MAX_VOLUME), 0, MIX_MAX_VOLUME));
        return true;
    }

    // Mutes the current music and returns if it was successful.
    bool Audio::MuteMusic() {
        if (Music == nullptr) {
            return false;
        }
        Mix_VolumeMusic(0);
        return true;
    }

    // Restarts the current music and returns if it was successful.
    bool Audio::ResetMusic() {
        if (Music == nullptr) {
            return false;
        }
        Mix_RewindMusic();
        return true;
    }

    // Returns whether music is currently paused.
    bool Audio::IsMusicPaused() {
        if (Music == nullptr) {
            return false;
        }
        return Mix_PausedMusic();
    }

    // Stops the current music and returns if it was successful.
    bool Audio::StopMusic() {
        if (Music == nullptr) {
            return false;
        }
        Mix_HaltMusic();
        return true;
    }

    // Returns whether music is currently fading out.
    bool Audio::IsMusicFadingOut() {
        if (Music == nullptr) {
            return false;
        }
        return Mix_FadingMusic() == MIX_FADING_OUT;
    }

    // Fades out the currently playing music for the given number of seconds and returns if it was successful.
    // After the given duration, the current music track will be stopped.
    bool Audio::FadeOutMusic(const double &Duration) {
        if (Music == nullptr) {
            return false;
        }
        return Mix_FadeOutMusic(static_cast<int>(Duration * 1000));
    }

    // STATIC VARIABLE INITIALIZATION

    // The background music currently playing.
    Mix_Music *Audio::Music = nullptr;

    // The current number of audio chunks active.
    int Audio::TotalChunks = 0;
}
