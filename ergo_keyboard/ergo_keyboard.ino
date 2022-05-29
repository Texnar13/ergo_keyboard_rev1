#include "HID-Project.h"

// --------------------------------------------------------------------------------
// ----------  ----------
// --------------------------------------------------------------------------------

// сдвиговые регистры вывода  
#define OUT_DATA  6
#define OUT_LATCH 8
#define OUT_CLOCK 9

// сдвиговый регистр ввода (<) левой клавиатуры
#define LEFT_SCAN_DATA 10
#define LEFT_SCAN_CLOCK 5
#define LEFT_SCAN_SHLATCH 7

// сдвиговый регистр ввода правой (>) клавиатуры
#define RIGHT_SCAN_DATA 4
#define RIGHT_SCAN_CLOCK 11
#define RIGHT_SCAN_SHLATCH 12


// координаты текущей кнопки в массиве
int8_t chousenKeyX = -1;// int8_t  1 байт  -128… 127
int8_t chousenKeyY = -1;

// массив с состояниями нажатых клавиш
boolean keysState[12][8]= {
  // таблица значений для левой руки (<)  
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  
  // таблица значений для правой руки (>)                                                                                                       
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false}
};

// массив значений нажатых клавиш
// https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h#L61
KeyboardKeycode keyCodes[12][8]= {
  // таблица значений для левой руки (<)  
  {KEY_ESC,        KEY_1,         KEY_2,        KEY_3,            KEY_4,     KEY_5,     KEY_RESERVED, KEY_RESERVED       },
  {KEY_TAB,        KEY_Q,         KEY_W,        KEY_E,            KEY_R,     KEY_T,     KEY_RESERVED, KEY_RESERVED       },
  {KEY_LEFT_SHIFT, KEY_A,         KEY_S,        KEY_D,            KEY_F,     KEY_G,     KEY_RESERVED, KEY_RESERVED       },
  {KEY_LEFT_CTRL,  KEY_Z,         KEY_X,        KEY_C,            KEY_V,     KEY_B,     KEY_DELETE,   KEY_DELETE         },
  {KEY_RESERVED,   KEY_RESERVED,  KEY_LEFT_ALT, KEY_LEFT_WINDOWS, KEY_SPACE, KEY_ENTER, KEY_INSERT,   KEY_INSERT         },
  
  // таблица значений для правой руки (>)                                                                                                       
  {KEY_RESERVED,   KEY_6,        KEY_7,        KEY_8,         KEY_9,           KEY_0,          KEY_RESERVED,   KEY_RESERVED    },
  {KEY_RESERVED,   KEY_Y,        KEY_U,        KEY_I,         KEY_O,           KEY_P,          KEY_LEFT_BRACE, KEY_RIGHT_BRACE,},
  {KEY_RESERVED,   KEY_H,        KEY_J,        KEY_K,         KEY_L,           KEY_SEMICOLON,  KEY_QUOTE,      KEY_BACKSLASH   },
  {KEY_RIGHT,      KEY_N,        KEY_M,        KEY_COMMA,     KEY_PERIOD,      KEY_SLASH,      KEY_MINUS,      KEY_EQUAL       },
  {KEY_UP,         KEY_SPACE,    KEY_ENTER,    KEY_BACKSPACE, KEY_RIGHT_SHIFT, KEY_DELETE,     KEY_RESERVED,   KEY_RESERVED    },
  {KEY_DOWN,       KEY_RESERVED, KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,    KEY_RESERVED,   KEY_RESERVED,   KEY_RESERVED    },
  {KEY_LEFT,       KEY_RESERVED, KEY_RESERVED, KEY_RESERVED,  KEY_RESERVED,    KEY_RESERVED,   KEY_RESERVED,   KEY_RESERVED    }
};


// KEY_ERROR_UNDEFINED KEY_MENU  pause/break KEY_PAUSE 
// есть нигде не использующаяся клавиша, можно назнаить ее под макросы 

// --------------------------------------------------------------------------------
// ----------  ----------
// --------------------------------------------------------------------------------

void setup() {
  // для регистра вывода 
  pinMode(OUT_DATA, OUTPUT);
  pinMode(OUT_LATCH, OUTPUT);
  pinMode(OUT_CLOCK, OUTPUT);
  digitalWrite(OUT_DATA, LOW);
  digitalWrite(OUT_LATCH, LOW);
  digitalWrite(OUT_CLOCK, LOW);
  
  // для регистра ввода (<) левой клавиатуры 
  pinMode(LEFT_SCAN_DATA, INPUT);
  pinMode(LEFT_SCAN_CLOCK, OUTPUT);
  pinMode(LEFT_SCAN_SHLATCH, OUTPUT);
  digitalWrite(LEFT_SCAN_DATA, LOW);
  digitalWrite(LEFT_SCAN_CLOCK, HIGH);
  digitalWrite(LEFT_SCAN_SHLATCH, HIGH);
  
  // для регистра ввода gправой (>) клавиатуры 
  pinMode(RIGHT_SCAN_DATA, INPUT);
  pinMode(RIGHT_SCAN_CLOCK, OUTPUT);
  pinMode(RIGHT_SCAN_SHLATCH, OUTPUT);
  digitalWrite(RIGHT_SCAN_DATA, LOW);
  digitalWrite(RIGHT_SCAN_CLOCK, HIGH);
  digitalWrite(RIGHT_SCAN_SHLATCH, HIGH);
  
  //Serial.begin(9600);
  BootKeyboard.begin();
}





// --------------------------------------------------------------------------------
// ---------- Главный цикл ----------
// --------------------------------------------------------------------------------

// флаг экстренного отключения
boolean disableFlag = false;

// число нажатых кнопок 
byte pressedButtonsCount = 0; 

// когда последний раз были нажаты клавиши
long lastPressTime = 0;

long deltaTime;

void loop() {
  //if(!disableFlag)// флаг экстренного отключения
  
  //главный рутинный цикл
  loopRoutine();


  // для экономии эергии по определенному таймеру, если долго нет нажатий, 
  // то частота опроса регистров уменьшается, но считывание все равно происходит. 
  // Как только снова происходит нажатие, задержка(delay) на опросе снова уменьшается
  
  // если есть нажатые клавиши, сохраняем текущее время
  if(pressedButtonsCount != 0) {lastPressTime = millis();}
  // сколько времени прошло с последнего нажатия
  deltaTime = millis() - lastPressTime;

  
//4294966307  todo а че там с максимальными значениями?

  // пусть микроконтроллер отдохнет немного
  // пять секунд
  if(deltaTime < 15000){
    delay(10);
  } else if(deltaTime < 30000){
    delay(30);
  } else {
    delay(60);
  }
}


// --------------------------------------------------------------------------------
// ----------  ----------
// --------------------------------------------------------------------------------


// переменные счетчики, для промежуточных расчетов
byte xPoz;
byte yPoz;
byte arrayYLeftPoz;
byte arrayYRightPoz;
// буффер состояния нажатия текущей клавиши
boolean keyPressStateBuffer;

void loopRoutine(){

  
  // --- регистр вывода ----
  // выводим поочередно единицу на каждый из разрядов регистра
  for (xPoz = 0; xPoz < 8; xPoz++)  {
    
    // отправляем один бит данных (самый первый единица)
    digitalWrite(OUT_DATA, (xPoz==0));
    // тактируем каждую отправку через clock
    digitalWrite(OUT_CLOCK, HIGH);
    digitalWrite(OUT_CLOCK, LOW);  
    // разрешаем вывод данных на пины регистра вывода
    digitalWrite(OUT_LATCH, HIGH);  
    // запрещаем вывод данных на пины регистра вывода
    digitalWrite(OUT_LATCH, LOW); 

    // --- регистр ввода ---- считываем биты из регистра ввода, ища нажатые клавиши в этой колонке
    
    // обнуляем состояние регистров и считываем состояние выводов
    digitalWrite(LEFT_SCAN_SHLATCH, LOW);
    digitalWrite(RIGHT_SCAN_SHLATCH, LOW);
    digitalWrite(LEFT_SCAN_SHLATCH, HIGH);
    digitalWrite(RIGHT_SCAN_SHLATCH, HIGH);  
  
    // пропускаем первый разряд регистров(т.к. он никуда не подключен)
    digitalWrite(LEFT_SCAN_CLOCK, LOW);
    digitalWrite(RIGHT_SCAN_CLOCK, LOW);
    digitalWrite(LEFT_SCAN_CLOCK, HIGH);
    digitalWrite(RIGHT_SCAN_CLOCK, HIGH);
  

    // читаем последовательно все биты из регистра
    for (yPoz = 0; yPoz < 7; yPoz++)  {
      // считываем данные с очередного пина регистра
    
    
     // --- Для левой руки (<) --- 
    
      // пропускаем первые два разряда регистра
      if(2 <= yPoz){
      
        // преобразуем числа с клавиатуры в индексы массива
        arrayYLeftPoz = 4-yPoz+2;
      
        // --- обрабатываем нажатие и отпускание клавиш ----
        // нажатие конкретной клавиши
        keyPressStateBuffer = digitalRead(LEFT_SCAN_DATA);
        // запоминаем адресс текущей клавиши из массива
    chousenKeyX = xPoz;
    chousenKeyY = arrayYLeftPoz;
   
        // экстренное отключение
        //if(xPoz == 3 && arrayYLeftPoz == 4 && keyPressStateBuffer) {/*disableFlag = true;*/ BootKeyboard.releaseAll();}
        
        editKeyboardState();
      }

      // --- Для правой руки (>) ---
          
      // преобразуем числа с клавиатуры в индексы массива
      arrayYRightPoz = 5 + yPoz;

      // --- обрабатываем нажатие и отпускание клавиш ----
      // нажатие конкретной клавиши
      keyPressStateBuffer = digitalRead(RIGHT_SCAN_DATA);
      // запоминаем адресс текущей клавиши из массива
      chousenKeyX = xPoz;
      chousenKeyY = arrayYRightPoz;
    
      editKeyboardState();
      
      // тактируем регистры ввода для чтения нового пина
      digitalWrite(LEFT_SCAN_CLOCK, LOW);
      digitalWrite(RIGHT_SCAN_CLOCK, LOW);
      digitalWrite(LEFT_SCAN_CLOCK, HIGH);
      digitalWrite(RIGHT_SCAN_CLOCK, HIGH);
    }
  }
}


void editKeyboardState(){
  
  // если состояние клавиши изменилось
  if(keysState[chousenKeyY][chousenKeyX] != keyPressStateBuffer ){
    // инвертируем внутреннюю переменную
    keysState[chousenKeyY][chousenKeyX] = keyPressStateBuffer;
      
    // нажимаем или отпускаем эту кнопку
    if(keysState[chousenKeyY][chousenKeyX]){
      pressedButtonsCount++;
      BootKeyboard.press(keyCodes[chousenKeyY][chousenKeyX]);
    }else{
      pressedButtonsCount--;
      BootKeyboard.release(keyCodes[chousenKeyY][chousenKeyX]);
    }
    // вызываем функции клавиш
    //if((*currentKeyLink).funcNumber != 0){
    //  funcPrintScreen(keysState[chousenKeyY][chousenKeyX]);  
    //}
  }
  
}

// --------------------------------------------------------------------------------
// ----------  ----------
// --------------------------------------------------------------------------------


// --- раздел с отложенными действиями --- 
//boolean areDeferredEvents = false; // отложенные события (общий флаг для упрощения проверок)
//void checkEvents(){
//  if(areDeferredEvents){
//    //areDeferredEvents = false;
//
//
//
//    // для последовательности команд создаем массив клавиш и переменную позиции.
//    //  и переключаем её по мере прохождения. Если последовательность не активна, то в позиции  будет -1
//  }  
//}
//
// ---------- функции кнопок ----------
// кстати спец кнопка для текста должна сохранять положение курсора только пока нажата а потом снова сбрасываться к последнему напечатанному симовлу
//void funcPrintScreen(boolean newKeyState){
//  if(newKeyState){
//    BootKeyboard.press(KEY_LEFT_WINDOWS );
//    BootKeyboard.press(KEY_LEFT_SHIFT );
//    BootKeyboard.press(KEY_S);
//  }else{
//    BootKeyboard.release(KEY_S);
//    BootKeyboard.release(KEY_LEFT_SHIFT);
//    BootKeyboard.release(KEY_LEFT_WINDOWS);
//  }
//}


  // You can wakeup you PC from sleep.
  // This might be not supported on all hardware, but on all OS types.
  //Keyboard.wakeupHost();
