
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

void setup() {

  delay(3000);  // Wait for computer to recognize USB

  Keyboard.begin();
  USB.begin();

  delay(2000);

  // Open Windows Run
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(200);
  Keyboard.releaseAll();

  delay(700);

  // Type website
  Keyboard.print("https://www.kitsatoz.com");

  delay(300);

  // Press Enter
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
}

void loop() {
}
