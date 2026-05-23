#pragma once

#include <vector>

#include "Types/MusicCell.hpp"
#include "Util/Singleton.hpp"

class LocalFilesService : public Singleton<LocalFilesService> {
public:
    bool init();

    void loadMusicCells();
    void renderMusicCells();

    std::vector<AudioMetadata> getAudioFiles() {
        return audioFiles;
    }
    
private:
    std::vector<AudioMetadata> audioFiles;
    std::vector<MusicCell> musicCells;
};