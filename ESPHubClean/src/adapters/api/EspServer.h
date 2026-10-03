#pragma once

#include <Arduino.h>
#include <HTTPSServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <ResourceNode.hpp>
#include <SSLCert.hpp>
#include <Preferences.h>
#include <WebServer.h>
#include <functional>
#include <utility>
#include <vector>

class SecureWebServer
{
public:
    using THandlerFunction = std::function<void(void)>;

    explicit SecureWebServer(uint16_t port = 443);
    void on(const String &uri, HTTPMethod method, THandlerFunction handler);
    void begin();
    void handleClient();
    bool hasArg(const String &name) const;
    String arg(const String &name) const;
    String header(const String &name) const;
    void collectHeaders(const char *headerKeys[], size_t count);
    void setContentLength(size_t length);
    void sendHeader(const String &name, const String &value, bool first = false);
    void send(int code, const char *contentType, const String &content = String());
    void send(int code, const String &contentType, const String &content);
    void sendContent(const String &content);
    void sendContent(const char *content, size_t length);

private:
    struct Route
    {
        String uri;
        String method;
        THandlerFunction handler;
    };

    struct ResponseHeader
    {
        String name;
        String value;
    };

    uint16_t port;
    httpsserver::HTTPSServer *secureServer = nullptr;
    httpsserver::SSLCert *certificate = nullptr;
    std::vector<Route> routes;
    std::vector<httpsserver::ResourceNode *> nodes;
    std::vector<std::pair<String, String>> arguments;
    std::vector<ResponseHeader> responseHeaders;
    httpsserver::HTTPRequest *currentRequest = nullptr;
    httpsserver::HTTPResponse *currentResponse = nullptr;
    size_t contentLength = CONTENT_LENGTH_UNKNOWN;
    bool chunked = false;
    bool responseStarted = false;

    static void dispatch(httpsserver::HTTPRequest *request,
                         httpsserver::HTTPResponse *response);
    static void notFound(httpsserver::HTTPRequest *request,
                         httpsserver::HTTPResponse *response);
    void handle(httpsserver::HTTPRequest *request,
                httpsserver::HTTPResponse *response);
    bool loadOrCreateCertificate();
    void parseArguments();
    void addArgument(const String &encoded);
    void writeChunk(const String &content);
    void finishResponse();
    static String decode(const String &value);
    static String methodName(HTTPMethod method);
    static SecureWebServer *instance;
};

class EspServer
{
public:
    static SecureWebServer server;
};
