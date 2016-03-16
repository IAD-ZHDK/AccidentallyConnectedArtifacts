#include <Bridge.h>
#include <YunClient.h>
#include <MQTTClient.h>

#define VIB_MOTOR_1 3
#define VIB_MOTOR_2 5
#define VIB_MOTOR_3 6
#define VIB_MOTOR_4 9
#define VIB_MOTOR_5 10
#define VIB_MOTOR_6 11

YunClient net;
MQTTClient client;

int strength;
int location;

void setup()
{
  Bridge.begin();
  Serial.begin(9600);
  client.begin("192.168.1.183", 1337, net);
  
  pinMode(VIB_MOTOR_1, OUTPUT);
  pinMode(VIB_MOTOR_2, OUTPUT);
  pinMode(VIB_MOTOR_3, OUTPUT);
  pinMode(VIB_MOTOR_4, OUTPUT);
  pinMode(VIB_MOTOR_5, OUTPUT);
  pinMode(VIB_MOTOR_6, OUTPUT);
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("vest", "grafik16", "grafik16")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");

  client.subscribe("/output/vest/*");
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  if(topic.equals("/output/vest/value1")){
    location = payload.toInt();
  } else if(topic.equals("/output/vest/value2")){
    strength = map(payload.toInt(), 0, 1000, 0, 255);
  }

  if (location <= 167) {
    analogWrite(VIB_MOTOR_1, strength);
    analogWrite(VIB_MOTOR_2, 0);
    analogWrite(VIB_MOTOR_3, 0);
    analogWrite(VIB_MOTOR_4, 0);
    analogWrite(VIB_MOTOR_5, 0);
    analogWrite(VIB_MOTOR_6, 0);
  } else if (location > 167 && location <= 334) {
    analogWrite(VIB_MOTOR_1, 0);
    analogWrite(VIB_MOTOR_2, strength);
    analogWrite(VIB_MOTOR_3, 0);
    analogWrite(VIB_MOTOR_4, 0);
    analogWrite(VIB_MOTOR_5, 0);
    analogWrite(VIB_MOTOR_6, 0);
  } else if (location > 334 && location <= 501) {
    analogWrite(VIB_MOTOR_1, 0);
    analogWrite(VIB_MOTOR_2, 0);
    analogWrite(VIB_MOTOR_3, strength);
    analogWrite(VIB_MOTOR_4, 0);
    analogWrite(VIB_MOTOR_5, 0);
    analogWrite(VIB_MOTOR_6, 0);
  } else if (location > 501 && location <= 668) {
    analogWrite(VIB_MOTOR_1, 0);
    analogWrite(VIB_MOTOR_2, 0);
    analogWrite(VIB_MOTOR_3, 0);
    analogWrite(VIB_MOTOR_4, strength);
    analogWrite(VIB_MOTOR_5, 0);
    analogWrite(VIB_MOTOR_6, 0);
  } else if (location > 668 && location <= 835) {
    analogWrite(VIB_MOTOR_1, 0);
    analogWrite(VIB_MOTOR_2, 0);
    analogWrite(VIB_MOTOR_3, 0);
    analogWrite(VIB_MOTOR_4, 0);
    analogWrite(VIB_MOTOR_5, strength);
    analogWrite(VIB_MOTOR_6, 0);
  } else if (location > 835) {
    analogWrite(VIB_MOTOR_1, 0);
    analogWrite(VIB_MOTOR_2, 0);
    analogWrite(VIB_MOTOR_3, 0);
    analogWrite(VIB_MOTOR_4, 0);
    analogWrite(VIB_MOTOR_5, 0);
    analogWrite(VIB_MOTOR_6, strength);
  }
}
