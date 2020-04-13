#include "mbed.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>



Serial pc(USBTX, USBRX);

DigitalOut gpo(D0);
DigitalOut led(LED_RED);


// I2C Communication
I2C i2c_lcd(I2C_SDA, I2C_SCL); // SDA, SCL




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

/*********************************************************************
I change the adafruit SSD1306 to SH1106

SH1106 driver don't provide several functions such as scroll commands.

*********************************************************************/


#define OLED_RESET 4
Adafruit_SH1106 display;

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

static const unsigned char logo16_glcd_bmp[] =
{
  0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++)
  {
    icons[f][XPOS] = rand() % display.width();
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = (rand() % 5) + 1;
    
    
    /*
    Serial.printf("x: ");
    Serial.printf(icons[f][XPOS], DEC);
    Serial.printf(" y: ");
    Serial.printf(icons[f][YPOS], DEC);
    Serial.printf(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
     */
  }

  while (1)
  {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) 
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
    }
    display.display();
    ThisThread::sleep_for(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++)
    {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height())
      {
         icons[f][XPOS] = rand() % display.width();
         icons[f][YPOS] = 0;
         icons[f][DELTAY] = (rand() % 5) + 1;
      }
    }
   }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++)
  {
    if (i == '\n')
       continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
       display.println(" ");
  }    
  display.display();
}

void testdrawcircle(void)
{
  for (int16_t i=0; i<display.height(); i+=2) 
  {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
  }
  ThisThread::sleep_for(250);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
  }
  ThisThread::sleep_for(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
  }
  ThisThread::sleep_for(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
    display.display();
  }
  ThisThread::sleep_for(250);
}

/*void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
 
  display.startscrollright(0x00, 0x0F);
  ThisThread::sleep_for(2000);
  display.stopscroll();
  ThisThread::sleep_for(1000);
  display.startscrollleft(0x00, 0x0F);
  ThisThread::sleep_for(2000);
  display.stopscroll();
  ThisThread::sleep_for(1000);    
  display.startscrolldiagright(0x00, 0x07);
  ThisThread::sleep_for(2000);
  display.startscrolldiagleft(0x00, 0x07);
  ThisThread::sleep_for(2000);
  display.stopscroll();
}*/


void setup()   
{                
   //Serial.begin(9600);

   // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
   display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
   // init done
  
   // Show image buffer on the display hardware.
   // Since the buffer is intialized with an Adafruit splashscreen
   // internally, this will display the splashscreen.
   display.display();
   ThisThread::sleep_for(2000);

  // Clear the buffer.
  display.clearDisplay();

  // draw a single pixel
  display.drawPixel(10, 10, WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  // draw many lines
  testdrawline();
  display.display();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  // draw rectangles
  testdrawrect();
  display.display();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.display();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  // draw mulitple circles
  testdrawcircle();
  display.display();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  // draw a white circle, 10 pixel radius
  display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
  display.display();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  testdrawroundrect();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  testfillroundrect();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  testdrawtriangle();
  ThisThread::sleep_for(2000);
  display.clearDisplay();
   
  testfilltriangle();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  ThisThread::sleep_for(2000);
  display.clearDisplay();

  // draw scrolling text
 /* testscrolltext();
  ThisThread::sleep_for(2000);
  display.clearDisplay();*/

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  //display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); 
  //display.println(0xDEADBEEF, HEX);
  display.display();
  ThisThread::sleep_for(2000);

  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();

  // invert the display
  display.invertDisplay(true);
  ThisThread::sleep_for(1000); 
  display.invertDisplay(false);
  ThisThread::sleep_for(1000); 

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}






/**************************************************
 * main
 **************************************************/
int main()
{
   int32_t count = 0;
   pc.printf("SSD1306 I2C Demo\r\n");
    
   while (true) 
   {
      pc.printf("Count: %d\r\n", count);

      setup();
      
      count++;
   }
}








