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

  client.begin("192.168.1.183", 1337, net);

  motor1.setup();
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("sphere", "grafik16", "grafik16")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");

  client.subscribe("/output/sphere/*");
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }
  
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
