#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

const char WIFI_SSID[] = "2212";          // เปลี่ยนเป็น WiFi SSID ของคุณ
const char WIFI_PASSWORD[] = "tipchaya";    // เปลี่ยนเป็น WiFi password ของคุณ

const char MQTT_BROKER_ADDRESS[] = "phycom.it.kmitl.ac.th";  // เปลี่ยนเป็นที่อยู่ของ MQTT broker
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "66070285";    // ตั้งค่า Client ID ตามที่ต้องการ
const char MQTT_USERNAME[] = "";             // ไม่ต้องใส่ถ้าไม่ต้องการ
const char MQTT_PASSWORD[] = "";             // ไม่ต้องใส่ถ้าไม่ต้องการ

const char PUBLISH_TOPIC[] = "66070285";       // เปลี่ยนตามที่ต้องการ
const char SUBSCRIBE_TOPIC[] = "phycom";     // เปลี่ยนตามที่ต้องการ

const int POTENTIOMETER_PIN = A0;            // ขา Analog สำหรับ potentiometer
int lastPotValue = -1;                       // เก็บค่าของ potentiometer ก่อนหน้าเพื่อเปรียบเทียบ
const int THRESHOLD = 5;                     // ตั้งค่า threshold สำหรับการเปลี่ยนแปลง

WiFiClient network;
MqttClient mqtt(network);

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (!mqtt.connected()) {
    connectToMQTT();
  }
  mqtt.poll();

  // อ่านค่าจาก potentiometer และส่งไปยัง MQTT เมื่อมีการเปลี่ยนแปลงเกิน threshold
  int potValue = analogRead(POTENTIOMETER_PIN);
  if (abs(potValue - lastPotValue) >= THRESHOLD) {
    sendToMQTT(potValue);
    lastPotValue = potValue;
  }
}

void connectToWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(WIFI_SSID);
  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  Serial.print("Connecting to MQTT broker...");
  mqtt.setId(MQTT_CLIENT_ID);
  mqtt.setUsernamePassword(MQTT_USERNAME, MQTT_PASSWORD);

  if (!mqtt.connect(MQTT_BROKER_ADDRESS, MQTT_PORT)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqtt.connectError());
    delay(5000);
    return;
  }

  Serial.println("Connected to MQTT broker!");
  mqtt.subscribe(SUBSCRIBE_TOPIC);
}

void sendToMQTT(int potValue) {
  String message = String(potValue);
  mqtt.beginMessage(PUBLISH_TOPIC);
  mqtt.print(message);
  mqtt.endMessage();

  Serial.print("Published to topic: ");
  Serial.print(PUBLISH_TOPIC);
  Serial.print(" | Potentiometer Value: ");
  Serial.println(message);
}

void messageHandler(int messageSize) {
  String incomingTopic = mqtt.messageTopic();
  Serial.print("Received from topic: ");
  Serial.print(incomingTopic);

  Serial.print(" | Payload: ");
  while (mqtt.available()) {
    Serial.print((char)mqtt.read());
  }
  Serial.println();
}