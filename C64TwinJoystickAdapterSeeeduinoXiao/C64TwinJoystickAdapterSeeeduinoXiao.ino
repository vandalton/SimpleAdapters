#include <KeyboardController.h>

// Amiga joystick pinout - seen from the front
// \1 2 3 4 5/
//  \6 7 8 9/
//
// 1 - up, 2 - down, 3 - left, 4 - right, 5 - middle mouse button 6 - fire/left mouse button
// 7 - 5V (HIGH), 8 - GND (LOW), 9 - right mouse button

#define OUT_UP 0 // connect digital pin 0 of Seeeduino to pin 1 of joystick port
#define OUT_DOWN 1
#define OUT_LEFT 2
#define OUT_RIGHT 3
#define OUT_FIRE 10
#define OUT_2ND_UP 8
#define OUT_2ND_DOWN 7
#define OUT_2ND_LEFT 6
#define OUT_2ND_RIGHT 5
#define OUT_2ND_FIRE 4

int outputs[10] = {
  OUT_UP, OUT_DOWN, OUT_LEFT, OUT_RIGHT, OUT_FIRE,
  OUT_2ND_UP, OUT_2ND_DOWN, OUT_2ND_LEFT, OUT_2ND_RIGHT, OUT_2ND_FIRE
};

volatile bool up = false, 
              down = false, 
              left = false,
              right = false, 
              fire = false,
              up2nd = false,
              down2nd = false,
              left2nd = false,
              right2nd = false,
              fire2nd = false;

struct PortInit {
  PortInit() {        
    for(int i=0;i<10;++i)
    {
      pinMode(outputs[i], OUTPUT);
      digitalWrite(outputs[i], HIGH);
    }
  }
};

PortInit portInit;

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
  else if (oemCode == 44 || oemCode == 29 || oemCode == 53 || oemCode == 43) // space or 'z' or tilde or tab
    fire = pressed;
  else if (oemCode == 91) // numeric 3
    right2nd = pressed;
  else if (oemCode == 89) // numeric 1
    left2nd = pressed;
  else if (oemCode == 90) // numeric 2
    down2nd = pressed;
  else if (oemCode == 93) // numeric 5
    up2nd = pressed;
  else if (oemCode == 56) // '/' 
    fire2nd = pressed;
}

void keyPressed() {
  handleKeyAction(true);
}

void keyReleased() {
  handleKeyAction(false);
}

void setup() {   
  if (usb.Init() == -1) {
    while (1);
  }

  delay(20);
}

void loop() {
  usb.Task();

  digitalWrite(OUT_UP, up ? LOW : HIGH);
  digitalWrite(OUT_DOWN, down ? LOW : HIGH);
  digitalWrite(OUT_LEFT, left ? LOW : HIGH);
  digitalWrite(OUT_RIGHT, right ? LOW : HIGH);
  digitalWrite(OUT_FIRE, fire ? LOW : HIGH);

  digitalWrite(OUT_2ND_UP, up2nd ? LOW : HIGH);
  digitalWrite(OUT_2ND_DOWN, down2nd ? LOW : HIGH);
  digitalWrite(OUT_2ND_LEFT, left2nd ? LOW : HIGH);
  digitalWrite(OUT_2ND_RIGHT, right2nd ? LOW : HIGH);
  digitalWrite(OUT_2ND_FIRE, fire2nd ? LOW : HIGH);
}
