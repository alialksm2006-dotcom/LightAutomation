#include "MyEspNowLib.h"
// حالة داخلية
static EspNowRole g_role = ESP_NOW_ROLE_RECEIVER;
static EspNowRecvCallback g_recvCb = nullptr;
static EspNowSendCallback g_sendCb = nullptr;
static bool g_inited = false;
static uint8_t g_channel = 1;

// Callbacks داخلية تربط ESP-NOW بالكود الخاص بنا

#if defined(ESP8266)

static void espNowInternalRecvCb(uint8_t *mac,
                                 uint8_t *data,
                                 uint8_t len)
{
  if (g_recvCb)
  {
    g_recvCb(mac, data, len);
  }
}

static void espNowInternalSendCb(uint8_t *mac,
                                 uint8_t status)
{
  if (g_sendCb)
  {
    g_sendCb(mac, status == 0);
  }
}

#elif defined(ESP32)

static void espNowInternalRecvCb(const uint8_t *mac,
                                 const uint8_t *data,
                                 int len)
{
  if (g_recvCb)
  {
    g_recvCb(mac, data, (uint8_t)len);
  }
}

static void espNowInternalSendCb(const uint8_t *mac,
                                 esp_now_send_status_t status)
{
  if (g_sendCb)
  {
    g_sendCb(mac, status == ESP_NOW_SEND_SUCCESS);
  }
}

#endif

bool espNowBegin(EspNowRole role, uint8_t channel)
{
  g_role = role;
  g_channel = channel;

#if defined(ESP8266)
  WiFi.mode(WIFI_STA);
  wifi_set_channel(g_channel);
#elif defined(ESP32)
  WiFi.mode(WIFI_STA);
  // في ESP32 القناة غالباً تضبط مع الـ peer
#endif

#if defined(ESP8266)
  if (esp_now_init() != 0)
  {
    return false;
  }
#elif defined(ESP32)
  if (esp_now_init() != ESP_OK)
  {
    return false;
  }
#endif

  g_inited = true;

#if defined(ESP8266)
  // تعيين الدور
  if (role == ESP_NOW_ROLE_SENDER)
  {
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  }
  else if (role == ESP_NOW_ROLE_RECEIVER)
  {
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  }
  else
  {
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  }

  if (role == ESP_NOW_ROLE_RECEIVER || role == ESP_NOW_ROLE_BOTH)
  {
    esp_now_register_recv_cb(espNowInternalRecvCb);
  }
  if (role == ESP_NOW_ROLE_SENDER || role == ESP_NOW_ROLE_BOTH)
  {
    esp_now_register_send_cb(espNowInternalSendCb);
  }

#elif defined(ESP32)
  // ESP32 لا يستخدم self_role
  if (role == ESP_NOW_ROLE_RECEIVER || role == ESP_NOW_ROLE_BOTH)
  {
    esp_now_register_recv_cb(espNowInternalRecvCb);
  }
  if (role == ESP_NOW_ROLE_SENDER || role == ESP_NOW_ROLE_BOTH)
  {
    esp_now_register_send_cb(espNowInternalSendCb);
  }
#endif

  return true;
}

bool espNowAddPeer(const uint8_t mac[6])
{
 
  if (!g_inited || mac == nullptr)
  {
    // TelnetStream.println("Failed add peer becuase : Not inited");
    return false;
  }
  #if defined(ESP32)
  if (esp_now_is_peer_exist(mac))
  {
    // TelnetStream.println("Peer already exists");
    return true; // Peer already exists, consider it a success
  }

  #elif defined(ESP8266)
    uint8_t macCopy[6];

  memcpy(macCopy,mac,6);
  if (esp_now_is_peer_exist(macCopy))
  {
    // TelnetStream.println("Peer already exists");
    return true; // Peer already exists, consider it a success
  }
  #endif 
#if defined(ESP8266)
  uint8_t peerRole =
      (g_role == ESP_NOW_ROLE_SENDER) ? ESP_NOW_ROLE_SLAVE : ESP_NOW_ROLE_CONTROLLER;

  int r = esp_now_add_peer((uint8_t *)mac,
                           peerRole,
                           g_channel,
                           nullptr,
                           0);
  return (r == 0);

#elif defined(ESP32)
  esp_now_peer_info_t peerInfo = {};

  memcpy(peerInfo.peer_addr, mac, 6);
  peerInfo.channel = g_channel;
  peerInfo.encrypt = false;
  esp_err_t result = esp_now_add_peer(&peerInfo);

  if (result != ESP_OK)
  {
    // TelnetStream.println("result: " + String(result));
    return false;
  }
  return true;
#endif
}

bool espNowSend(const uint8_t mac[6],
                const void *data,
                size_t len)
{

  if (!g_inited)
  {
    return false;
  }
  if (g_role == ESP_NOW_ROLE_RECEIVER)
  {
    // مستقبل فقط، لا يرسل
    return false;
  }
  //  TelnetStream.println("Test from espNowSend //MyEspNowLib.cpp a");

#if defined(ESP8266)
  int r = esp_now_send((uint8_t *)mac,
                       ( uint8_t *)data,
                       (uint8_t)len);
  return (r == 0);

#elif defined(ESP32)
  
  esp_err_t r = esp_now_send(mac,
                             (const uint8_t *)data,
                             len);
  return (r == ESP_OK);
#endif
}

void espNowOnReceive(EspNowRecvCallback cb)
{
  g_recvCb = cb;
}

void espNowOnSend(EspNowSendCallback cb)
{
  g_sendCb = cb;
}
