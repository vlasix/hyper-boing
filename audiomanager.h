#pragma once

#include <string>

/**
 * AudioManager class (Singleton)
 *
 * Encapsulates audio playback functionality using Windows MCI (Media Control Interface).
 * Provides a clean interface for music control throughout the game.
 *
 * Note: Current implementation is Windows-specific. For cross-platform support,
 * consider replacing MCI with SDL_mixer in the future.
 */
class AudioManager
{
private:
    static AudioManager* s_instance;
    
    bool isPlaying;
    std::string currentTrack;
    
    // Private constructor for singleton
    AudioManager();
    
    // Delete copy constructor and assignment operator
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

public:
    // Singleton accessor
    static AudioManager& instance();
    static void destroy();
    
    // Music control methods
    bool openMusic(const char* filename);
    bool play();
    bool stop();
    bool resume();
    void closeAll();
    
    // State queries
    bool getIsPlaying() const { return isPlaying; }
    const std::string& getCurrentTrack() const { return currentTrack; }
};
