#pragma once

#include <cstring>
#include <string>
#include <vector>

class WebManager {
public:
    struct ImageBuffer {
        std::vector<unsigned char> data;
    };

    static WebManager get() {
        static WebManager instance;
        return instance;
    }

    bool fetchImage(const std::string& url, ImageBuffer& out);

private:
    static size_t imageWriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};