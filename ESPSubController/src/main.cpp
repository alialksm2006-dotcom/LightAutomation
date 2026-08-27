#include <Arduino.h>
#include <MyESpNowLib.h>

enum Type
    {
        Toggle = 0,
        updateChannel = 1
    };

     typedef struct 
    {
        Type type;
        bool state;
        int pin;
    }DataToggle;

typedef struct
{
  Type type;
  uint8_t newChannel;
} stChangeChannel;

DataToggle dataToggle;
stChangeChannel changeChannelPayload;



void receiveData(const uint8_t* mac, const uint8_t* data, uint8_t len) {
  switch (data[0]) {
    case Type::Toggle:

    
      memcpy(&dataToggle, data, sizeof(dataToggle));
      digitalWrite(dataToggle.pin, !dataToggle.state);
      break;
    case Type::updateChannel:
      memcpy(&changeChannelPayload, data, sizeof(changeChannelPayload));
      wifi_set_channel(changeChannelPayload.newChannel);
      break;
  }
}

void setup() {
   pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

  espNowBegin(ESP_NOW_ROLE_BOTH,2);
  espNowOnReceive(receiveData);
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.channel());
  
}

void loop() {

}