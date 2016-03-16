#include <Bridge.h>
#include <YunClient.h>
#include <MQTTClient.h>

YunClient net;
MQTTClient client;

unsigned long lastMillis = 0;

int flexSensorPin1 = A0; // links
int flexSensorPin2 = A1; // rechts
int flexSensorPin3 = A2; // mitte

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  
  client.begin("192.168.1.183", 1337, net);
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("exo-glove", "grafik16", "grafik16")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }

  int flexSensorReading1 = analogRead(flexSensorPin1);
  int flexSensorReading2 = analogRead(flexSensorPin2);

  int value1 = constrain(map(flexSensorReading1, 530, 600, 0, 1000), 0, 1000);
  int value2 = constrain(map(flexSensorReading2, 540, 670, 0, 1000), 0, 1000);

  if (millis() - lastMillis > 500) {
    client.publish("input/exo-glove/value1", String(value1));
    client.publish("input/exo-glove/value2", String(value2));

    lastMillis = millis();
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {}
