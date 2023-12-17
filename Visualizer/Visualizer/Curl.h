#include <curl/curl.h>
#include <iostream>
#include <string>

class CurlRequest {
public:
    CurlRequest();

    ~CurlRequest();

    std::string sendGetRequest(const std::string& url);

private:
    CURL* curl;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};
