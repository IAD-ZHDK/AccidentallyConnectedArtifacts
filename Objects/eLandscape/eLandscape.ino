#include <Bridge.h>
#include <YunClient.h>
#include <MQTTClient.h>

#define INPUT1 A0
#define INPUT2 A1

YunClient net;
MQTTClient client;

int input1 = 0;
int input2 = 0;
unsigned long lastMillis = 0;

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  
  client.begin("192.168.1.183", 1337, net);
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("e-landscape", "grafik16", "grafik16")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }

  int _input1 = constrain(map(analogRead(INPUT1), 500, 1023, 0, 1000), 0, 1000);
  int _input2 = constrain(map(analogRead(INPUT2), 500, 1023, 0, 1000), 0, 1000);

  if (millis() - lastMillis > 500) {
    if(input1 != _input1) {
      input1 = _input1;
      client.publish("input/e-landscape/value1", String(input1));
    }

    if(input2 != _input2) {
      input2 = _input2;
      client.publish("input/e-landscape/value2", String(input2));
    }

    lastMillis = millis();
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {}

