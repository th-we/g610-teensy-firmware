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

void loop() {
  for (int r = 0; r < ROW_COUNT; r++) {
    digitalWrite(r + ROW_MIN, LOW);
    for (int c = 0; c < COL_COUNT; c++) {
      boolean down = digitalRead(c + COL_MIN) == LOW;
      if (down != downState[r][c]) {
        int code = keymap[r][c];
        down ? Keyboard.press(code) : Keyboard.release(code);
        downState[r][c] = down;
      }
    }
    digitalWrite(r + ROW_MIN, HIGH);
    delay(1); // tiny delay to avoid false keypress
    // Without the delay, seemingly random keys fire.
    // Wild guess: This might be due to capacitance in the system.
    // This delay also seems enough to take care of debouncing,
    // at least with switches that are not worn out yet.
  }
}
