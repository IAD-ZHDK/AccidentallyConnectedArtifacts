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
  
  client.begin("broker.shiftr.io", net);

  Serial.println("connecting...");
  if (client.connect("e-landscape", "try", "try")) {
    Serial.println("connected!");
  }
}

void loop() {
  client.loop();

  int _input1 = constrain(map(analogRead(INPUT1), 500, 1023, 0, 1000), 0, 1000);
  int _input2 = constrain(map(analogRead(INPUT2), 500, 1023, 0, 1000), 0, 1000);

  if (millis() - lastMillis > 500) {
    lastMillis = millis();

    if(input1 != _input1) {
      input1 = _input1;
      client.publish("input/e-landscape/value1", String(input1));
    }

    if(input2 != _input2) {
      input2 = _input2;
      client.publish("input/e-landscape/value2", String(input2));
    }
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {}

