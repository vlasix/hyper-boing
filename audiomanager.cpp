#include "audiomanager.h"
#include <cstdio>

// Initialize static singleton instance
AudioManager* AudioManager::s_instance = nullptr;

#ifdef _WIN32
#include <windows.h>

AudioManager::AudioManager()
    : isPlaying(false), currentTrack("")
{
}

AudioManager& AudioManager::instance()
{
    if (!s_instance)
    {
        s_instance = new AudioManager();
    }
    return *s_instance;
}

void AudioManager::destroy()
{
    if (s_instance)
    {
        s_instance->closeAll();
        delete s_instance;
        s_instance = nullptr;
    }
}

bool AudioManager::openMusic(const char* filename)
{
    char buffer[256];
    std::sprintf(buffer, "open %s type sequencer alias MUSIC", filename);
    
    bool success = (mciSendStringA(buffer, nullptr, 0, nullptr) == 0);
    
    if (success)
    {
        currentTrack = filename;
        isPlaying = false;
    }
    
    return success;
}

bool AudioManager::play()
{
    bool success = (mciSendStringA("play MUSIC from 0 notify", nullptr, 0, nullptr) == 0);
    
    if (success)
    {
        isPlaying = true;
    }
    
    return success;
}

bool AudioManager::stop()
{
    bool success = (mciSendStringA("stop MUSIC", nullptr, 0, nullptr) == 0);
    
    if (success)
    {
        isPlaying = false;
    }
    
    return success;
}

bool AudioManager::resume()
{
    bool success = (mciSendStringA("play MUSIC notify", nullptr, 0, nullptr) == 0);
    
    if (success)
    {
        isPlaying = true;
    }
    
    return success;
}

void AudioManager::closeAll()
{
    mciSendStringA("close all", nullptr, 0, nullptr);
    isPlaying = false;
    currentTrack = "";
}

#else
// Stub implementation for non-Windows platforms

AudioManager::AudioManager()
    : isPlaying(false), currentTrack("")
{
}

AudioManager& AudioManager::instance()
{
    if (!s_instance)
    {
        s_instance = new AudioManager();
    }
    return *s_instance;
}

void AudioManager::destroy()
{
    if (s_instance)
    {
        delete s_instance;
        s_instance = nullptr;
    }
}

bool AudioManager::openMusic(const char* filename)
{
    currentTrack = filename;
    return true;
}

bool AudioManager::play()
{
    isPlaying = true;
    return true;
}

bool AudioManager::stop()
{
    isPlaying = false;
    return true;
}

bool AudioManager::resume()
{
    isPlaying = true;
    return true;
}

void AudioManager::closeAll()
{
    isPlaying = false;
    currentTrack = "";
}

#endif
