#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

const char WIFI_SSID[] = "NowEiEI";               // เปลี่ยนเป็น WiFi SSID ของคุณ
const char WIFI_PASSWORD[] = "knX569977";         // เปลี่ยนเป็น WiFi Password ของคุณ

const char MQTT_BROKER_ADDRESS[] = "mqtt-dashboard.com";  // เปลี่ยนเป็นที่อยู่ของ MQTT broker
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "66070285";         // ตั้งค่า Client ID ตามที่ต้องการ
const char MQTT_USERNAME[] = "";                   // ไม่ต้องใส่ถ้าไม่ต้องการ
const char MQTT_PASSWORD[] = "";                   // ไม่ต้องใส่ถ้าไม่ต้องการ

const char PUBLISH_TOPIC[] = "phycom";  
const char SUBSCRIBE_TOPIC[] = "student_id/venus";  // ตั้งค่า Topic สำหรับ Subscribe

// กำหนดขา Ultrasonic
const int trigPin = 12;  // ขา Trigger
const int echoPin = 13;  // ขา Echo

WiFiClient network;
MqttClient mqtt(network);

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  pinMode(trigPin, OUTPUT); // กำหนดขา trigPin เป็น Output
  pinMode(echoPin, INPUT);  // กำหนดขา echoPin เป็น Input
  
  connectToWiFi();
  connectToMQTT();
  mqtt.onMessage(messageHandler);  // ตั้งค่า messageHandler
}

void loop() {
  if (!mqtt.connected()) {
    connectToMQTT();
  }
  
  mqtt.poll();
  
  // อ่านค่าระยะทางจาก Ultrasonic
  long duration, distanceCm;
  
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin, return the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in cm
  distanceCm = duration * 0.034 / 2;

  // Print the distances to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  
  // Publish "off" if the distance is greater than 20 cm
  if (distanceCm > 20) {
    mqtt.beginPublish(PUBLISH_TOPIC, 2); // 2 = QoS level
    mqtt.print("off");
    mqtt.endPublish();
    Serial.println("Published: off");
  }

  delay(1000); // Wait 1 second before the next measurement
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
}

