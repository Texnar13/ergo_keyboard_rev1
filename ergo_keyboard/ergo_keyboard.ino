#include "HID-Project.h"

// --------------------------------------------------------------------------------
// ----------  ----------
// --------------------------------------------------------------------------------

// сдвиговые регистры вывода  
#define LEFT_OUT_DATA  6
#define LEFT_OUT_LATCH 8
#define LEFT_OUT_CLOCK 9

// сдвиговый регистр ввода (<) левой клавиатуры
#define LEFT_SCAN_DATA 10
#define LEFT_SCAN_CLOCK 5
#define LEFT_SCAN_SHLATCH 7

// сдвиговый регистр ввода правой (>) клавиатуры
#define RIGHT_SCAN_DATA 4
#define RIGHT_SCAN_CLOCK 11
#define RIGHT_SCAN_SHLATCH 12


// запись с данными одной кнопки
struct KeyData {
  KeyboardKeycode symbol;
  boolean isChecked;  
  byte funcNumber;  
  
  KeyData(KeyboardKeycode newSymbol, byte func = 0){
    this -> symbol = newSymbol;
    this -> isChecked = false;
    this -> funcNumber = func;
  }
};

// https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h#L61
// таблица значений клавиш
KeyData keysDefinition[12][8] = {
  // таблица значений для левой руки (<)  
  { KeyData(KEY_ESC),        KeyData(KEY_1),            KeyData(KEY_2),            KeyData(KEY_3),            KeyData(KEY_4),           KeyData(KEY_5),          NULL,                   NULL                          },
  { KeyData(KEY_TAB),        KeyData(KEY_Q),            KeyData(KEY_W),            KeyData(KEY_E),            KeyData(KEY_R),           KeyData(KEY_T),          NULL,                   NULL                          },
  { KeyData(KEY_LEFT_SHIFT), KeyData(KEY_A),            KeyData(KEY_S),            KeyData(KEY_D),            KeyData(KEY_F),           KeyData(KEY_G),          NULL,                   NULL                          },
  { KeyData(KEY_LEFT_CTRL),  KeyData(KEY_Z),            KeyData(KEY_X),            KeyData(KEY_C),            KeyData(KEY_V),           KeyData(KEY_B),          KeyData(KEY_DELETE),    KeyData(KEY_DELETE)           },
  { NULL,                    NULL,                      KeyData(KEY_LEFT_ALT),     KeyData(KEY_LEFT_WINDOWS), KeyData(KEY_SPACE),       KeyData(KEY_ENTER),      KeyData(KEY_INSERT),    KeyData(KEY_INSERT)           },
                                                                                       
  // таблица значений для правой руки (>)                                                                                                       
  { NULL,                    KeyData(KEY_6),            KeyData(KEY_7),            KeyData(KEY_8),            KeyData(KEY_9),           KeyData(KEY_0),          NULL,                    NULL                         },
  { NULL,                    KeyData(KEY_Y),            KeyData(KEY_U),            KeyData(KEY_I),            KeyData(KEY_O),           KeyData(KEY_P),          KeyData(KEY_LEFT_BRACE), KeyData(KEY_RIGHT_BRACE),    },
  { NULL,                    KeyData(KEY_H),            KeyData(KEY_J),            KeyData(KEY_K),            KeyData(KEY_L),           KeyData(KEY_SEMICOLON),  KeyData(KEY_QUOTE),      KeyData(KEY_BACKSLASH)       },
  { KeyData(KEY_RIGHT),      KeyData(KEY_N),            KeyData(KEY_M),            KeyData(KEY_COMMA),        KeyData(KEY_PERIOD),      KeyData(KEY_SLASH),      KeyData(KEY_MINUS),      KeyData(KEY_EQUAL)           },
  { KeyData(KEY_UP),         KeyData(KEY_SPACE),        KeyData(KEY_ENTER),        KeyData(KEY_RIGHT_SHIFT),  KeyData(KEY_BACKSPACE),   KeyData(KEY_DELETE),     NULL,                    NULL                         },
  { KeyData(KEY_DOWN),       NULL,                      NULL,                      NULL,                      NULL,                     NULL,                    NULL,                    NULL                         },
  { KeyData(KEY_LEFT),       NULL,                      NULL,                      NULL,                      NULL,                     NULL,                    NULL,                    NULL                         }
};

// KeyData(KEY_ERROR_UNDEFINED, 1); KeyData(KEY_MENU)KeyData(KEY_RIGHT_SHIFT)
// есть нигде не использующаяся клавиша, можно назнаить ее под макросы, paus/break KEY_PAUSE  

// --------------------------------------------------------------------------------
// ----------  ----------
// --------------------------------------------------------------------------------

void setup() {
  // для регистра вывода 
  pinMode(LEFT_OUT_DATA, OUTPUT);
  pinMode(LEFT_OUT_LATCH, OUTPUT);
  pinMode(LEFT_OUT_CLOCK, OUTPUT);
  digitalWrite(LEFT_OUT_DATA, LOW);
  digitalWrite(LEFT_OUT_LATCH, LOW);
  digitalWrite(LEFT_OUT_CLOCK, LOW);
  
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
  }else if(deltaTime < 30000){
    delay(30);
  }else{
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

// буфферы для для уменьшения количества обращений к данным
boolean keyPressStateBuffer;
KeyData* currentKeyLink; 

void loopRoutine(){

  
  // --- регистр вывода ----
  // выводим поочередно единицу на каждый из разрядов регистра
  for (xPoz = 0; xPoz < 8; xPoz++)  {
    
    // запрещаем вывод данных на пины регистра вывода
    digitalWrite(LEFT_OUT_LATCH, LOW);
    
    // отправляем один бит данных (самый первый единица)
    digitalWrite(LEFT_OUT_DATA, (xPoz==0));
    // тактируем каждую отправку через clock
    digitalWrite(LEFT_OUT_CLOCK, HIGH);
    digitalWrite(LEFT_OUT_CLOCK, LOW);  
    // разрешаем вывод данных на пины регистра вывода
    digitalWrite(LEFT_OUT_LATCH, HIGH);   

    // --- регистр ввода ---- считываем биты из регистра ввода, ища нажатые клавиши в этой колонке
    
    // обнуляем состояние регистров и считываем состояние выводов
    digitalWrite(LEFT_SCAN_SHLATCH, LOW);
    digitalWrite(RIGHT_SCAN_SHLATCH, LOW);
    digitalWrite(LEFT_SCAN_SHLATCH, HIGH);
    digitalWrite(RIGHT_SCAN_SHLATCH, HIGH);  
  
    // пропускаем первый разряд регистров(т.к. он никуда не подключены)
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
        // извлекаем адресс текущей клавиши из массива
        currentKeyLink = &keysDefinition[arrayYLeftPoz][xPoz];
   
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
      // извлекаем адресс текущей клавиши из массива
      currentKeyLink = &keysDefinition[arrayYRightPoz][xPoz];
    
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
  if((*currentKeyLink).isChecked != keyPressStateBuffer ){
    // инвертируем внутреннюю переменную
    (*currentKeyLink).isChecked = keyPressStateBuffer;
      
    // нажимаем или отпускаем эту кнопку
    if((*currentKeyLink).isChecked){
      pressedButtonsCount++;
      BootKeyboard.press((*currentKeyLink).symbol);
    }else{
      pressedButtonsCount--;
      BootKeyboard.release((*currentKeyLink).symbol);
    }
    // вызываем функции клавиш
    if((*currentKeyLink).funcNumber != 0){
      funcPrintScreen((*currentKeyLink).isChecked);  
    }
  }
}

// --------------------------------------------------------------------------------
// ----------  ----------
// --------------------------------------------------------------------------------


// --- раздел с отложенными действиями --- 
boolean areDeferredEvents = false; // отложенные события (общий флаг для упрощения проверок)
void checkEvents(){
  if(areDeferredEvents){
    //areDeferredEvents = false;



    // для последовательности команд создаем массив клавиш и переменную позиции.
    //  и переключаем её по мере прохождения. Если последовательность не активна, то в позиции  будет -1
  }  
}

// ---------- функции кнопок ----------
// кстати спец кнопка для текста должна сохранять положение курсора только пока нажата а потом снова сбрасываться к последнему напечатанному симовлу
void funcPrintScreen(boolean newKeyState){
  if(newKeyState){
    BootKeyboard.press(KEY_LEFT_WINDOWS );
    BootKeyboard.press(KEY_LEFT_SHIFT );
    BootKeyboard.press(KEY_S);
  }else{
    BootKeyboard.release(KEY_S);
    BootKeyboard.release(KEY_LEFT_SHIFT);
    BootKeyboard.release(KEY_LEFT_WINDOWS);
  }
}


  // You can wakeup you PC from sleep.
  // This might be not supported on all hardware, but on all OS types.
  //Keyboard.wakeupHost();
