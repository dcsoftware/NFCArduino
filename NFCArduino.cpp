// Do not remove the include below
#include "NFCArduino.h"

// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries

#include "SPI.h"
#include "MPN532_SPI.h"
#include "MyCard.h"
//#include "emulatetag.h"
#include "MNdefMessage.h"
//#include <avr/wdt.h>
#include <EEPROMex.h>
#include <avr/interrupt.h>
#define SERIAL_COMMAND_CONNECTION "connection:"
#define SERIAL_COMMAND_RECHARGE "recharge:"
#define SERIAL_COMMAND_PURCHASE "purchase:"
#define SERIAL_COMMAND_SET_DATA "set_data:"
#define SERIAL_COMMAND_GET_TIME "get_time:"
#define SERIAL_COMMAND_SET_TIME "set_time:"
#define SERIAL_RESPONSE_OK "ok;"
#define SERIAL_RESPONSE_ERROR "err;"
#define SERIAL_VALUE_REQUEST "req;"


//typedef enum {S_DISCONNECTED, S_CONNECTED} SerialState;

//SerialState serialState;

PN532_SPI pn532spi(SPI, 10);
MyCard nfc(pn532spi);
//EmulateTag nfc(pn532spi);

uint8_t ndefBuf[120];
NdefMessage message;
int messageSize;
int idAddress = 0;
char id[8] = "";

uint8_t uid[3] = { 0x12, 0x34, 0x56 };
volatile int n = 0;
/*int led = 7;
int led1 = 6;
int led2 = 5;
int intPin = 2;
volatile int interruptCount = 0;

String inputCommand = "";         // a string to hold incoming data
String inputValue = "";
boolean commandComplete = false;  // whether the string is complete
boolean valueIn = false;*/


/*void myIsr (){
    noInterrupts();
    interruptCount++;
    nfc.updateInterruptCount(interruptCount);
    interrupts();
    digitalWrite(led, !digitalRead(led));
}

ISR(INT0_vect) {
    interruptCount++;
    nfc.updateInterruptCount(interruptCount);
}*/

//
// Brief	Setup
// Details	Define the pin the LED is connected to
//
// Add setup code
void setup() {
    if(EEPROM.isReady()) {
        //EEPROM.readBlock<char>(idAddress, id, 8);
    }

    if(id == 0) {
        if(EEPROM.isReady()) {
            //EEPROM.writeBlock<char>(idAddress, "00000001", 8);
        }
    }

    nfc.setId("00005678");

    /*pinMode(intPin, INPUT);
    digitalWrite(intPin, HIGH); //enabling pull up resistor
    serialState = S_DISCONNECTED;*/
    Serial.begin(230400);
    //Serial.println("------- Emulate Tag --------");
    //Serial.println("connection:req;");
    /*if(serialState == S_DISCONNECTED) {
        Serial.println("connection:req;");
    }*/
    message = NdefMessage();
    message.addMimeMediaRecord("application/coffeeapp", "ciao");
    messageSize = message.getEncodedSize();
    if (messageSize > sizeof(ndefBuf)) {
        //Serial.println("ndefBuf is too small");
        while (1) { }
    }

    //Serial.print("Ndef encoded message size: ");
    //Serial.println(messageSize);

    message.encode(ndefBuf);

    // comment out this command for no ndef message
    nfc.setNdefFile(ndefBuf, messageSize);

    // uid must be 3 bytes!
    nfc.setUid(uid);

    nfc.init();

    /*sei();                            //external interrupt routine
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01);

    //attachInterrupt(0, myIsr, RISING);*/

    // sleep bit patterns:
    //  16 ms: 0b000000
    //  32 ms: 0b000001
    //  64 ms: 0b000010
    //  125 ms: 0b000011
    //  250 ms: 0b000100
    //  500 ms: 0b000101
    //  1 s:  0b000110
    //  2 s: 0b000111
    //  4 s: 0b100000
    //  8 s: 0b100001
    //watchdogTimerEnable(0b000101);
}

//
// Brief	Loop
// Details	Blink the LED
//
// Add loop code

void loop() {
	Serial.println("log:main loop;");
    nfc.emulate();
    Serial.println("log:fine emulazione;");
}

