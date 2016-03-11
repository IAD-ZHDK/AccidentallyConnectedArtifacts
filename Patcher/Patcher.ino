/**
 * Accidentally Connected Artifacts
 * Patcher
 *
 * © 2015 Joël Gähwiler
 */

#define CALIBRATE false
#define CHANNELS 4
#define TOLERANCE 20

#include <Bridge.h>
#include <YunClient.h>
#include <MQTTClient.h>

YunClient net;
MQTTClient client;

int averages[CHANNELS] = { 1012, 886, 779, 653 };
int ch[CHANNELS] = {-1, -1, -1, -1};

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  client.begin("broker.shiftr.io", net);

  Serial.print("connecting...");
  while (!client.connect("patcher", "try", "try")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");
}

void loop() {
  client.loop();
  
  if(CALIBRATE) {
    for(int i=0; i<CHANNELS; i++) {
      Serial.print(i);
      Serial.print(":");
      Serial.print(readSelector(i));
      Serial.print(" ");
    }
    Serial.println();
  } else {
    for(int i=0; i<CHANNELS; i++) {
      int v = readChannel(i);
    
      if(ch[i] != v) {        
        Serial.print("channel: ");
        Serial.print(v + 1);
        Serial.print(" => ");
        Serial.println(i + 1);
        ch[i] = v;
        client.publish("update", String(v + 1) + " => " + String(i + 1)); 
      }
    }
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {}

int readSelector(int ch) {
  int v = 0;
  delay(5);
  v = analogRead(ch);
  delay(5);
  return v;
} 

int readChannel(int ch) {
  int v = readSelector(ch);

  for(int i=0; i<CHANNELS; i++) {
    if(v > averages[i] - TOLERANCE && v < averages[i] + TOLERANCE) {
      return i;
    }
  }

  return -1;
}
