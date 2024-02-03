#include <KeyboardController.h>

// Atari 2600 joystick pinout - seen from the front
// \1 2 3 4 5/
//  \6 7 8 9/
//
// 1 - row 1, 2 - row 2, 3 - row 3, 4 - row 4, 5 - col 1
// 6 - col 3, 7 - 5V (HIGH), 8 - GND (LOW), 9 - col 2

#define OUT_COL_3 4 // connect digital pin 4 of Seeeduino to pin 5 of joystick port
#define OUT_COL_2 5
#define OUT_COL_1 10

#define IN_ROW_4 0
#define IN_ROW_3 1
#define IN_ROW_2 2
#define IN_ROW_1 3

int inputs[4] = { IN_ROW_1, IN_ROW_2, IN_ROW_3, IN_ROW_4 };
int outputs[3] = { OUT_COL_1, OUT_COL_2, OUT_COL_3 };

volatile bool key1 = false, 
              key2 = false, 
              key3 = false,
              key4 = false, 
              key5 = false,
              key6 = false,
              key7 = false,
              key8 = false,
              key9 = false,
              key0 = false,
              keyHash = false,
              keyAsterisk = false;

struct PortInit {
  PortInit() {        
    for(int i=0;i<3;++i)
    {
      pinMode(outputs[i], OUTPUT);
      digitalWrite(outputs[i], HIGH);
    }
    
    for(int i=0;i<4;++i)
      pinMode(inputs[i], INPUT);
  }
};

PortInit portInit;

USBHost usb;

KeyboardController keyboard(usb);

void handleKeyAction(bool pressed) {
  int oemCode = keyboard.getOemKey();

  if (oemCode == 30) // 1
    key1 = pressed;
  else if (oemCode == 31) // 2
    key2 = pressed;
  else if (oemCode == 32) // 3
    key3 = pressed;
  else if (oemCode == 20) // q
    key4 = pressed;
  else if (oemCode == 26) // w
    key5 = pressed;
  else if (oemCode == 8) // e
    key6 = pressed;
  else if (oemCode == 4) // a
    key7 = pressed;
  else if (oemCode == 22) // s
    key8 = pressed;
  else if (oemCode == 7) // d
    key9 = pressed;
  else if (oemCode == 27) // x
    key0 = pressed;
  else if (oemCode == 6) // c
    keyHash = pressed;
  else if (oemCode == 29) // z
    keyAsterisk = pressed;
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

  bool row1 = digitalRead(IN_ROW_1);
  bool row2 = digitalRead(IN_ROW_2);
  bool row3 = digitalRead(IN_ROW_3);
  bool row4 = digitalRead(IN_ROW_4);

  if (!row1)
  {
    digitalWrite(OUT_COL_1, keyHash ? LOW : HIGH);
    digitalWrite(OUT_COL_2, key0 ? LOW : HIGH);
    digitalWrite(OUT_COL_3, keyAsterisk ? LOW : HIGH);
  }
  else if(!row2)
  {
    digitalWrite(OUT_COL_1, key9 ? LOW : HIGH);
    digitalWrite(OUT_COL_2, key8 ? LOW : HIGH);
    digitalWrite(OUT_COL_3, key7 ? LOW : HIGH);
  }
  else if(!row3)
  {
    digitalWrite(OUT_COL_1, key6 ? LOW : HIGH);
    digitalWrite(OUT_COL_2, key5 ? LOW : HIGH);
    digitalWrite(OUT_COL_3, key4 ? LOW : HIGH);
  }
  else if(!row4)
  {
    digitalWrite(OUT_COL_1, key3 ? LOW : HIGH);
    digitalWrite(OUT_COL_2, key2 ? LOW : HIGH);
    digitalWrite(OUT_COL_3, key1 ? LOW : HIGH);
  }
}
