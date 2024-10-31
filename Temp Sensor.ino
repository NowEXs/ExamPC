#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

const char WIFI_SSID[] = "NowEiEI";               // เปลี่ยนเป็น WiFi SSID ของคุณ
const char WIFI_PASSWORD[] = "knX569977";         // เปลี่ยนเป็น WiFi Password ของคุณ

const char MQTT_BROKER_ADDRESS[] = "mqtt-dashboard.com";  // เปลี่ยนเป็นที่อยู่ของ MQTT broker
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "66070285";         // ตั้งค่า Client ID ตามที่ต้องการ
const char MQTT_USERNAME[] = "";                   // ไม่ต้องใส่ถ้าไม่ต้องการ
const char MQTT_PASSWORD[] = "";                   // ไม่ต้องใส่ถ้าไม่ต้องการ

const char PUBLISH_TOPIC[] = "temperature/topic";  // ตั้งค่า Topic สำหรับ Publish

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

  // อ่านค่าจากเซนเซอร์อุณหภูมิ
  int sensorValue = analogRead(A0); // อ่านค่าเซนเซอร์
  float voltage = sensorValue * (5.0 / 1023.0); // แปลงค่า Analog เป็น Voltage
  float temperatureC = voltage * 100; // แปลง Voltage เป็น อุณหภูมิ (Celsius)
  
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" C");

  // Publish ค่าที่อ่านได้ไปยัง MQTT
  mqtt.beginPublish(PUBLISH_TOPIC, 10); // 10 = ความยาวของ Payload
  mqtt.print(temperatureC);
  mqtt.endPublish();
  Serial.print("Published Temperature: ");
  Serial.println(temperatureC);

  delay(1000); // หน่วงเวลา 1 วินาที
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
}
