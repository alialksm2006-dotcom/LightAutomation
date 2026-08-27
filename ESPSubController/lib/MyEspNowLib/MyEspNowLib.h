#ifndef ESP_NOW_HELPER_H
#define ESP_NOW_HELPER_H

#include <Arduino.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <espnow.h>
  extern "C" {
    #include "user_interface.h"
  }
#elif defined(ESP32)
  #include <WiFi.h>
  #include <esp_now.h>
  
  
#endif

// دور الجهاز في شبكة ESP-NOW
enum EspNowRole {
  ESP_NOW_ROLE_SENDER,
  ESP_NOW_ROLE_RECEIVER,
  ESP_NOW_ROLE_BOTH
};

// Callback عند الاستقبال
typedef void (*EspNowRecvCallback)(const uint8_t *mac,
                                   const uint8_t *data,
                                   uint8_t        len);

// Callback لنتيجة الإرسال (اختياري)
typedef void (*EspNowSendCallback)(const uint8_t *mac,
                                   bool           success);

// تهيئة ESP-NOW (تحديد الدور والقناة)
 bool espNowBegin(EspNowRole role, uint8_t channel);

// إضافة peer بعنوان MAC
bool espNowAddPeer(const uint8_t mac[6]);

// إرسال بيانات (struct أو أي بيانات خام)
 bool espNowSend(const uint8_t mac[6],
                const void   *data,
                size_t        len);

// تسجيل دالة الاستقبال
 void espNowOnReceive(EspNowRecvCallback cb);

// تسجيل دالة نتيجة الإرسال
 void espNowOnSend(EspNowSendCallback cb);

#endif  // ESP_NOW_HELPER_H
