#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

const char WIFI_SSID[] = "2212";               // เปลี่ยนเป็น WiFi SSID ของคุณ
const char WIFI_PASSWORD[] = "tipchaya";         // เปลี่ยนเป็น WiFi Password ของคุณ

const char MQTT_BROKER_ADDRESS[] = "phycom.it.kmitl.ac.th";  // เปลี่ยนเป็นที่อยู่ของ MQTT broker
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "66070285";         // ตั้งค่า Client ID ตามที่ต้องการ
const char MQTT_USERNAME[] = "";                   // ไม่ต้องใส่ถ้าไม่ต้องการ
const char MQTT_PASSWORD[] = "";                   // ไม่ต้องใส่ถ้าไม่ต้องการ

const char PUBLISH_TOPIC[] = "phycom";  
const char SUBSCRIBE_TOPIC[] = "student_id/venus";  // ตั้งค่า Topic สำหรับ Subscribe

// กำหนดขา LED RGB
const int RED_PIN = 9;    // ขา PWM สำหรับสีแดง
const int GREEN_PIN = 10;  // ขา PWM สำหรับสีเขียว
const int BLUE_PIN = 11;   // ขา PWM สำหรับสีน้ำเงิน

WiFiClient network;
MqttClient mqtt(network);

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (!mqtt.connected()) {
    connectToMQTT();
  }
  mqtt.poll();
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi SSID: ");
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

void messageHandler(int messageSize) {
  String incomingTopic = mqtt.messageTopic();
  String payload = "";

  while (mqtt.available()) {
    payload += (char)mqtt.read();
  }
  
  Serial.print("Received from topic: ");
  Serial.print(incomingTopic);
  Serial.print(" | Payload: ");
  Serial.println(payload);
  
  // เปลี่ยนค่าจาก String เป็น Integer
  int value = payload.toInt();
  setLEDColor(value);  // เรียกฟังก์ชันเพื่อเปลี่ยนสี LED
}

void setLEDColor(int value) {
  if (value >= 36 && value <= 50) {
    digitalWrite(RED_PIN, HIGH);
  } else if (value >= 26 && value <= 35) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
  } else if (value >= 10 && value <= 25) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
  }
}