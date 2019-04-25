#include <Keypad.h>

const byte n_rows = 4; // Four rows

const byte n_cols = 4; // Three columns
int j=0;
// Define the Keymap

char keys[n_rows][n_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 
byte colPins[n_rows] = {D3, D2, D1, D0};
byte rowPins[n_cols] = {D7, D6, D5, D4};
int firstvariable;
// Create the Keypad Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols); 

void setup()

{


Serial.begin(9600);

}

void loop()
{
 // for 12V -----------------------------------------------------------
 char key1 = kpd.getKey();
 switch (key1) {
   case NO_KEY:
   break;

   case '0': case '1': case '2': case '3': case '4': 
   case '5': case '6': case '7': case '8': case '9': 
   //lcd.print(key1);
       firstvariable = firstvariable* 10 + key1 - '0'; 
        Serial.print (firstvariable);
   break;

   case '#': 
       Serial.println ();
   Serial.print ("Value for 12V is: ");
   Serial.print (firstvariable );
       break;

   case '*': 
       firstvariable = 0;
      // lcd.clear();
       Serial.println ();
   Serial.print ("Reset value:");
   Serial.print (firstvariable );
 }
 // for 12V end -----------------------------------------------------------
}
