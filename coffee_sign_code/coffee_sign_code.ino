#define CD4017_CLK        14
#define POT_DDR           DDRC
#define POT_PORT          PORTC
#define POT_PIN           4
#define BUTTON_PORT       PORTD
#define BUTTON_PORT_DDR   DDRD
#define BUTTON_PIN        7

// transistor control pins 
#define COFF_PIN          2
#define CUP_PIN           3
#define FLAME_PIN         4

unsigned long long last_button_press = 0; 
unsigned long current_button_press = 0;
uint16_t debounce_interval = 50;
int current_button_state = 0;
int button_reading;

/**
* Init GPIO
*/
void initGPIO() {
  // init pins as input
  BUTTON_PORT_DDR &= ~(1<<BUTTON_PIN);

  // enable pull up resistor
  BUTTON_PORT |= (1 << BUTTON_PIN); 

  // output pins
  DDRD |= (1 << COFF_PIN);
  DDRD |= (1 << CUP_PIN);
  DDRD |= (1 << FLAME_PIN);
  
}

/*
* Read push button
*/
void readPushButton() {
  // this button goes LOW when pressed
  if (!(PIND & (1 << BUTTON_PIN))) {
    button_reading = 0; // button presse
  } else {
    button_reading = 1;
  }

  // debounce
  current_button_press = millis();
  if(button_reading == 0) {
    // wait
    if(current_button_press - last_button_press > debounce_interval) {
      if(button_reading == 0) {
        current_button_state = 0;
      }

      current_button_press = last_button_press;
    }
  }
}

void coffee_sign_blink_ms(unsigned long delay){
  PORTD |= (1 << COFF_PIN);
  delay(delay);
  PORTD &= ~(1 << COFF_PIN);
  delay(delay);
}

void cup_sign_blink_ms(unsigned long delay){
  PORTD |= (1<<CUP_PIN);
  delay(delay);
  PORTD &= ~(1<<CUP_PIN);
  delay(delay);
}

void flame_sign_blink_ms(unsigned long delay){
  PORTD |= (1 << FLAME_PIN);
  delay(delay);
  PORTD &= ~(1 << FLAME_PIN);
  delay(delay);
}

void setup() {
  initGPIO();
  Serial.begin(9600);
}

void loop() {
  readPushButton();
  blink();
  
  // process button press
  if(current_button_press == 0) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button not pressed");
  }


}
