#include "Util/WebManager.hpp"

#include <curlcpp/curl_easy.h>
#include <curlcpp/curl_exception.h>

using curl::curl_easy;
using curl::curl_easy_exception;

size_t WebManager::imageWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    auto* buf = static_cast<ImageBuffer*>(userp);
    auto* bytes = static_cast<unsigned char*>(contents);
    buf->data.insert(buf->data.end(), bytes, bytes + total);
    return total;
}

bool WebManager::fetchImage(const std::string& url, ImageBuffer& out) {
    curl_easy easy;
    try {
        easy.add<CURLOPT_URL>(url.c_str());
        easy.add<CURLOPT_FOLLOWLOCATION>(1L);
        easy.add<CURLOPT_WRITEFUNCTION>(imageWriteCallback);
        easy.add<CURLOPT_WRITEDATA>(&out);
        easy.perform();
        return true;
    } catch (curl_easy_exception& e) {
        return false;
    }
}