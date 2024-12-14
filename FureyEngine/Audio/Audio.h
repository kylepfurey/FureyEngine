// .h
// SDL Audio Script
// by Kyle Furey

#pragma once
#include "../Standard/Standard.h"

#define ANY_CHANNEL (-1)
#define LOOP_FOREVER (-1)

namespace FureyEngine {
    /** Represents an SDL Mixer chunk that can be played on a sound channel. */
    class Audio final {
        // PATH

        /** The path to this audio file. */
        const std::string AudioPath;

        // AUDIO

        /** The Mix chunk of this audio. */
        Mix_Chunk *MyChunk;

        // MUSIC

        /** The background music currently playing. */
        static Mix_Music *Music;

        // TOTAL CHUNKS

        /** The current number of audio chunks active. */
        static int TotalChunks;

    public:
        // CONSTRUCTORS

        /** Creates a new audio instance from the given path that can be played on a channel. */
        explicit Audio(const std::string &Path);

        /** Delete copy constructor. */
        Audio(const Audio &Copied) = delete;

        /** Delete move constructor. */
        Audio(Audio &&Moved) = delete;

        // ASSIGNMENT OPERATORS

        /** Delete copy constructor. */
        Audio &operator=(const Audio &Copied) = delete;

        /** Delete move constructor. */
        Audio &operator=(Audio &&Moved) = delete;

        // DESTRUCTOR

        /** Properly destroys this audio instance. */
        ~Audio();

        // GETTERS

        /** Returns the path to this audio instance's file. */
        [[nodiscard]] std::string Path() const;

        /** Returns the pointer to the Mix_Chunk of this audio instance. */
        [[nodiscard]] Mix_Chunk *Mix_Chunk() const;

        /** Returns the total number of audio chunks currently active. */
        [[nodiscard]] static int Total();

        // SOUND

        /** Returns whether audio is playing at the given channel. */
        [[nodiscard]] static bool IsPlaying(const int &Channel);

        /**
         * Plays this sound at the first available or given channel for the given number of loops.<br/>
         * Returns the channel this sound was played on or -1 if it failed.
         */
        int Play(const int &Channel = ANY_CHANNEL, const int &Loops = 0) const;

        /** Stops the audio currently playing on the given channel and returns whether it was successful. */
        static bool Stop(const int &Channel);

        /** Returns whether the given channel is paused. */
        [[nodiscard]] static bool IsPaused(const int &Channel);

        /** Pauses the given channel and returns whether it was successful. */
        static bool Pause(const int &Channel);

        /** Unpauses the given channel and returns whether it was successful. */
        static bool Resume(const int &Channel);

        /** Returns the volume of this audio instance. */
        [[nodiscard]] float GetVolume() const;

        /** Sets the volume of this audio instance. */
        void SetVolume(const float &Volume) const;

        /** Sets the volume of the given channel. */
        static void SetChannelVolume(const int &Channel, const float &Volume);

        /** Mutes the given audio channel. */
        static void MuteChannel(const int &Channel);

        /** Sets the global volume of each audio channel. */
        static void SetGlobalVolume(const float &Volume);

        /** Mutes each audio channel. */
        static void MuteAll();

        /** Returns the Mix_Chunk playing at the given channel, if any. */
        static ::Mix_Chunk *GetChunk(const int &Channel);

        // MUSIC

        /** Returns the pointer to the current Mix_Music of this program. */
        [[nodiscard]] static Mix_Music *Mix_Music();

        /** Returns whether music is currently loaded. */
        static bool IsMusicLoaded();

        /**
         * Loads in music to be played separately at the given path.<br/>
         * NOTE: An instance of the Audio class must be present for the duration of your program.
         */
        static ::Mix_Music *LoadMusic(const std::string &Path);

        /** Returns whether music is currently playing. */
        static bool IsMusicPlaying();

        /**
         * Plays the currently loaded music and returns if it was successful.<br/>
         * You can specify how many times to loop or use LOOP_FOREVER for continuous looping.
         */
        static bool PlayMusic(const int &Loops = 0);

        /** Returns whether music is currently fading in. */
        static bool IsMusicFadingIn();

        /**
         * Fades in the currently loaded music for the given number of seconds and returns if it was successful.<br/>
         * You can specify how many times to loop or use LOOP_FOREVER for continuous looping.
         */
        static bool FadeInMusic(const double &Duration, const int &Loops = 0);

        /**
        * Fades in the currently loaded music at the given position (in seconds)<br/>
        * for the given number of seconds and returns if it was successful.<br/>
        * You can specify how many times to loop or use LOOP_FOREVER for continuous looping.
        */
        static bool FadeInMusicAt(const double &Position, const double &Duration, const int &Loops = 0);

        /** Returns the duration of the music (in seconds) or -1 if no music is present. */
        static double GetMusicDuration();

        /** Returns the position of the music (in seconds) or -1 if no music is present. */
        static double GetMusicPosition();

        /** Sets the position of the music (in seconds) and returns if it was successful. */
        static bool SetMusicPosition(const double &Position);

        /** Returns whether music is currently paused. */
        static bool IsMusicPaused();

        /** Pauses the current music and returns whether it was successful. */
        static bool PauseMusic();

        /** Resumes the current music and returns whether it was successful. */
        static bool ResumeMusic();

        /** Returns the volume of the current music (as a percentage) or -1 if no music is present. */
        static float GetMusicVolume();

        /** Sets the volume of the current music (as a percentage) and returns if it was successful. */
        static bool SetMusicVolume(const float &Volume);

        /** Mutes the current music and returns if it was successful. */
        static bool MuteMusic();

        /** Restarts the current music and returns if it was successful. */
        static bool ResetMusic();

        /** Stops the current music and returns if it was successful. */
        static bool StopMusic();

        /** Returns whether music is currently fading out. */
        static bool IsMusicFadingOut();

        /**
         * Fades out the currently playing music for the given number of seconds and returns if it was successful.<br/>
         * After the given duration, the current music track will be stopped.
         */
        static bool FadeOutMusic(const double &Duration = 0);
    };
}
