#ifndef __kbdrptparser_h_
#define __kbdrptparser_h_

class KbdRptParser {
  public:
    KbdRptParser();
    volatile bool up, down, left, right, fire, mouse_left, mouse_right, mouse_middle;
    void HandleKey(uint8_t key, bool down);
};

KbdRptParser::KbdRptParser() {
  up = down = left = right = fire = mouse_left = mouse_right = mouse_middle = false;
}

void KbdRptParser::HandleKey(uint8_t oemCode, bool pressed) {
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

#endif
