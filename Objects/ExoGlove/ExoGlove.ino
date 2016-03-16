#include <Bridge.h>
#include <YunClient.h>
#include <MQTTClient.h>

YunClient net;
MQTTClient client;

unsigned long lastMillis = 0;

int flexSensorPin1 = A0;
int flexSensorPin2 = A1;
int flexSensorPin3 = A2;

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  client.begin("broker.shiftr.io", net);

  Serial.println("connecting...");

  if (client.connect("exo-glove", "try", "try")) {
    Serial.println("connected!");
  } else {
    Serial.println("not connected!");
  }
}


void loop() {
  client.loop();

  int flexSensorReading1 = analogRead(flexSensorPin1);
  int flexSensorReading2 = analogRead(flexSensorPin2);
  int flexSensorReading3 = analogRead(flexSensorPin3);

  int value1 = constrain(map(flexSensorReading1, 655, 860, 0, 1000), 0, 1000);
  int value2 = constrain(map(flexSensorReading2, 513, 700, 0, 1000), 0, 1000);
  int value3 = constrain(map(flexSensorReading3, 440, 820, 0, 1000), 0, 1000);

  int valueFingers = value2 + value3;
  int inputValue1 = map(valueFingers, 0, 2000, 0, 1000);
  int inputValue2 = value1;

  if (millis() - lastMillis > 500) {
    client.publish("input/exo-glove/value1", String(inputValue1));
    client.publish("input/exo-glove/value2", String(inputValue2));

    lastMillis = millis();
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {}
