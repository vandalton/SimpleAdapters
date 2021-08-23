#include <KeyboardController.h>

// Amiga joystick pinout - seen from the front
// \1 2 3 4 5/
//  \6 7 8 9/
//
// 1 - up, 2 - down, 3 - left, 4 - right, 6 - fire
// 7 - 5V (HIGH), 8 - GND (LOW)

#define OUT_UP 2 // connect digital pin 2 of Seeeduino to pin 1 of joystick port
#define OUT_DOWN 3
#define OUT_LEFT 4
#define OUT_RIGHT 5
#define OUT_FIRE 6

int outputs[10] = {OUT_UP, OUT_DOWN, OUT_LEFT, OUT_RIGHT, OUT_FIRE};

volatile bool up = false, 
              down = false, 
              left = false,
              right = false, 
              fire = false;

USBHost usb;

KeyboardController keyboard(usb);

void handleKeyAction(bool pressed) {
  int oemCode = keyboard.getOemKey();

  if (oemCode == 79 || oemCode == 7) // right arrow or 'd'
    right = pressed;
  else if (oemCode == 80 || oemCode == 4) // left arrow or 'a'
    left = pressed;
  else if (oemCode == 81 || oemCode == 22) // down arrow or 's'
    down = pressed;
  else if (oemCode == 82 || oemCode == 26) // up arrow or 'w'
    up = pressed;
  else if (oemCode == 44)
    fire = pressed;
}

void keyPressed() {
  handleKeyAction(true);
}

void keyReleased() {
  handleKeyAction(false);
}

void setup() {  
  for(int i=0;i<5;++i)
  {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i], HIGH);
  }
  
  if (usb.Init() == -1) {
    while (1);
  }
}

void loop() {
  usb.Task();
    
  digitalWrite(OUT_UP, up ? LOW : HIGH);
  digitalWrite(OUT_DOWN, down ? LOW : HIGH);
  digitalWrite(OUT_LEFT, left ? LOW : HIGH);
  digitalWrite(OUT_RIGHT, right ? LOW : HIGH);
  digitalWrite(OUT_FIRE, fire ? LOW : HIGH);
}
