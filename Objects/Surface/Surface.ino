#include <Bridge.h>
#include <SoftwareSerial.h>
#include <YunClient.h>
#include <MQTTClient.h>

struct pole {
  int start;
  int end;
} poles[16] = {
  { 110, 245 },
  { 85, 245 },
  { 75, 245 },
  { 95, 245 },
  { 90, 245 },
  { 100, 245 },
  { 105, 255 },
  { 90, 245 },
  { 95, 245 },
  { 85, 245 },
  { 90, 245 },
  { 90, 245 },
  { 60, 245 },
  { 85, 245 },
  { 90, 245 },
  { 85, 245 }
};

YunClient net;
MQTTClient client;
SoftwareSerial polulu1(7, 8);
SoftwareSerial polulu2(10, 9);

int value1 = 0;
int value2 = 0;

void setup() {
  Bridge.begin();
  Serial.begin(9600);

  client.begin("broker.shiftr.io", net);
  polulu1.begin(9600);
  polulu2.begin(9600);
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("surface", "try", "try")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");

  client.subscribe("/output/surface/*");
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  if(topic.equals("/output/surface/value1")) {
    value1 = payload.toInt();
  } else if(topic.equals("/output/surface/value2")) {
    value2 = payload.toInt();
  }

  setPole();
}

void setPole() {
  int pole = map(value1, 0, 1000, 0, 16);

  for(int i=0; i<16; i++) {
    if(i <= pole) {
      setPosition(i, map(value2, 0, 1000, poles[i].start, poles[i].end));  
    } else {
      setPosition(i, poles[i].start);
    }
  }
}

void setPosition(int servo, int pos) {
  if(servo < 8) {
    polulu1.write(0xFF);
    polulu1.write((byte)servo);
    polulu1.write((byte)pos);  
  } else {
    polulu2.write(0xFF);
    polulu2.write((byte)(servo-8));
    polulu2.write((byte)pos);
  } 
}

