#pragma once

#include <vector>

#include "Types/AudioMetadata.hpp"
#include "Util/Singleton.hpp"

class LocalFilesService : public Singleton<LocalFilesService> {
public:
    bool init();

    std::vector<AudioMetadata> getAudioFiles() {
        return audioFiles;
    }
    
private:
    std::vector<AudioMetadata> audioFiles;
};