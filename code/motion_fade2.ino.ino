

#define PIR_PIN 2
#define LED_PIN 9
#define FADE_STEP 1
#define FADE_DELAY 40
#define FADE_STOP 0
#define FADE_UP 1
#define FADE_DOWN 2

int delayTime = 0;
int brightness = 0;
int fadeMode = FADE_UP;
int readState = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  analogWrite(LED_PIN, 0);

  // configure timer0 for 2kHz interrutps
  cli();
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;
  OCR0A = 124; // set compare match register
  TCCR0A |= (1 << WGM01); // turn on CTC mode
  TCCR0B |= (1 << CS01) | (1 << CS00); // Set CS01 and CS00 bits for 64 prescaler
  TIMSK0 |= (1<< OCIE0A); // enable timer compare interrupt
  sei();
}


ISR(TIMER0_COMPA_vect) {
  delayTime += 2; // 2ms between interrupts
  if (delayTime >= FADE_DELAY) {
    delayTime = 0;
    if (fadeMode != FADE_STOP) {
      adjustBrightness();
    }
  }
}


void adjustBrightness() {
  if (fadeMode == FADE_UP) {
    brightness +=  FADE_STEP;
  }
  else if (fadeMode == FADE_DOWN) {
    brightness -= FADE_STEP;
  }
  else {
    return;
  }

  if (brightness >= 255) {
    brightness = 255;
    fadeMode = FADE_STOP;
  }
  else if (brightness <= 0) {
    brightness = 0;
    fadeMode = FADE_STOP;
  }
  analogWrite(LED_PIN, brightness);
}


// the loop function runs over and over again forever
void loop() {
  readState = digitalRead(PIR_PIN);
  if (readState == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
    fadeMode = FADE_UP;
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    fadeMode = FADE_DOWN;
  }
  delay(250);
}

