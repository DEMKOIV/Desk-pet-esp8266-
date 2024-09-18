#include <Arduino.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#include <FluxGarage_RoboEyes.h>

roboEyes roboEyes; // create RoboEyes instance

#define b1_pin 13
#define b2_pin 0
#define delayy 399

bool b1;
bool b2;

unsigned long del;

byte mood = 0;
bool anim = 0;

unsigned char moods[] = {DEFAULT, TIRED, ANGRY, SQUINT, HAPPY};

void setup() {
  Serial.begin(9600);

  // Startup OLED Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Startup robo eyes
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100); // screen-width, screen-height, max framerate

  // Define some automated eyes behaviour
  roboEyes.setAutoblinker(ON, 3, 3); // Start auto blinker animation cycle -> bool active, int interval, int variation -> turn on/off, set interval between each blink in full seconds, set range for random interval variation in full seconds
  roboEyes.setIdleMode(ON, 2, 2); // Start idle animation cycle (eyes looking in random directions) -> turn on/off, set interval between each eye repositioning in full seconds, set range for random time interval variation in full seconds
  
  // Define eye shapes, all values in pixels
  roboEyes.setWidth(25, 25); // byte leftEye, byte rightEye
  roboEyes.setHeight(30, 30); // byte leftEye, byte rightEye
  roboEyes.setBorderradius(5, 5); // byte leftEye, byte rightEye
  roboEyes.setSpacebetween(25); // int space -> can also be negative

  // Cyclops mode
  //roboEyes.setCyclops(ON); // bool on/off -> if turned on, robot has only on eye

  // Define mood, curiosity and position
  //roboEyes.setMood(DEFAULT); // mood expressions, can be TIRED, ANGRY, HAPPY, DEFAULT
  //roboEyes.setPosition(DEFAULT); // cardinal directions, can be N, NE, E, SE, S, SW, W, NW, DEFAULT (default = horizontally and vertically centered)
  //roboEyes.setCuriosity(ON); // bool on/off -> when turned on, height of the outer eyes increases when moving to the very left or very right

  // Set horizontal or vertical flickering
  //roboEyes.setHFlicker(ON, 2); // bool on/off, byte amplitude -> horizontal flicker: alternately displacing the eyes in the defined amplitude in pixels
  //roboEyes.setVFlicker(ON, 2); // bool on/off, byte amplitude -> vertical flicker: alternately displacing the eyes in the defined amplitude in pixels

  // Play prebuilt oneshot animations
  //roboEyes.anim_confused(); // confused - eyes shaking left and right
  //roboEyes.anim_laugh(); // laughing - eyes shaking up and down
  
  pinMode(b1_pin, INPUT_PULLUP);
  pinMode(b2_pin, INPUT_PULLUP);

} 


void loop() {
  roboEyes.update(); // update eyes drawings

  b1 = !digitalRead(b1_pin);
  b2 = !digitalRead(b2_pin);

  if(b1 == 1 && millis() - del >= delayy){
    mood++;
    del = millis();
  }

  if(mood >= sizeof(moods)){
    mood = 0;
  }

  if(b2 == 1 && millis() - del >= delayy){

    if(!anim){
      roboEyes.anim_confused();
    }
    
    if(anim){
      roboEyes.anim_laugh();
    }
    anim = !anim;
    del = millis();
  }

  roboEyes.setMood(moods[mood]);

 // Dont' use delay() here in order to ensure fluid eyes animations.
 // Check the AnimationSequences example for common practices.
}