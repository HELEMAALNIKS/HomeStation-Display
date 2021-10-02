// Rotary Encoder Inputs
#define Clock 2   //Clock pin connected to D9
#define Data 0 //Data pin connected to D8
#define Push 10   //Push button pin connected to D10

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

int counter = 20;                    //Use this variable to store "steps"
int currentStateClock;              //Store the status of the clock pin (HIGH or LOW)
int lastStateClock;                 //Store the PREVIOUS status of the clock pin (HIGH or LOW)
String currentDir ="";              //Use this to print text 
unsigned long lastButtonPress = 0;  //Use this to store if the push button was pressed or not

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Welcome");

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 17);
  display.println("This is");
  display.println("a Splash");
  display.println("Screen");

  display.display();
  
  /*  Set encoder pins as inputs with pullups. If you use the Encoder Module, you don't need 
   *  pullups for Clock and Data, only for the push button.*/
  pinMode(Clock,INPUT_PULLUP);
  pinMode(Data,INPUT_PULLUP);
  pinMode(Push, INPUT_PULLUP);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of Clock pin (it could be HIGH or LOW)
  lastStateClock = digitalRead(Clock);
}

void loop() {
  
  // Read the current state of CLK
  currentStateClock = digitalRead(Clock);

  // If last and current state of Clock are different, then "pulse occurred"
  // React to only 1 state change to avoid double count
  if (currentStateClock != lastStateClock  && currentStateClock == 1){

    // If the Data state is different than the Clock state then
    // the encoder is rotating "CCW" so we decrement
    if (digitalRead(Data) != currentStateClock) {
      counter ++;
      currentDir ="Counterclockwise";
    } else {
      // Encoder is rotating CW so increment
      counter --;
      currentDir ="Clockwise";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);

    display.clearDisplay();
  
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Binnen "+(String)counter+".6");

    display.println("Buiten 16.5");

    display.display();

  }

  // We save last Clock state for next loop
  lastStateClock = currentStateClock;

  // Read the button state
  int btnState = digitalRead(Push);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
}
