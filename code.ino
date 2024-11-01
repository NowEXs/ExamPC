const int BTN = 2;
const int LED = 3;

bool is_led_on = true;

void setup()
{
  pinMode(BTN, INPUT_PULLUP);  // Use internal pull-up resistor
  pinMode(LED, OUTPUT);
}

void loop()
{
  if (digitalRead(BTN) == LOW) {
    is_led_on = !is_led_on;
    delay(100);
  }
  
  if (is_led_on) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  delay(5);
}
// --------------
const int BTN = 2;
const int LED = 3;

bool is_led_on = true;

void setup()
{
  pinMode(BTN, INPUT);
  pinMode(LED, OUTPUT);
}

void loop()
{
  if (digitalRead(BTN) == HIGH) {
    is_led_on = !is_led_on;
    delay(100);
  }
  
  if (is_led_on) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  delay(5);
}
// --------------Color LED Switch:
const int BTN_RED = 4;
const int BTN_GREEN = 5;
const int BTN_BLUE = 6;

const int PIN_RED = 8;
const int PIN_GREEN = 9;
const int PIN_BLUE = 10;

bool is_red_on = false;
bool is_green_on = false;
bool is_blue_on = false;

void setup() {
  pinMode(BTN_RED, INPUT);
  pinMode(BTN_GREEN, INPUT);
  pinMode(BTN_BLUE, INPUT);
  
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
}

void loop() {
  change_stat(BTN_RED, PIN_RED, &is_red_on);
  change_stat(BTN_GREEN, PIN_GREEN, &is_green_on);
  change_stat(BTN_BLUE, PIN_BLUE, &is_blue_on);
  delay(5);
}

void change_stat(int btn_pin, int led_pin, bool* status) {
  if (digitalRead(btn_pin) == LOW) {
    *status = !(*status);
    if (*status) {
      digitalWrite(led_pin, HIGH);
    } else {
      digitalWrite(led_pin, LOW);
    }
    delay(100);
  }
}
//--------------- Analog LED Fading:
const int ledPin1 = 5;  // เลือก PWM Pin สำหรับ LED1
const int ledPin2 = 6; // เลือก PWM Pin สำหรับ LED2

int brightness1 = 0; // ความสว่างของ LED1
int brightness2 = 0; // ความสว่างของ LED2

void setup() {
  // กำหนด Pin เป็น Output
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  // Fading LED1 จาก Off ไปยัง Full-Bright
  analogWrite(ledPin1, brightness1);
  brightness1++;

  // Fading LED2 จาก Off ไปยัง Full-Bright เร็วเป็นสองเท่า
  analogWrite(ledPin2, brightness2);
  brightness2 += 2;

  // ตรวจสอบความสว่าง 
  if (brightness1 > 255) {
    brightness1 = 0;
  }
  if (brightness2 > 255) {
    brightness2 = 0;
  }

  delay(10); // หน่วงเวลาเล็กน้อยสำหรับการปรับความสว่าง
}