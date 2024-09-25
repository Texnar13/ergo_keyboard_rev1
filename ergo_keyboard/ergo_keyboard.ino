#pragma once
#include <Arduino.h>
#include "HID-Project.h"
#include "binds.h"
#include "macro.h"

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

// текущий слой
int8_t choosenLayer = 0;

// массив с состояниями нажатых клавиш (нужен только для определения нажата физически клавиша или нет)
// [строка][столбец]
bool keysState[12][8] = {
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
bool disableFlag = false;

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
    
    // есть ли на данную клавишу макрос
    if(keysMacros[0][chousenKeyY][chousenKeyX] != 0){

      KeyboardKeycode key = activateMacro(&choosenLayer, keysMacros[0][chousenKeyY][chousenKeyX], keysState[chousenKeyY][chousenKeyX]);
      
      if(key != KEY_RESERVED){
        BootKeyboard.press(key);
        BootKeyboard.release(key);
      }
    }else{

      // нажимаем или отпускаем эту кнопку
      if(keysState[chousenKeyY][chousenKeyX]){
        pressedButtonsCount++;
        BootKeyboard.press(keyCodes[choosenLayer][chousenKeyY][chousenKeyX]);
        // отмечаем любое нажатие клавиши
        wasSomethingPressedOnLayer = true;
      }else{
        pressedButtonsCount--;
        BootKeyboard.release(keyCodes[choosenLayer][chousenKeyY][chousenKeyX]);
      }

    }
  }
  
}