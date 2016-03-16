#include <Bridge.h>
#include <YunClient.h>
#include <MQTTClient.h>

#define FLEX_PIN A0
#define PRESS_PIN A1

int value1 = 0;
int value2 = 0;

YunClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void setup() {
  Bridge.begin();
  Serial.begin(9600);

  client.begin("broker.shiftr.io", net);

  Serial.println("connecting...");
  if(client.connect("brush", "try", "try")) {
    Serial.println("connected!"); 
  }
}

void loop() {
  client.loop();

  int _flex = constrain(map(analogRead(FLEX_PIN), 500, 750, 0, 1000), 0, 1000);
  int _press = constrain(map(analogRead(PRESS_PIN), 0, 200, 0, 1000), 0, 1000);

  if (millis() - lastMillis > 500) {
    lastMillis = millis();

    if(_flex != value1) {
      value1 = _flex;
      client.publish("input/brush/value1", String(value1));  
    }

    if(_press != value2) {
      value2 = _press;
      client.publish("input/brush/value2", String(value2));  
    }
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {}
