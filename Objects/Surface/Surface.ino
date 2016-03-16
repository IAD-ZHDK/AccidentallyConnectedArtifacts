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

  client.begin("192.168.1.183", 1337, net);
  polulu1.begin(9600);
  polulu2.begin(9600);
}

void connect() {
  Serial.print("connecting...");
  while (!client.connect("surface", "grafik16", "grafik16")) {
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

  setSurface();
}

int getIntensity(int pole, int x, int y) {
  int posX;
  int posY;

  if(pole >= 12) {
    posX = pole - 11;
    posY = 4;
  } else if(pole >= 8) {
    posX = pole - 7;
    posY = 3;
  } else if(pole >= 4) {
    posX = pole - 3;
    posY = 2;
  } else {
    posX = pole;
    posY = 1;
  }

  return distance(x, y, posX, posY);
}

void setSurface() {
  int x = map(value1, 0, 1000, 1, 4);
  int y = map(value2, 0, 1000, 1, 4);
  
  for(int i=0; i<16; i++) {
    setPosition(i, map(getIntensity(i, x, y), 0, 1000, poles[i].start, poles[i].end));
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

int distance(float x1, float y1, float x2, float y2) {  
    float diffx = x1 - x2;
    float diffy = y1 - y2;
    float diffx_sqr = square(diffx);
    float diffy_sqr = square(diffy);
    float distance = sqrt(diffx_sqr + diffy_sqr);
    return map(distance, 0, 4, 0, 1000);
}
