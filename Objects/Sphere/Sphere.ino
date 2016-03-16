#include "Motor.h"
#include <Bridge.h>
#include <YunClient.h>
#include <MQTTClient.h>

YunClient net;
MQTTClient client;

Motor motor1(10, 11, 9);

int lastValue = 0;
bool lastDir = 1;

void setup() {
  Bridge.begin();
  Serial.begin(9600);

  client.begin("broker.shiftr.io", net);

  Serial.println("connecting...");
  if (client.connect("sphere", "try", "try")) {
    Serial.println("conncted!");
    client.subscribe("output/sphere/*");
  }
  
  motor1.setup();
}

void loop() {
  client.loop();
  motor1.update();
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  if (topic.equals("output/sphere/value1")) {
    int value = payload.toInt();
    value = map(value, 0, 1000, 0, 255);
    motor1.setSpeed(value);
  } else if (topic.equals("output/sphere/value2")) {
    int value = payload.toInt();
    if (abs(value - lastValue) > 400) {
      lastValue = value;
      lastDir = !lastDir;
      motor1.setDirection(lastDir);
    }
  }
}
