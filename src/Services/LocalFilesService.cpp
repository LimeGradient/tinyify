#include "Services/LocalFilesService.hpp"

#include <filesystem>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

#include "Util/Logging.hpp"

namespace fs = std::filesystem;

bool LocalFilesService::init() {
    if (!fs::exists("LocalFiles/")) {
        fs::create_directory("LocalFiles/");
    }

    for (const auto& file : fs::directory_iterator("LocalFiles/")) {
        Logging::info("Found file: {}", file.path().filename().string());

        TagLib::FileRef audioFile(file.path().string().c_str());
        TagLib::ByteVector imageData;

        if (auto mpegFile = dynamic_cast<TagLib::MPEG::File*>(audioFile.file())) {
            TagLib::ID3v2::Tag* tag = mpegFile->ID3v2Tag();
            auto frames = tag->frameList("APIC");
            if (!frames.isEmpty()) {
                auto frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
                imageData = frame->picture();
            }
        }
        
        std::string title = audioFile.tag()->title().toCString();
        title.erase(0, title.find_first_not_of(" \t\n\r\f\v"));
        
        audioFiles.push_back(AudioMetadata(
            file.path().string(),
            title,
            std::string(audioFile.tag()->album().toCString()),
            std::string(audioFile.tag()->artist().toCString()),
            imageData
        ));
    }

    return true;
}

void LocalFilesService::loadMusicCells() {
    for (auto file : audioFiles) {
        musicCells.push_back(MusicCell(
            file
        ));
    }
}

void LocalFilesService::renderMusicCells() {
    for (auto cell : musicCells) {
        cell.render();
    }
}