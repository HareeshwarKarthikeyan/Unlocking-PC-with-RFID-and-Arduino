// importation of necessary headers
#include <SPI.h>  // header file 
#include <MFRC522.h>  // header file for the rfid sensor (mfrc522) library
#include <Keyboard.h> // include the Keyboard header file for the peripheral functionality of a keyboard

// declaration of constant values
#define RETURN_KEY 0xB0  // hex code of the enter key 
#define RST_PIN 5  // reset pin
#define SS_PIN 10  // slave select pin    

int ledPin = 2;    // Pin for the Red LED     
 
MFRC522 rfidSensor(SS_PIN,RST_PIN) ; // defining the sensor variable

char enter = RETURN_KEY;  // hex code set for the character

String id;
String acceptedCardID="2395862";  // accepted rfid card's id            
int unlocked = 0;    // variable to specify locked or unlocked state       


void setup( ) 
{
  Serial.begin(9600);    // initialising the Serial monitor
  Keyboard.begin();      // initialising the keyboard setup
  SPI.begin();           // initialising the SPI
  rfidSensor.PCD_Init(); // initialising the RFID sensor
}


void convertToString(byte *uid, byte uidSize) // function to convert and store the UID in String format
{
    id = "";
    for (byte i = 0;i<uidSize; i++)
    {
      id = id + String(uid[i], HEX); // converting to hex equivalent of each character and convert to string
    }
   
}



void loop( ) 
{
  if(! rfidSensor.PICC_IsNewCardPresent()) // checking if a new card is present
  {
      return;
  }

  if(! rfidSensor.PICC_ReadCardSerial())  // reading the card's serial number
  {
      return;
  }

Serial.println(rfidSensor.PICC_IsNewCardPresent());
 //rfidSensor.PICC_DumpToSerial(&(rfidSensor.uid));  // Printing UID of RFID card to Serial Monitor

  convertToString(rfidSensor.uid.uidByte,rfidSensor.uid.size); // Converting the UID to string format
  
  Serial.println(id);

  if(id==acceptedCardID) // checking if the card is accepted
  {      
      Serial.println("RFID Accepted !!");
      // Unlocking when Locked
      if(unlocked == 0){
        // Enter followed by Password
        Keyboard.press(enter);                    
        Keyboard.release(enter);                   
        delay(100);
        Keyboard.print("FUTURE");                    
        Keyboard.releaseAll();
        delay(100);
        Keyboard.press(enter);
        Keyboard.releaseAll();
        Serial.println("Unlocked !!");
        delay(4000);
        unlocked = 1;
      }
      // Locking when unlocked
      else if(unlocked == 1){
        // CTRL-ALT-DEL
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_DELETE);
        delay(100);
        Keyboard.releaseAll();
      // DOWN ARROW AND ENTER
        Keyboard.press(KEY_DOWN_ARROW);
        delay(100);
        Keyboard.releaseAll();
        Keyboard.press(enter);
        Keyboard.releaseAll();
        Serial.println("Locked !!");
        delay(5000);
        unlocked = 0;
      }
  }
  // case of RFID card getting rejected
  else
  {
      Serial.println(" RFID Not Accepted !! ");
      // Glow Red LED for 2 seconds as indication
      digitalWrite(ledPin,HIGH);
      delay(2000);
      digitalWrite(ledPin,LOW);
      return;
  } 
}
