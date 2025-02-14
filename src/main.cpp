// https://github.com/mcvladoc33/Clock_u8g2/tree/main
// https://wokwi.com/projects/419456946279552001

#include <Arduino.h>

#include <U8g2lib.h>
#include <Bounce2.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

Bounce up = Bounce();
Bounce down = Bounce();
Bounce ex = Bounce();
Bounce ok = Bounce();

#define UP 2
#define DOWN 3
#define EX 4
#define OK 5

#define ITEMS 6

#define SLEEP 10000

#define SNOWFLAKES 30

struct Snowflake {
  int x, y;
};

Snowflake snowflakes[SNOWFLAKES];

// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

void printPointer(uint8_t ptr);
void clock_settings();
void sleep();
void hello();
void new_year();
void drawSnowflakes();
void thanks();

void clock_tick();
// void sleep2();

// Sleep
unsigned long cur_sleep = 0;
unsigned long last_sleep = 0;

// Clock
unsigned long cur_milllis = 0;
unsigned long last_tick = 0;

unsigned int hour = 0;
unsigned int min = 0;
unsigned int sec = 0;

void setup(void) {
  Serial.begin(9600);

  up.attach( UP,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  up.interval(5); // interval in ms
  down.attach( DOWN,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  down.interval(5); // interval in ms
  ex.attach( EX,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  ex.interval(5); // interval in ms
  ok.attach( OK,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  ok.interval(5); // interval in ms


  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setDrawColor(2);
  u8g2.setFont(u8g2_font_6x10_tf);
  //u8g2.setFont(u8g2_font_3x5im_mr);
  //u8g2.setFont(u8x8_font_pxplusibmcgathin_f);

}

void loop(void) {
  static uint8_t pointer = 1;
  up.update();
  down.update();
  ex.update();
  ok.update();

  if (up.fell()){
    pointer = constrain (pointer - 1, 1, ITEMS);
    last_sleep = millis();  // Reset the sleep timer
  }

  if (down.fell()){
    pointer = constrain (pointer + 1, 1, ITEMS);
    last_sleep = millis();  // Reset the sleep timer
  }

  if (ok.fell()){
    switch (pointer){
      case 1: clock_settings(); break;
      case 2: hello(); break;
      case 3: new_year(); break;
      case 4: drawSnowflakes(); break;
      case 5: thanks(); break;
      case 6: break;
    }
    // Reset sleep timer after returning from a function
    last_sleep = millis();  // Reset the sleep timer
  }

  // Reset the sleep timer when any button is pressed
  if (up.fell() || down.fell() || ex.fell() || ok.fell()) {
    last_sleep = millis();  // Reset the sleep timer
  }

  // Sleep check: Only enter sleep mode if x seconds have passed with no button press
  cur_sleep = millis();
  if (cur_sleep - last_sleep >= SLEEP) {
    sleep();
    last_sleep = millis();  // Reset the sleep timer
  }

  // picture loop  
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(F(" 1. Clock settings \n"));
  u8g2.setCursor(0, 20);
  u8g2.print(F(" 2. Hello, world! \n"));
  u8g2.setCursor(0, 30);
  u8g2.print(F(" 3. Happy New Year!!! \n"));
  u8g2.setCursor(0, 40);
  u8g2.print(F(" 4. Snow * \n"));
  u8g2.setCursor(0, 50);
  u8g2.print(F(" 5. Thanks \n"));
  u8g2.setCursor(0, 60);
  u8g2.print(F(" 6. Parametr 6 \n"));
  printPointer (pointer);
  u8g2.sendBuffer();

  clock_tick();

}

void printPointer(uint8_t ptr){
  u8g2.setCursor(0, ptr * 10);
  u8g2.print(">");
}

void clock_tick(){
    //Clock
  cur_milllis = millis();
  if(cur_milllis - last_tick >= 1000){
    sec = sec + 1;
    if(sec >= 60){
      min = min +1;
      sec = 0;
    }
    if(min >= 60){
      hour = hour +1;
      min = 0;
    }
    if(hour >= 24){
      hour = 0;
    }
    last_tick = cur_milllis;
  }
}

void clock_settings(){

  while (1){
    clock_tick();

    up.update();
    down.update();
    ex.update();
    ok.update();
    u8g2.setFont(u8g2_font_timR24_tr);

    // Update display
    u8g2.clearBuffer();
    u8g2.setCursor(7, 40);
    //u8g2.print(F("Time: "));
    
    // Output in format HH:MM:SS
    u8g2.print(hour < 10 ? "0" : ""); u8g2.print(hour);
    u8g2.print(F(":"));
    u8g2.print(min < 10 ? "0" : ""); u8g2.print(min);
    u8g2.print(F(":"));
    u8g2.print(sec < 10 ? "0" : ""); u8g2.print(sec);

    u8g2.sendBuffer();

    if (up.fell()){
      hour= hour + 1;
    }

    if (down.fell()){
      min= min + 1;
    }

    if (ex.fell()){
      u8g2.setFont(u8g2_font_6x10_tf);
      return;  // Exit from function by pressed EX button
    }

    if (ok.fell()){
      sec = 0;
    }
  }
}

void hello(){
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(F("Hello, world! \n"));
  u8g2.sendBuffer();
  while (1){
    clock_tick();

    ex.update();
    if (ex.fell()){
      return; // Exit from function by pressed EX button
    }
  }
}

void new_year(){
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(F("Happy New Year!!! \n"));
  u8g2.sendBuffer();
  while (1){
    clock_tick();

    ex.update();
    if (ex.fell()){
      return;
    }
  }
}

void drawSnowflakes() {
  // Initialize snowflakes at random positions
  for (int i = 0; i < SNOWFLAKES; i++) {
    snowflakes[i].x = random(0, 128);
    snowflakes[i].y = random(0, 64);
  }
  while (1){
    clock_tick();

    ex.update();
    u8g2.clearBuffer();
    u8g2.setCursor(0, 0);

    // Draw each snowflake
    for (int i = 0; i < SNOWFLAKES; i++) {

      u8g2.drawPixel(snowflakes[i].x, snowflakes[i].y);

      // u8g2.setCursor(snowflakes[i].x, snowflakes[i].y);
      // u8g2.print('*');

    }

    // Update the positions of the snowflakes
    for (int i = 0; i < SNOWFLAKES; i++) {
      snowflakes[i].y += 1;  // Move snowflake down
      if (snowflakes[i].y >= 64) {  // If it reaches the bottom, reset to top
        snowflakes[i].y = 0;
        snowflakes[i].x = random(0, 128);  // Random horizontal position
      }
    }
    u8g2.sendBuffer();
      if (ex.fell()){
        return; // Exit from function by pressed EX button
      }
  }
}

//Thanks to the Ukrainian military for protecting us
void thanks(){
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print(F("Thanks to the \n"));
  u8g2.setCursor(0, 20);
  u8g2.print(F("Ukrainian military \n"));
  u8g2.setCursor(0, 30);
  u8g2.print(F("for protecting us \n"));
  u8g2.setCursor(0, 40);
  u8g2.print(F("Happy New Year!!! \n"));
  u8g2.sendBuffer();
  while (1){
    clock_tick();

    ex.update();
    if (ex.fell()){
      return; // Exit from function by pressed EX button
    }
  }
}

void sleep(){
  while (1){
    clock_tick();

    up.update();
    down.update();
    ex.update();
    ok.update();
    u8g2.setFont(u8g2_font_timR24_tr);

    // Update display
    u8g2.clearBuffer();
    u8g2.setCursor(7, 40);
    //u8g2.print(F("Time: "));
    
    // Output in format HH:MM:SS
    u8g2.print(hour < 10 ? "0" : ""); u8g2.print(hour);
    u8g2.print(F(":"));
    u8g2.print(min < 10 ? "0" : ""); u8g2.print(min);
    u8g2.print(F(":"));
    u8g2.print(sec < 10 ? "0" : ""); u8g2.print(sec);

    u8g2.sendBuffer();

    // If any button is pressed, wake up from sleep
    if (up.fell() || down.fell() || ex.fell() || ok.fell()) {
      u8g2.setFont(u8g2_font_6x10_tf);  // Set the font
      last_sleep = millis();  // Reset the sleep timer
      return;  // Exit sleep mode
    }

  }
}
