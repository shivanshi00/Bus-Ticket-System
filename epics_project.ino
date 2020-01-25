
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
boolean b = true;
char input[10];
int k = 0;
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS] = {0,2,3,4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,6,7,8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

/**
 * Initialize.
 */
void setup() {
    Serial.begin(9600); // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key 
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    Serial.println("Scan card...");
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();
}

/**
 * Main loop.
 */
void loop() {
    static int amt=0;

    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

     Serial.println();

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // In this sample we use the second sector,
    // that is: sector #1, covering block #4 up to and including block #7
    byte sector         = 1;
    byte blockAddr      = 4;
    byte trailerBlock   = 7;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);

    Serial.println();

    // Show the whole sector as it currently is
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();


    // Read data from the block
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    dump_byte_array(buffer, 16); 

    //Hexadecimal logic
    amt=buffer[0]; 
    amt=amt*256;
    amt|=buffer[1];
    
    Serial.println();
    //Serial.println(amt);
    Serial.println();

    static int r;
    static int pay;
    boolean c=true;
    while(c){
    char keys = keypad.getKey();
    if (!keys && b)
    {
      Serial.println("A. Recharge");
      Serial.println("B. Pay ticket price");
      Serial.println("C. Balance");
      Serial.println("D. Empty Card");
      //Serial.println(input);
      b=false;
    }
    else
    {
      if(keys)
      {
          Serial.print(keys);
          input[k] = keys;
          k++;
      }
      if(keys == 'A')
      {
          input[k--]='\0';
          Serial.println();
          Serial.println(input);
          Serial.println();
          Serial.print("Recharged amount: ");
          r = atoi(input);
          amt=amt+r;
          Serial.println(r);
          Serial.println();
          Serial.print("New Balance: ");
          Serial.println(amt);
          k = 0;
          r = 0;
          c=false;
       }
       if (keys == 'B')
       {
          input[k--]='\0';
          Serial.println();
          Serial.println(input);
          Serial.println();
          Serial.println("ROUTES: ");
          Serial.println("1. AMRITSAR TO DELHI -> Rs.100");
          Serial.println("2. DELHI TO CHANDIGARH -> Rs.200");
          Serial.println("3. CHANDIGARH TO AMRITSAR -> Rs.300");
          Serial.println("4. PATIALA TO CHANDIGARH -> Rs.400");
          Serial.println("5. RAJPURA TO MANSA -> Rs.500");
          while(c){
    char keys = keypad.getKey();
    if (!keys && b)
    {
      //Serial.println(input);
      b=false;
    }
    else
    {
      if(keys)
      {
          Serial.print(keys);
          input[k] = keys;
          k++;
      }
      if(keys == '1')
      {
          input[k--]='\0';
          pay=100;
          Serial.println();
          Serial.println("ROUTE: AMRITSAR TO DELHI");
          Serial.print("TICKET PRICE: ");
          Serial.println(pay);
          k = 0;
          r = 0;
          c=false;
       }
       if (keys == '2')
       {
          input[k--]='\0';
          pay=200;
          Serial.println();
          Serial.println("ROUTE: DELHI TO CHANDIGARH");
          Serial.print("TICKET PRICE: ");
          Serial.println(pay);
          k = 0;
          c=false;
         }
         if (keys == '3')
         {
          input[k--]='\0';
          pay=300;
          Serial.println();
          Serial.println("ROUTE: CHANDIGARH TO AMRITSAR");
          Serial.print("TICKET PRICE: ");
          Serial.println(pay);
          k = 0;
          c=false;
         }
         if (keys == '4')
         {
          input[k--]='\0';
          pay=400;
          Serial.println();
          Serial.println("ROUTE: PATIALA TO CHANDIGARH");
          Serial.print("TICKET PRICE: ");
          Serial.println(pay);
          k = 0;
          c=false;
         }
         if (keys == '5')
         {
          input[k--]='\0';
          pay=500;
          Serial.println();
          Serial.println("ROUTE: RAJPURA TO MANSA");
          Serial.print("TICKET PRICE: ");
          Serial.println(pay);
          k = 0;
          c=false;
         }
    }
    }
          if(amt>=pay)
          {
          amt=amt-pay;
          Serial.println();
          Serial.print("Balance: ");
          Serial.println(amt);
          k = 0;
          pay = 0;
          c=false;
          }
          else
          {
             Serial.println();
             Serial.println("Insufficient Balance!!");
          }
         }
         if (keys == 'C')
         {
          input[k--]='\0';
          Serial.println();
          Serial.print("Balance left: ");
          Serial.println(amt);
          k = 0;
          c=false;
         }
         if (keys == 'D')
         {
          input[k--]='\0';
          Serial.println();
          Serial.println("Card emptied!");
          amt=0;
          Serial.println();
          Serial.println("Balance: ");
          Serial.println(amt);
          k = 0;
          c=false;
         }
    }
    }
    byte dataBlock[] = {highByte(amt), lowByte(amt)};

    Serial.println();
  
    // Write data to the block
    Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlock, 16); Serial.println();
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();

    // Dump the sector data
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    c=true;
    b = true;
}


/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
