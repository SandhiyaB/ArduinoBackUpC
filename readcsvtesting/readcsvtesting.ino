#include <SD.h>

// Note that even if it's not used as the CS pin, the hardware
// CS pin (10 on most Arduino boards,
// must be left as an output or the SD library functions will
// not work.  In the Sparkfun SD Shield the CS is on
// Arduino Pin 8.
const int chipSelect = 15;
int fieldIndex = 0;  //The current field being received

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);



  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  File dataFile = SD.open("json2.csv");

  // if the file is available, read from it:
  if (dataFile)
  {
  // In order to correctly size the array the first two digits
  // off the file will be read as the Number of Fields to use.
  // NoF is the number of comma separated fields we expected.

    char ch = dataFile.read();  // Read the first entry - tens
    Serial.println("data");
    Serial.println(ch);
    int NoF = 10 * (ch-'0');    //Change ASCII digit to a tens digit
    ch = dataFile.read();    // Read the second entry
    NoF = NoF + ch-'0';    // Add the ones digit to the tens digit
    ch = dataFile.read(); // read the next figure which will be a comma, but ignore it.
    unsigned long values[NoF];  //This is the array to hold the values.
    Serial.println(NoF);


    if (dataFile.available())
    {
      for(fieldIndex = 0; fieldIndex < NoF; fieldIndex ++)
      {
        values[fieldIndex]=dataFile.parseInt(); //get a numerical value
      }
      Serial.print(fieldIndex);
      Serial.println(" Fields recieved:");
      for(int i = 0; i < fieldIndex; i++)
      {
        Serial.println(values[i]);
      }
      fieldIndex = 0; //reset the indexer
    }

    dataFile.close();
  }

  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening timesfic.csv");
  } 
}

void loop()
{
}
