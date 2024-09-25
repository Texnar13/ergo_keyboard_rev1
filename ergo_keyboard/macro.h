#ifndef macro_h
#define macro_h
#include <Arduino.h>
#include "HID-Project.h"

// сделано ли что-то на слое
bool wasSomethingPressedOnLayer = false;


// выполение макросов
KeyboardKeycode activateMacro(uint8_t* choosenLayer, uint8_t macrosCode, bool pressedState){

  KeyboardKeycode returnKeyPress = KEY_RESERVED;

  switch(macrosCode){
    case 1:// нижний слой
      if(*choosenLayer == 0 && pressedState){
        *choosenLayer = 2;
        wasSomethingPressedOnLayer = false;
      }else if(*choosenLayer == 2 && !pressedState){
        *choosenLayer = 0;
        if(!wasSomethingPressedOnLayer){
          returnKeyPress = KEY_SPACE;
        }
      }
      break;

    case 2:// включить верхний слой
      if(*choosenLayer == 0 && pressedState == true){
        *choosenLayer = 1;
        wasSomethingPressedOnLayer = false;
      }else if(*choosenLayer == 1 && pressedState == false){
        *choosenLayer = 0;
        if(!wasSomethingPressedOnLayer){
          returnKeyPress = KEY_SPACE;
        }
      }
      break;
  }

  return returnKeyPress;
}


#endif



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


