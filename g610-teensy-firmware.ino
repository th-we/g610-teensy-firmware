#define LAYOUT_GERMAN
#define ROW_MIN 4
#define ROW_COUNT 9
#define COL_MIN 14
#define COL_COUNT 16
#include "keylayouts.h"

int keymap[ROW_COUNT][COL_COUNT] = {
    KEY_ESC      ,KEY_F1    ,KEY_F2           ,KEY_F3         ,KEY_F4         ,KEY_F5      ,KEY_F6        ,KEY_F7     ,KEY_F8              ,KEY_F9              ,KEY_F10             ,KEY_F11              ,KEY_F12              ,KEY_PRINTSCREEN     ,KEY_SCROLL_LOCK       ,KEY_PAUSE              ,
    0            ,0         ,KEY_TILDE        ,KEY_1          ,KEY_2          ,KEY_3       ,KEY_4         ,KEY_5      ,KEY_6               ,KEY_7               ,KEY_8               ,KEY_9                ,KEY_0                ,KEY_MINUS           ,KEY_EQUAL             ,0                      ,
    0            ,0         ,KEY_TAB          ,KEY_Q          ,KEY_W          ,KEY_E       ,KEY_R         ,KEY_T      ,KEY_Y               ,KEY_U               ,KEY_I               ,KEY_O                ,KEY_P                ,KEY_LEFT_BRACE      ,KEY_RIGHT_BRACE       ,0                      ,
    0            ,0         ,KEY_CAPS_LOCK    ,KEY_A          ,KEY_S          ,KEY_D       ,KEY_F         ,KEY_G      ,KEY_H               ,KEY_J               ,KEY_K               ,KEY_L                ,KEY_SEMICOLON        ,KEY_QUOTE           ,KEY_NON_US_NUM        ,KEY_ENTER              ,
    0            ,0         ,MODIFIERKEY_SHIFT,KEY_NON_US_BS  ,KEY_Z          ,KEY_X       ,KEY_C         ,KEY_V      ,KEY_B               ,KEY_N               ,KEY_M               ,KEY_COMMA            ,KEY_PERIOD           ,KEY_SLASH           ,0                     ,MODIFIERKEY_RIGHT_SHIFT,
    0            ,0         ,MODIFIERKEY_CTRL ,MODIFIERKEY_GUI,MODIFIERKEY_ALT,0           ,0             ,KEY_SPACE  ,0                   ,0                   ,0                   ,MODIFIERKEY_RIGHT_ALT,MODIFIERKEY_RIGHT_GUI,KEY_MENU            ,MODIFIERKEY_RIGHT_CTRL,KEY_LEFT               ,
    KEY_BACKSPACE,KEY_INSERT,KEY_HOME         ,KEY_PAGE_UP    ,KEY_NUM_LOCK   ,KEYPAD_SLASH,KEYPAD_ASTERIX,KEYPAD_PLUS,KEY_DELETE          ,KEY_END             ,KEY_PAGE_DOWN       ,KEYPAD_7             ,KEYPAD_8             ,KEYPAD_9            ,KEYPAD_4              ,KEYPAD_5               ,
    KEY_RIGHT    ,KEYPAD_0  ,KEYPAD_PERIOD    ,KEYPAD_ENTER   ,KEYPAD_1       ,KEYPAD_2    ,KEYPAD_6      ,KEYPAD_3   ,KEY_DOWN            ,KEY_UP              ,0                   ,0                    ,0                    ,0                   ,0                     ,0                      ,
    0            ,0         ,0                ,0              ,0              ,0           ,0             ,0          ,KEY_MEDIA_VOLUME_DEC,KEY_MEDIA_VOLUME_INC,KEY_MEDIA_PLAY_PAUSE,KEY_MEDIA_STOP       ,KEY_MEDIA_NEXT_TRACK ,KEY_MEDIA_PREV_TRACK,KEYPAD_MINUS          ,KEY_MEDIA_MUTE
};


void setup() {
  /*Serial.begin(38400);
  while (!Serial) {}
  Serial.println("Start");*/
  for (int r = 0; r < ROW_COUNT; r++) {
    pinMode(r + ROW_MIN, OUTPUT);
    for (int c = 0; c <= COL_COUNT; c++) {
      pinMode(c + COL_MIN, INPUT_PULLUP);
    }
  }
  // A tiny delay to avoid initial false keypresses
  delay(10);
}


boolean downState[ROW_COUNT][COL_COUNT] = {false};

#define CHORD_COOLDOWN 100
unsigned long chordCooldownStart = millis();
// Chord bitmasks for ASCII letter keys A-Z
unsigned long activeChord = 0;
int activeChordCode = 0;
unsigned long emergingChord = 0;
unsigned char chordKeyCount = 0;
unsigned lastChordKeycode = 0;
int lastKeycode = 0;


/**
 * @return keycode the chord is translated to
 */
int getChordKey() {
  if (chordKeyCount == 0) {
    return 0;
  }
  if (chordKeyCount == 1) {
    return lastChordKeycode;
  }

  switch (emergingChord) {
    case 1UL << (KEY_J - KEY_A) | 1UL << (KEY_I - KEY_A):
      return KEY_SLASH;
    default:
      return 0;
  }
}


static inline boolean chordCooledDown() {
  return abs(millis() - chordCooldownStart) >= CHORD_COOLDOWN;
}


void loop() {
  if (emergingChord != activeChord && chordCooledDown()) {
    if (activeChordCode != 0) {
      Keyboard.release(activeChordCode);
      // TODO: Reset key buffer
    }
    activeChordCode = getChordKeycode(chord);
    if (activeChordCode != 0) {
      Keyboard.press(activeChordCode);
    }
  }
  for (int r = 0; r < ROW_COUNT; r++) {
    digitalWrite(r + ROW_MIN, LOW);
    for (int c = 0; c < COL_COUNT; c++) {
      int code = keymap[r][c];
      boolean down = code && (digitalRead(c + COL_MIN) == LOW);
      if (down != downState[r][c]) {
        /*Serial.print(r);
        Serial.print(", ");
        Serial.print(c);
        Serial.print(down ? " down" : " up");
        Serial.println();*/
        if (code >= KEY_A && code <= KEY_Z) {
          // Toggle the key's bit in the chord mask
          emergingChord ^= 1UL << (code - KEY_A);
          chordCooldownStart = millis();
          chordKeyCount += (down ? 1 : -1);
          if (down) {
            // TODO: Change this to a key (ring?)buffer
            lastChordKeycode = code;
          }
        } else {
          down ? Keyboard.press(code) : Keyboard.release(code);
        }
        downState[r][c] = down;
      }
    }
    digitalWrite(r + ROW_MIN, HIGH);
    delay(1); // If we don't have a delay here, random keys fire constantly (unrelated to debouncing).
  }
}
