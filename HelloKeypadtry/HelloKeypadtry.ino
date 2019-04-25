/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>

const byte n_rows = 4;
const byte n_cols = 4;
int num[] = {0, 0, 0, 0};
char keys[n_rows][n_cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[n_rows] = {D3, D2, D1, D0};
byte rowPins[n_cols] = {D7,D9, D5, D4};
int i = 0;
int firstvariable;

Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);
void setup(){
  Serial.begin(9600);
}
  
void loop(){
  
 
  getNum();
}
void getNum(){
   char key = myKeypad.getKey();
  
  if (key){
    Serial.println(key);
  }
}
