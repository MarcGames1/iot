#include "Curl.h"

CurlRequest::CurlRequest() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

CurlRequest::~CurlRequest() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

std::string CurlRequest::sendGetRequest(const std::string& url) {
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }

    return readBuffer;
}

size_t CurlRequest::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    size_t oldLength = s->size();
    try {
        s->resize(oldLength + newLength);
    }
    catch (std::bad_alloc& e) {
        //handle memory problem
        return 0;
    }

    std::copy((char*)contents, (char*)contents + newLength, s->begin() + oldLength);
    return size * nmemb;
}