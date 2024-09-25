#ifndef binds_h
#define binds_h
#include <Arduino.h>


// массив значений нажатых клавиш
// https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h#L61
// [слой][строка][столбец]
KeyboardKeycode keyCodes[3][12][8] = {

  { // основной слой
    // таблица значений для левой руки (<)  
    {KEY_ESC,        KEY_1,             KEY_2,        KEY_3,        KEY_4,          KEY_5,     0,             0          },
    {KEY_TAB,        KEY_Q,             KEY_W,        KEY_E,        KEY_R,          KEY_T,     0,             0          },
    {KEY_LEFT_SHIFT, KEY_A,             KEY_S,        KEY_D,        KEY_F,          KEY_G,     0,             0          },
    {KEY_LEFT_CTRL,  KEY_Z,             KEY_X,        KEY_C,        KEY_V,          KEY_B,     KEY_RESERVED,  KEY_RESERVED },
    {0,              KEY_LEFT_WINDOWS,  KEY_LEFT_ALT, KEY_RESERVED, KEY_LEFT_SHIFT, KEY_SPACE, KEY_BACKSPACE, KEY_RESERVED },
    
    // таблица значений для правой руки (>)                                                                                                       
    {0,          KEY_6,        KEY_7,           KEY_8,         KEY_9,           KEY_0,          KEY_RESERVED,   KEY_RESERVED    },
    {0,          KEY_Y,        KEY_U,           KEY_I,         KEY_O,           KEY_P,          KEY_LEFT_BRACE, KEY_RIGHT_BRACE,},
    {0,          KEY_H,        KEY_J,           KEY_K,         KEY_L,           KEY_SEMICOLON,  KEY_QUOTE,      KEY_BACKSLASH   },
    {KEY_RIGHT,  KEY_N,        KEY_M,           KEY_COMMA,     KEY_PERIOD,      KEY_SLASH,      KEY_MINUS,      KEY_EQUAL       },
    {KEY_UP,     KEY_SPACE,    KEY_RIGHT_SHIFT, KEY_BACKSPACE, KEY_DELETE,      KEY_RESERVED,   KEY_RESERVED,   KEY_RESERVED    },
    {KEY_DOWN,   KEY_ENTER,    0,               0,             0,               0,              0,              0               },
    {KEY_LEFT,   KEY_ENTER,    0,               0,             0,               0,              0,              0               }
  },
  
  { // верхний слой
    // таблица значений для левой руки (<)                                   
    {KEY_RESERVED, KEY_F1,       KEY_F2,       KEY_F3,       KEY_F4,       KEY_F5,       0,            0},
    {KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, 0,            0},
    {KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, 0,            0},
    {KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {0,            KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    
    // таблица значений для правой руки (>)                                                                                                     
    {0,            KEY_F6,        KEY_F7,       KEY_F8,       KEY_F9,       KEY_F10,      KEY_RESERVED, KEY_RESERVED},
    {0,            KEY_RESERVED,  KEY_RESERVED, KEY_INSERT,   KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {0,            KEY_BACKSPACE, KEY_RESERVED, KEY_RESERVED, KEY_1,        KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {KEY_RESERVED, KEY_RESERVED,  0,            0,            0,            0,            0,            0           },
    {KEY_RESERVED, KEY_RESERVED,  0,            0,            0,            0,            0,            0           }
  },
  
  { // нижний слой
    // таблица значений для левой руки (<)                                   
    {KEY_RESERVED, KEY_F1,       KEY_F2,       KEY_F3,       KEY_F4,       KEY_F5,       0,            0},
    {KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_UP,       KEY_RESERVED, KEY_RESERVED, 0,            0},
    {KEY_RESERVED, KEY_RESERVED, KEY_LEFT,     KEY_DOWN,     KEY_RIGHT,    KEY_RESERVED, 0,            0},
    {KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {0,            KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    
    // таблица значений для правой руки (>)                                                                                                     
    {0,            KEY_F6,        KEY_F7,       KEY_F8,       KEY_F9,       KEY_F10,      KEY_RESERVED, KEY_RESERVED},
    {0,            KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {0,            KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED, KEY_2,        KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED, KEY_RESERVED},
    {KEY_RESERVED, KEY_RESERVED,  0,            0,            0,            0,            0,            0           },
    {KEY_RESERVED, KEY_RESERVED,  0,            0,            0,            0,            0,            0           }
  },
};

// Макросы клавиш
uint8_t keysMacros[3][12][8] = {// переделать на обьект содержащий (слой, событие (нажатие, отпускание), клавишу)

  {// основной слой
    // таблица значений для левой руки (<)  
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    
    // таблица значений для правой руки (>)                                                                                                       
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {// верхний слой
    // таблица значений для левой руки (<)  
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    
    // таблица значений для правой руки (>)                                                                                                              
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {//нижний слой
    // таблица значений для левой руки (<)  
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    
    // таблица значений для правой руки (>)                                                                                                              
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

// KEY_ERROR_UNDEFINED KEY_MENU  pause/break KEY_PAUSE 
// есть нигде не использующаяся клавиша, можно назнаить ее под макросы 

#endif