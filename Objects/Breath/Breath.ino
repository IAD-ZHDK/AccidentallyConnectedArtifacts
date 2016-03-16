#define RED_LED 11 // PWM fähiger PIN

#include <Bridge.h>
#include <YunClient.h>
#include <MQTTClient.h>

YunClient net;
MQTTClient client;

const int analogInPin = A0;  
const int analogOutPin = 11; 

int sensorValue = 0;        
int outputValue = 0;        
int oldOutputValue = 0;

unsigned long lastTime = 0;

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  
  pinMode(RED_LED, OUTPUT);
  client.begin("192.168.1.183", 1337, net);
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("breath", "grafik16", "grafik16")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }

  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 255, 0, 1000);

  int value = oldOutputValue + ((outputValue - oldOutputValue) * 0.1);
  oldOutputValue = value;
  
  analogWrite(RED_LED, constrain(value, 0, 255));

  if (value > 0 && millis() - lastTime > 500) {
    int value1 = constrain(map(value, 0, 150, 0, 1000), 0, 1000);
    client.publish("/input/breath/value1", String(value1));

    int value2 = constrain(map(value, 150, 300, 0, 1000), 0, 1000);
    client.publish("/input/breath/value2", String(value2));

    lastTime = millis();
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {}
