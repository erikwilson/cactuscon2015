/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
static Adafruit_SSD1306 display1(OLED_RESET);
static Adafruit_SSD1306 display2(OLED_RESET);
static Adafruit_PCD8544 display3(A3, 4, 5, 7, 8);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

static uint8_t buffer[128*64/8];

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
/*
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
*/
#if (SSD1306_LCDHEIGHT != 64)
#error(F("Height incorrect, please fix Adafruit_SSD1306.h!"));
#endif

char buf [100];
volatile byte pos;
volatile boolean process_it;
 
 
// SPI interrupt routine
ISR (SPI_STC_vect)
{
  Serial.println("In ISR!!");
byte c = SPDR;  // grab byte from SPI Data Register
  
  // add to buffer if room
  if (pos < sizeof buf)
    {
    buf [pos++] = c;
    
    // example: newline means time to process buffer
    if (c == '\n')
      process_it = true;
      
    }  // end of room available
}  // end of interrupt routine SPI_STC_vect
 
 /*
// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (process_it)
    {
    buf [pos] = 0;  
    Serial.println (buf);
    pos = 0;
    process_it = false;
    }  // end of flag set
    
}  // end of loop
*/

void setup()   {                
  Serial.begin(9600);
  Serial.println("start!");
  pinMode(6, OUTPUT);
  analogWrite(6, 128);

 /*
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;
 
  // now turn on interrupts
  SPI.attachInterrupt();
  delay(5000);
 */

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  
  display1.setBuffer(buffer);
  display2.setBuffer(buffer);
  display1.stopscroll();
  display2.stopscroll();
  display1.clearDisplay();
  display2.clearDisplay();
  display1.display();
  display2.display();

  display3.begin();
  display3.setBuffer(buffer);  
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display3.setContrast(55);

  display3.display(); // show splashscreen

  // text display tests
  display3.setTextSize(2);
  display3.setTextColor(PCD8544_BLACK);
  display3.setCursor(0, 0);
  display3.println(F("HELLO"));
  display3.setTextSize(1);
  display3.println(F("CACTUSCON"));
  display3.setTextColor(PCD8544_WHITE,PCD8544_BLACK); // 'inverted' text
  //display3.println(3.141592);
  //display3.setTextSize(2);
  display3.display();

/*
  delay(500);
  display3.println(F("OK1"));
  display3.display();
  delay(500);
  display3.println(F("OK2"));
  display3.display();
  delay(500);
  display3.println(F("OK3"));
  display3.display();
  delay(500);
  display3.println(F("OK4"));
  display3.display();
  delay(500);
  display3.println(F("OK5"));
  display3.display();
  delay(500);
  display3.println(F("OK6"));
  display3.display();


  //Serial.println("display begin");
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
//  display1.display();
//  display2.display();
  Serial.println(F("display display"));
  delay(2000);
  // Clear the buffer.
  */
  display1.clearDisplay();
  display2.clearDisplay();
/*
  display1.display();
  display2.display();
  
  // draw a single pixel
  display1.drawPixel(10, 10, SSD1306_WHITE);
  display2.drawPixel(10, 10, SSD1306_WHITE);
  Serial.println("drew single pixel");
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display1.display();
  display2.display();
  delay(2000);
  display1.clearDisplay();
  display2.clearDisplay();

  // draw many lines
  testdrawline();
  Serial.println("drew test line");
  display1.display();
  display2.display();
  delay(2000);
  display1.clearDisplay();
  display2.clearDisplay();

  // draw rectangles
  testdrawrect();
  Serial.println("drew test rectangle");
  display1.display();
  display2.display();
  delay(2000);
  display1.clearDisplay();
  display2.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display1.display();
  display2.display();
  delay(2000);
  display1.clearDisplay();
  display2.clearDisplay();

  // draw mulitple circles
  testdrawcircle();
  display1.display();
  display2.display();
  delay(2000);
  display1.clearDisplay();
  display2.clearDisplay();

  // draw a white circle, 10 pixel radius
  display1.fillCircle(display1.width()/2, display1.height()/2, 10, SSD1306_WHITE);
  display2.fillCircle(display2.width()/2, display2.height()/2, 10, SSD1306_WHITE);
  display1.display();
  display2.display();
  delay(2000);
  display1.clearDisplay();
  display2.clearDisplay();

  testdrawroundrect();
  delay(2000);
  display1.clearDisplay();
  display2.clearDisplay();

  testfillroundrect();
  delay(2000);
  display1.clearDisplay();
  display2.clearDisplay();
*/
/*
  Serial.println(F("displaying triangle..."));
  testdrawtriangle(true);
  delay(1000);
  
  bool inverted = true;
  for (int i=0; i<1; i++) {
    display1.invertDisplay(inverted);
    display2.invertDisplay(inverted);
    inverted = !inverted;
    delay(100);
  }
  
  //display1.clearDisplay();
  //display2.clearDisplay();
   
  delay(1000);
  Serial.println(F("displaying fill triangle..."));
  testfilltriangle();
  delay(1000);
  
  for (int i=0; i<1; i++) {
    display1.invertDisplay(inverted);
    display2.invertDisplay(inverted);
    inverted = !inverted;
    delay(100);
  }
  delay(1000);

  display1.clearDisplay();
  display2.clearDisplay();
*/
/*
  // draw the first ~12 characters in the font
  testdrawchar();
  display1.display();
  display2.display();
  delay(500);
  display1.clearDisplay();
  display2.clearDisplay();
*/
  // draw scrolling text

/*  
  Serial.println(F("displaying scroll text..."));
  testscrolltext();
  */
  /*
  delay(500);
  display1.clearDisplay();
  display2.clearDisplay();

  // text display tests
  display1.clearDisplay();
  display1.setTextSize(1);
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(0,0);
  display1.println("Hello, world 1!");
  display1.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // 'inverted' text
  display1.println(3.141592);
  display1.setTextSize(2);
  display1.setTextColor(SSD1306_WHITE);
  display1.print("0x"); display1.println(0x0DECAF, HEX);
  display1.display();

  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(SSD1306_WHITE);
  display2.setCursor(0,0);
  display2.println("Hello, world 2!");
  display2.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // 'inverted' text
  display2.println(3.141592);
  display2.setTextSize(2);
  display2.setTextColor(SSD1306_WHITE);
  display2.print("0x"); display2.println(0xC0FFEE, HEX);
  display2.display();
  delay(2000);

  // miniature bitmap display
  display1.clearDisplay();
  display2.clearDisplay();
  display1.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display2.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display1.display();
  display2.display();

  // invert the display
  display1.invertDisplay(true);
  display2.invertDisplay(true);
  delay(1000); 
  display1.invertDisplay(false);
  display2.invertDisplay(false);
  delay(1000); 

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
  */
  /*
  display1.clearDisplay();
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(10,10);
  display1.setTextSize(2.75);
  display1.println("Hello");
  display1.display();
  
  display2.clearDisplay();
  display2.setTextColor(SSD1306_WHITE);
  display2.setCursor(10,10);
  display2.setTextSize(2.75);
  display2.println("CactusCon");
  display2.display();
  */

}


bool inverted = true;

void loop() {
  for (int i = 0; i < 128; i++) {
    analogWrite(6, i);
    delay(15);
  }
    Serial.println(F("displaying triangle..."));
  testdrawtriangle(true);

  display1.invertDisplay(inverted);
  inverted = !inverted;
  delay(100);
  
  display1.clearDisplay();
  //display2.clearDisplay();
   
  delay(1000);
  Serial.println(F("displaying fill triangle..."));
  testfilltriangle();
  delay(1000);
  /*
  for (int i=0; i<1; i++) {
    display1.invertDisplay(inverted);
    display2.invertDisplay(inverted);
    inverted = !inverted;
    delay(100);
  }
  delay(1000);
*/
  display1.clearDisplay();
  Serial.println(F("displaying scroll text..."));
  testscrolltext();
  display1.clearDisplay();

  for (int i = 127; i >= 0; i--) {
    analogWrite(6, i);
    delay(15);
  }
}
/*

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons1[NUMFLAKES][3];
  uint8_t icons2[NUMFLAKES][3];
   
  uint8_t w1 = w;
  uint8_t h1 = h;
 
  uint8_t w2 = w;
  uint8_t h2 = h;

  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons1[f][XPOS] = random(display1.width());
    icons1[f][YPOS] = 0;
    icons1[f][DELTAY] = random(5) + 1;
    icons2[f][XPOS] = random(display2.width());
    icons2[f][YPOS] = 0;
    icons2[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons1[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons1[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons1[f][DELTAY], DEC);
    Serial.print("x: ");
    Serial.print(icons2[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons2[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons2[f][DELTAY], DEC);
  }

  while (1) {
    
    display1.clearDisplay();
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      // move it
      icons1[f][YPOS] += icons1[f][DELTAY];
      // if its gone, reinit
      if (icons1[f][YPOS] > display1.height()) {
	icons1[f][XPOS] = random(display1.width());
	icons1[f][YPOS] = 0;
	icons1[f][DELTAY] = random(5) + 1;
      }
    }

    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display1.drawBitmap(icons1[f][XPOS], icons1[f][YPOS], logo16_glcd_bmp, w, h, SSD1306_WHITE);
    }
    display1.display();


    display2.clearDisplay();
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      // move it
      icons2[f][YPOS] += icons2[f][DELTAY];
      // if its gone, reinit
      if (icons2[f][YPOS] > display2.height()) {
	icons2[f][XPOS] = random(display2.width());
	icons2[f][YPOS] = 0;
	icons2[f][DELTAY] = random(5) + 1;
      }
    }

    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display2.drawBitmap(icons2[f][XPOS], icons2[f][YPOS], logo16_glcd_bmp, w, h, SSD1306_WHITE);
    }
    display2.display();
    
    //delay(40);
  }  
}

*/
/*
void testdrawchar(void) {
  display1.setTextSize(1);
  display2.setTextSize(1);
  display1.setTextColor(SSD1306_WHITE);
  display2.setTextColor(SSD1306_WHITE);
  display1.setCursor(0,0);
  display2.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display1.write(i);
    display2.write(i);
    if ((i > 0) && (i % 21 == 0))
      display1.println();
      display2.println();
  }    
  display1.display();
  display2.display();
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display1.height(); i+=2) {
    display1.drawCircle(display1.width()/2, display1.height()/2, i, SSD1306_WHITE);
    display2.drawCircle(display2.width()/2, display2.height()/2, i, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display1.height()/2; i+=3) {
    // alternate colors
    display1.fillRect(i, i, display1.width()-i*2, display1.height()-i*2, color%2);
    display2.fillRect(i, i, display2.width()-i*2, display2.height()-i*2, color%2);
    display1.display();
    display2.display();
    color++;
  }
}
*/
void testdrawtriangle(bool dodisplay) {
  int width = display1.width()-1;
  int height = display1.height()-1;
  
  for (int16_t i=min(width,height)/2; i>0; i-=10) {
  //for (int16_t i=0; i<min(display1.width(),display1.height())/2; i+=5) {
    display1.drawTriangle(width/2, height/2-i/2,
                     width/2-i, height/2+i,
                     width/2+i, height/2+i, SSD1306_WHITE);

    if (dodisplay) {
      display1.display();
      display2.display();
      delay(100);
    }
  }
}

void testfilltriangle(void) {
  uint8_t color = SSD1306_WHITE;
  testdrawtriangle(false);
  int width = display1.width()-1;
  int height = display1.height()-1;

  //for (int16_t i=min(display1.width(),display1.height())/2; i>0; i-=5) {
  for (int16_t i=0; i<min(width,height)/2; i+=10) {
    //int16_t i=min(display1.width(),display1.height())/2;
    display1.fillTriangle(width/2, height/2-i,
                     width/2-i, height/2+i,
                     width/2+i, height/2+i, SSD1306_WHITE);
    if (color == SSD1306_WHITE) color = SSD1306_BLACK;
    else color = SSD1306_WHITE;
    display1.display();
    display2.display();
    delay(100);
  }
}
/*
void testdrawroundrect(void) {
  for (int16_t i=0; i<display1.height()/2-2; i+=2) {
    display1.drawRoundRect(i, i, display1.width()-2*i, display1.height()-2*i, display1.height()/4, SSD1306_WHITE);
    display2.drawRoundRect(i, i, display2.width()-2*i, display2.height()-2*i, display2.height()/4, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = SSD1306_WHITE;
  for (int16_t i=0; i<display1.height()/2-2; i+=2) {
    display1.fillRoundRect(i, i, display1.width()-2*i, display1.height()-2*i, display1.height()/4, color);
    display2.fillRoundRect(i, i, display2.width()-2*i, display2.height()-2*i, display2.height()/4, color);
    if (color == SSD1306_WHITE) color = SSD1306_BLACK;
    else color = SSD1306_WHITE;
    display1.display();
    display2.display();
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display1.height()/2; i+=2) {
    display1.drawRect(i, i, display1.width()-2*i, display1.height()-2*i, SSD1306_WHITE);
    display2.drawRect(i, i, display2.width()-2*i, display2.height()-2*i, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display1.width(); i+=4) {
    display1.drawLine(0, 0, i, display1.height()-1, SSD1306_WHITE);
    display2.drawLine(0, 0, i, display2.height()-1, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
  for (int16_t i=0; i<display1.height(); i+=4) {
    display1.drawLine(0, 0, display1.width()-1, i, SSD1306_WHITE);
    display2.drawLine(0, 0, display2.width()-1, i, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
  delay(250);
  
  display1.clearDisplay();
  display2.clearDisplay();
  for (int16_t i=0; i<display1.width(); i+=4) {
    display1.drawLine(0, display1.height()-1, i, 0, SSD1306_WHITE);
    display2.drawLine(0, display2.height()-1, i, 0, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
  for (int16_t i=display1.height()-1; i>=0; i-=4) {
    display1.drawLine(0, display1.height()-1, display1.width()-1, i, SSD1306_WHITE);
    display2.drawLine(0, display2.height()-1, display2.width()-1, i, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
  delay(250);
  
  display1.clearDisplay();
  display2.clearDisplay();
  for (int16_t i=display1.width()-1; i>=0; i-=4) {
    display1.drawLine(display1.width()-1, display1.height()-1, i, 0, SSD1306_WHITE);
    display2.drawLine(display2.width()-1, display2.height()-1, i, 0, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
  for (int16_t i=display1.height()-1; i>=0; i-=4) {
    display1.drawLine(display1.width()-1, display1.height()-1, 0, i, SSD1306_WHITE);
    display2.drawLine(display2.width()-1, display2.height()-1, 0, i, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
  delay(250);

  display1.clearDisplay();
  display2.clearDisplay();
  for (int16_t i=0; i<display1.height(); i+=4) {
    display1.drawLine(display1.width()-1, 0, 0, i, SSD1306_WHITE);
    display2.drawLine(display2.width()-1, 0, 0, i, SSD1306_WHITE);
    display1.display();
    display2.display();
  }
  for (int16_t i=0; i<display1.width(); i+=4) {
    display1.drawLine(display1.width()-1, 0, i, display1.height()-1, SSD1306_WHITE); 
    display2.drawLine(display2.width()-1, 0, i, display2.height()-1, SSD1306_WHITE); 
    display1.display();
    display2.display();
  }
  delay(250);
}
*/
void testscrolltext(void) {
  display1.setTextSize(2);
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(0,0);
  display1.clearDisplay();
  display1.println(F("Hello\nCactusCon"));
  display1.display();

  display1.startscrolldiagright(0xFF, 0xFF);
  delay(3000);
  display1.stopscroll();
}
