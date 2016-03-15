import mqtt.*;

MQTTClient client;

void setup() {
  size(800, 800);
  background(0);
  
  client = new MQTTClient(this);
  client.connect("mqtt://try:try@broker.shiftr.io", "tester");
}

void draw() {}

void mousePressed() {
  client.publish("/outputs/surface/value1", Integer.toString(int(map(mouseX, 0, width, 0, 1000))));
  client.publish("/outputs/surface/value2", Integer.toString(int(map(mouseY, 0, height, 0, 1000))));
}

void messageReceived(String topic, byte[] payload) {}