#include "WirelessDiscovery.h"
#include <WiFi.h>
#include "MyEspNowLib.h"

namespace
{
const uint8_t discoveryMessage[] = "WIRELESS_DISCOVERY_V1";
struct DiscoveryPacket
{
    uint8_t mac[6];
    uint8_t channel;
};
DiscoveryPacket discoveryQueue[16];
portMUX_TYPE discoveryMux = portMUX_INITIALIZER_UNLOCKED;
uint8_t queueReadIndex = 0;
uint8_t queueWriteIndex = 0;
uint8_t queuedDiscoveries = 0;
bool discoveryStarted = false;
}

void WirelessDiscovery::onReceive(const uint8_t *mac, const uint8_t *data, uint8_t length)
{
    if (mac == NULL || data == NULL ||
        length != sizeof(discoveryMessage) - 1 ||
        memcmp(data, discoveryMessage, sizeof(discoveryMessage) - 1) != 0)
        return;

    portENTER_CRITICAL(&discoveryMux);
    for (uint8_t i = 0; i < queuedDiscoveries; ++i)
    {
        uint8_t index = (queueReadIndex + i) % 16;
        if (memcmp(discoveryQueue[index].mac, mac, 6) == 0)
        {
            portEXIT_CRITICAL(&discoveryMux);
            return;
        }
    }
    if (queuedDiscoveries == 16)
    {
        queueReadIndex = (queueReadIndex + 1) % 16;
        --queuedDiscoveries;
    }
    memcpy(discoveryQueue[queueWriteIndex].mac, mac, 6);
    discoveryQueue[queueWriteIndex].channel = WiFi.channel();
    queueWriteIndex = (queueWriteIndex + 1) % 16;
    ++queuedDiscoveries;
    portEXIT_CRITICAL(&discoveryMux);
}

bool WirelessDiscovery::begin()
{
    if (discoveryStarted)
        return true;
    if (!espNowBegin(ESP_NOW_ROLE_BOTH, WiFi.channel()))
        return false;
    espNowOnReceive(onReceive);
    discoveryStarted = true;
    return true;
}

bool WirelessDiscovery::take(uint8_t mac[6], uint8_t &channel)
{
    bool found;
    portENTER_CRITICAL(&discoveryMux);
    found = queuedDiscoveries > 0;
    if (found)
    {
        memcpy(mac, discoveryQueue[queueReadIndex].mac, 6);
        channel = discoveryQueue[queueReadIndex].channel;
        queueReadIndex = (queueReadIndex + 1) % 16;
        --queuedDiscoveries;
    }
    portEXIT_CRITICAL(&discoveryMux);
    return found;
}
