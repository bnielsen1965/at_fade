

#define PIR_PIN 1
#define LED_PIN 0
#define SIG_PIN 2
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
//  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(SIG_PIN, OUTPUT);
  digitalWrite(SIG_PIN, LOW);
//  digitalWrite(LED_BUILTIN, LOW);
  analogWrite(LED_PIN, 0);

  cli();

  // COM0A inverted, WGM fast PWM
  TCCR0A |= (1<<COM0A1)|(1<<COM0A0)|(1<<WGM01)|(1<<WGM00);

  // CS prescaler 64, f_pwm = f_clk/(N * 256)
  TCCR0B |= (1<<CS01)|(1<<CS00);

  // enable Timer/Counter0 output compare match A interrupt
  TIMSK |= (1<<OCIE0A);


  
////  // Timer/Counter Compare Register set PB0 to inverted output and fast PWM mode
///  TCCR0A = 3<<COM0A0 | 3<<WGM00;
  
  // configure timer0 for 2kHz interrutps
//  cli();
//  TCCR0A = 0;
//  TCCR0B = 0;
//  TCNT0 = 0;
//  OCR0A = 124; // set compare match register
//  TCCR0A |= (1 << WGM01); // turn on CTC mode
//  TCCR0B |= (1 << CS01) | (1 << CS00); // Set CS01 and CS00 bits for 64 prescaler


////  TIMSK |= (1<< OCIE0A); // enable timer compare interrupt

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
    digitalWrite(SIG_PIN, HIGH);
    fadeMode = FADE_UP;
  }
  else {
    digitalWrite(SIG_PIN, LOW);
    fadeMode = FADE_DOWN;
  }
  delay(250);
}

