#include <Bridge.h>
#include <SoftwareSerial.h>
#include <YunClient.h>
#include <MQTTClient.h>

struct arm {
  int start;
  int end;
} arms[3] = {
  { 1500, 3900 },
  { 1600, 4100 },
  { 1600, 3850 }
};

YunClient net;
MQTTClient client;
SoftwareSerial servoController(6, 7);

int value1 = 0;
int value2 = 0;
 
void setup() {
  Bridge.begin();
  Serial.begin(9600);
  
  client.begin("broker.shiftr.io", net);
  servoController.begin(9600);
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("scissomat", "try", "try")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");

  client.subscribe("/output/scissomat/*");
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  if(topic.equals("/output/scissomat/value1")) {
    value1 = payload.toInt();
  } else if(topic.equals("/output/scissomat/value2")) {
    value2 = payload.toInt();
  }

  setMachine();
}

void setMachine() {
  int arms = map(value1, 0, 1000, 0, 3);

  for(int i=0; i<3; i++) {
    if(i <= arms) {
      setArm(i, value2);  
    } else {
      setArm(i, 0);
    }
  }
}

void setArm(int arm, int pos) {
  int value = map(pos, 0, 1000, arms[arm].start, arms[arm].end);
  setPosition(arm, value);
}

void setPosition(int servo, int pos) {
  byte data2 = pos & B01111111; // calculate second data byte 
  byte data1 = pos >> 7; // calculate first data byte

  servoController.write(0x80); // write start byte
  servoController.write(0x01); // write device id
  servoController.write(0x04); // write absolute position command
  servoController.write((byte)servo); // write servo number
  servoController.write(data1); // write first data byte
  servoController.write(data2); // write second data byte
}
