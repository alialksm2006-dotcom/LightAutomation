#include "EspServer.h"

#include <HTTPSServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <ResourceNode.hpp>
#include <SSLCert.hpp>

#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace
{
    std::vector<unsigned char> certificateBytes;
    std::vector<unsigned char> privateKeyBytes;
}

SecureWebServer *SecureWebServer::instance = nullptr;
SecureWebServer EspServer::server(443);

SecureWebServer::SecureWebServer(uint16_t port) : port(port)
{
    instance = this;
}

String SecureWebServer::methodName(HTTPMethod method)
{
    switch (method)
    {
    case HTTP_GET: return "GET";
    case HTTP_POST: return "POST";
    case HTTP_PUT: return "PUT";
    case HTTP_PATCH: return "PATCH";
    case HTTP_DELETE: return "DELETE";
    case HTTP_OPTIONS: return "OPTIONS";
    case HTTP_HEAD: return "HEAD";
    default: return "GET";
    }
}

void SecureWebServer::on(const String &uri, HTTPMethod method, THandlerFunction handler)
{
    const String routeMethod = methodName(method);
    routes.push_back({uri, routeMethod, handler});
}

void SecureWebServer::begin()
{
    if (!loadOrCreateCertificate())
    {
        Serial.println("HTTPS startup failed: unable to load or create TLS certificate");
        return;
    }

    secureServer = new httpsserver::HTTPSServer(certificate, port, 4);
    for (const Route &route : routes)
    {
        httpsserver::ResourceNode *node = new httpsserver::ResourceNode(
            route.uri.c_str(), route.method.c_str(), &SecureWebServer::dispatch);
        nodes.push_back(node);
        secureServer->registerNode(node);
    }
    httpsserver::ResourceNode *fallback = new httpsserver::ResourceNode(
        "", "", &SecureWebServer::notFound);
    nodes.push_back(fallback);
    secureServer->setDefaultNode(fallback);

    if (!secureServer->start())
    {
        Serial.println("HTTPS server failed to start");
        return;
    }
    Serial.printf("HTTPS server ready at https://%s:%u/\n",
                  WiFi.localIP().toString().c_str(), port);
}

void SecureWebServer::handleClient()
{
    if (secureServer != nullptr && secureServer->isRunning())
        secureServer->loop();
}

bool SecureWebServer::loadOrCreateCertificate()
{
    Preferences preferences;
    const bool opened = preferences.begin("tls_cert", false);
    if (!opened)
        return false;

    const size_t certLength = preferences.getBytesLength("cert");
    const size_t keyLength = preferences.getBytesLength("key");
    bool restored = certLength > 0 && certLength <= UINT16_MAX &&
                    keyLength > 0 && keyLength <= UINT16_MAX;
    if (restored)
    {
        certificateBytes.resize(certLength);
        privateKeyBytes.resize(keyLength);
        restored = preferences.getBytes("cert", certificateBytes.data(), certLength) == certLength &&
                   preferences.getBytes("key", privateKeyBytes.data(), keyLength) == keyLength;
    }

    if (!restored)
    {
        preferences.remove("cert");
        preferences.remove("key");
        preferences.end();

        Serial.println("Generating first-use HTTPS certificate; this may take up to a minute");
        httpsserver::SSLCert generatedCertificate;
        const int result = httpsserver::createSelfSignedCert(
            generatedCertificate,
            httpsserver::KEYSIZE_2048,
            "CN=smart.local,O=LightAutomation",
            "20240101000000",
            "20460101000000");
        if (result != 0 || generatedCertificate.getCertLength() == 0 ||
            generatedCertificate.getPKLength() == 0)
        {
            Serial.printf("TLS certificate generation failed (0x%02X)\n", result);
            return false;
        }

        certificateBytes.assign(generatedCertificate.getCertData(),
                                generatedCertificate.getCertData() +
                                    generatedCertificate.getCertLength());
        privateKeyBytes.assign(generatedCertificate.getPKData(),
                               generatedCertificate.getPKData() +
                                   generatedCertificate.getPKLength());

        if (!preferences.begin("tls_cert", false))
            return false;
        const bool saved = preferences.putBytes("cert", certificateBytes.data(),
                                                certificateBytes.size()) == certificateBytes.size() &&
                           preferences.putBytes("key", privateKeyBytes.data(),
                                                privateKeyBytes.size()) == privateKeyBytes.size();
        preferences.end();
        if (!saved)
            return false;
    }
    else
    {
        preferences.end();
    }

    certificate = new httpsserver::SSLCert(
        certificateBytes.data(), static_cast<uint16_t>(certificateBytes.size()),
        privateKeyBytes.data(), static_cast<uint16_t>(privateKeyBytes.size()));
    return certificate != nullptr;
}

void SecureWebServer::dispatch(httpsserver::HTTPRequest *request,
                               httpsserver::HTTPResponse *response)
{
    if (instance != nullptr)
        instance->handle(request, response);
}

void SecureWebServer::notFound(httpsserver::HTTPRequest *request,
                               httpsserver::HTTPResponse *response)
{
    request->discardRequestBody();
    response->setStatusCode(404);
    response->setHeader("Content-Type", "text/plain; charset=utf-8");
    response->setHeader("Connection", "close");
    response->println("Not found");
}

void SecureWebServer::handle(httpsserver::HTTPRequest *request,
                             httpsserver::HTTPResponse *response)
{
    currentRequest = request;
    currentResponse = response;
    arguments.clear();
    responseHeaders.clear();
    contentLength = CONTENT_LENGTH_UNKNOWN;
    chunked = false;
    responseStarted = false;
    parseArguments();

    String path = request->getRequestString().c_str();
    int queryStart = path.indexOf('?');
    if (queryStart >= 0)
        path.remove(queryStart);

    const String currentMethod = request->getMethod().c_str();
    for (const Route &route : routes)
    {
        if (route.uri == path && route.method == currentMethod)
        {
            route.handler();
            finishResponse();
            currentRequest = nullptr;
            currentResponse = nullptr;
            return;
        }
    }

    notFound(request, response);
    currentRequest = nullptr;
    currentResponse = nullptr;
}

void SecureWebServer::parseArguments()
{
    httpsserver::ResourceParameters *params = currentRequest->getParams();
    for (auto it = params->beginQueryParameters(); it != params->endQueryParameters(); ++it)
        arguments.push_back({it->first.c_str(), decode(it->second.c_str())});

    const size_t bodyLength = currentRequest->getContentLength();
    if (bodyLength == 0)
        return;
    String body;
    body.reserve(bodyLength);
    size_t remaining = bodyLength;
    char buffer[129];
    while (remaining > 0)
    {
        const size_t requested = std::min(remaining, sizeof(buffer) - 1);
        const size_t received = currentRequest->readChars(buffer, requested);
        if (received == 0)
        {
            currentRequest->discardRequestBody();
            break;
        }
        buffer[received] = '\0';
        body += buffer;
        remaining -= received;
    }

    if (body.indexOf("multipart/form-data") >= 0)
        return;
    int start = 0;
    while (start <= body.length())
    {
        int end = body.indexOf('&', start);
        if (end < 0)
            end = body.length();
        if (end > start)
            addArgument(body.substring(start, end));
        if (end == body.length())
            break;
        start = end + 1;
    }
}

void SecureWebServer::addArgument(const String &encoded)
{
    const int equals = encoded.indexOf('=');
    if (equals < 0)
        arguments.push_back({decode(encoded), ""});
    else
        arguments.push_back({decode(encoded.substring(0, equals)),
                             decode(encoded.substring(equals + 1))});
}

String SecureWebServer::decode(const String &value)
{
    String result;
    result.reserve(value.length());
    for (size_t i = 0; i < value.length(); ++i)
    {
        if (value[i] == '+')
            result += ' ';
        else if (value[i] == '%' && i + 2 < value.length())
        {
            char hex[3] = {value[i + 1], value[i + 2], '\0'};
            char *end = nullptr;
            long decoded = strtol(hex, &end, 16);
            if (end != hex && *end == '\0')
            {
                result += static_cast<char>(decoded);
                i += 2;
            }
            else
                result += value[i];
        }
        else
            result += value[i];
    }
    return result;
}

bool SecureWebServer::hasArg(const String &name) const
{
    for (const auto &argument : arguments)
    {
        if (argument.first == name)
            return true;
    }
    return false;
}

String SecureWebServer::arg(const String &name) const
{
    for (const auto &argument : arguments)
    {
        if (argument.first == name)
            return argument.second;
    }
    return "";
}

String SecureWebServer::header(const String &name) const
{
    if (currentRequest == nullptr)
        return "";
    return currentRequest->getHeader(name.c_str()).c_str();
}

void SecureWebServer::collectHeaders(const char *headerKeys[], size_t count)
{
    (void)headerKeys;
    (void)count;
}

void SecureWebServer::setContentLength(size_t length)
{
    contentLength = length;
}

void SecureWebServer::sendHeader(const String &name, const String &value, bool first)
{
    ResponseHeader headerValue = {name, value};
    if (first)
        responseHeaders.insert(responseHeaders.begin(), headerValue);
    else
        responseHeaders.push_back(headerValue);
}

void SecureWebServer::send(int code, const char *contentType, const String &content)
{
    if (currentResponse == nullptr)
        return;
    responseStarted = true;
    currentResponse->setStatusCode(code);
    if (contentType != nullptr && contentType[0] != '\0')
        currentResponse->setHeader("Content-Type", contentType);
    currentResponse->setHeader("Connection", "close");
    for (const ResponseHeader &headerValue : responseHeaders)
        currentResponse->setHeader(headerValue.name.c_str(), headerValue.value.c_str());

    chunked = contentLength == CONTENT_LENGTH_UNKNOWN;
    if (chunked)
        currentResponse->setHeader("Transfer-Encoding", "chunked");
    else
        currentResponse->setHeader("Content-Length", String(content.length()).c_str());

    if (!content.isEmpty())
    {
        if (chunked)
            writeChunk(content);
        else
            currentResponse->print(content);
    }
}

void SecureWebServer::send(int code, const String &contentType, const String &content)
{
    send(code, contentType.c_str(), content);
}

void SecureWebServer::sendContent(const String &content)
{
    if (!responseStarted)
        send(200, "text/html", "");
    if (chunked)
        writeChunk(content);
    else if (currentResponse != nullptr)
        currentResponse->print(content);
}

void SecureWebServer::sendContent(const char *content, size_t length)
{
    if (content != nullptr)
        sendContent(String(content).substring(0, length));
}

void SecureWebServer::writeChunk(const String &content)
{
    if (currentResponse == nullptr || content.isEmpty())
        return;
    char length[12];
    snprintf(length, sizeof(length), "%X", static_cast<unsigned int>(content.length()));
    currentResponse->print(length);
    currentResponse->print("\r\n");
    currentResponse->write(reinterpret_cast<const uint8_t *>(content.c_str()), content.length());
    currentResponse->print("\r\n");
}

void SecureWebServer::finishResponse()
{
    if (chunked && currentResponse != nullptr)
        currentResponse->print("0\r\n\r\n");
}
