#include <bits/stdc++.h>
#include <iostream>
#include <SDL3/SDL.h>
#include "sounds.h"

typedef struct
{
    Uint8 *wav_data;
    Uint32 wav_data_len;
} Sound;

const int AUDIO_CHANNELS = 12;

static Sound music;
static Sound bonus_sound;
static Sound wall_hit_sound;
static Sound death_sound;
static SDL_AudioStream *music_stream;
static SDL_AudioStream *sfx_streams[AUDIO_CHANNELS];
static SDL_AudioDeviceID audio_device = 0;

static bool initialize_sound(const char *path, Sound *sound)
{
    SDL_AudioSpec audio_spec;
    char *wav_path;

    SDL_asprintf(&wav_path, "%s%s", "", path);

    if (!SDL_LoadWAV(wav_path, &audio_spec, &sound->wav_data, &sound->wav_data_len))
    {
        SDL_Log("Failed to load sound %s: %s", path, SDL_GetError());
        return false;
    }

    SDL_free(wav_path);
    return true;
}

static bool initialize_audio_stream(SDL_AudioStream *&stream, SDL_AudioSpec &audio_spec)
{
    stream = SDL_CreateAudioStream(&audio_spec, NULL);

    if (!stream)
    {
        SDL_Log("Failed to create audio stream: %s", SDL_GetError());
        return false;
    }
    else if (!SDL_BindAudioStream(audio_device, stream))
    {
        SDL_Log("Failed to bind audio stream: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool initialize_sounds()
{
    // Open audio device
    audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

    if (audio_device == 0)
    {
        SDL_Log("Failed to open audio device: %s", SDL_GetError());
        return false;
    }

    SDL_AudioSpec audio_spec;
    audio_spec.format = SDL_AUDIO_S16LE;
    audio_spec.channels = 2;
    audio_spec.freq = 44100;

    // Initialize sounds
    if (!initialize_sound("bonus.wav", &bonus_sound))
    {
        return false;
    }
    else if (!initialize_sound("wall_hit.wav", &wall_hit_sound))
    {
        return false;
    }
    else if (!initialize_sound("death.wav", &death_sound))
    {
        return false;
    }
    else if (!initialize_sound("music.wav", &music))
    {
        return false;
    }

    // Initialize audio streams
    if (!initialize_audio_stream(music_stream, audio_spec))
    {
        return false;
    }

    for (int i = 0; i < AUDIO_CHANNELS; i++)
    {
        if (!initialize_audio_stream(sfx_streams[i], audio_spec))
        {
            return false;
        }
    }

    return true;
}

static Sound get_sound(const SoundType sound_type)
{
    switch (sound_type)
    {
    case DEATH:
        return death_sound;
    case BONUS:
        return bonus_sound;
    case WALL_HIT:
        return wall_hit_sound;
    }
}

void play_sfx(SoundType sound_type)
{
    Sound sound = get_sound(sound_type);

    for (SDL_AudioStream *stream : sfx_streams)
    {
        if (SDL_GetAudioStreamAvailable(stream) <= 0)
        {
            SDL_PutAudioStreamData(stream, sound.wav_data, sound.wav_data_len);
            return;
        }
    }
}

void play_music()
{
    if (!SDL_PutAudioStreamData(music_stream, music.wav_data, music.wav_data_len))
    {
        SDL_Log("Failed to play: %s", SDL_GetError());
    }
}

void stop_music()
{
    SDL_ClearAudioStream(music_stream);
}
