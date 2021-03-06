import mqtt.*;

MQTTClient client;

void setup() {
  size(600, 600);
  background(0);
  
  client = new MQTTClient(this);
  client.connect("mqtt://grafik16:grafik16@192.168.1.183:1337", "tester");
}

void draw() {}

void mousePressed() {
  client.publish("/output/scissomat/value1", Integer.toString(int(map(mouseX, 0, width, 0, 1000))));
  client.publish("/output/scissomat/value2", Integer.toString(int(map(mouseY, 0, height, 0, 1000))));
}

void keyPressed() {
  client.publish("/output/scissomat/value1", "0");
  client.publish("/output/scissomat/value2", "0");
}

void messageReceived(String topic, byte[] payload) {}