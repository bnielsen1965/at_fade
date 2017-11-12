

#define PIR_PIN 2
#define LED_PIN 9
#define FADE_STEP 1
#define FADE_DELAY 20

int brightness = 0;
int readState = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  analogWrite(LED_PIN, 0);
  fade(FADE_STEP);
}



// the loop function runs over and over again forever
void loop() {
  readState = digitalRead(PIR_PIN);
  if (readState == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  if (readState == HIGH && brightness == 0) {
    fade(FADE_STEP);
  }
  else if (readState == LOW && brightness == 255) {
    fade(-1 * FADE_STEP);
  }
  
  delay(20);
}


void fade(int inc) {
  do {
    brightness += inc;
    if (brightness >= 255) {
      brightness = 255;
      digitalWrite(LED_PIN, HIGH);
      return;
    }
    else if (brightness <= 0) {
      brightness = 0;
      digitalWrite(LED_PIN, LOW);
      return;
    }
    analogWrite(LED_PIN, brightness);
    delay(FADE_DELAY);
  } while(brightness > 0 && brightness < 255);
}

