/*  Vibrerande armband för döva
 *  -------------------------------------------------------------------------------------------------------
 *  Programmet tar in en signal från mikrofonen och räknar utifrån detta ut amplituden (alltså ljudnivån) 
 *  för aktuell ljudvåg. Om amplituden går över ett tröskelvärde vibrerar motorn så länge denna håller sig 
 *  över tröskelvärdet.
 *  Programmet kommunicerar via bluetooth med tillhörande konfigueringsapp. Denna kan användas för att 
 *  konfiguera vibrationsstyrkan, det aktuella värdet sparas på Arduinon och laddas in varje gång denna 
 *  startas. Programmet skickar även konstant inläst amplitud till appen för att kunna logga denna. 
 *  --------------------------------------------------------------------------------------------------------
 *  Kod skriven av: Oscar Fredriksson   
 */

#include <SoftwareSerial.h> //För bluetooth uppkopplingen
#include <EEPROM.h> //För att spara till minnet

//Använder define istället för const int för att enkelt kunna byta mellan int och string
#define inputPin 0          //Vilken pin mikrofonen är inkopplad på
#define motorPin 3          //Vilken pin vibrationsmotorn är inkopplad på
#define ledPin LED_BUILTIN  //Vilken pin lysdioden är inkopplad på
#define threshold 1000      //Tröskelvärde för ljudnivån
#define rxPin 7             //Rx pin för bluetooth uppkopplingen
#define txPin 8             //Tx pin för bluetooth uppkopplingen

SoftwareSerial bluetoothSerial (rxPin, txPin);  //Initiering av bluetooth uppkopplingen

int vibrStrength; //Variabel för vibrationsstyrkan

void setup()
{
    pinMode(ledPin, OUTPUT);    //Definiera ledPin som en output
    pinMode(rxPin, INPUT);      //Definera rxPin som en input
    pinMode(txPin, OUTPUT);     //definiera txPin som en output
    Serial.begin(9600);         //Startar serial monitorn (används för tillfället endast för felsökning)
    bluetoothSerial.begin(9600);      //Startar bluetoothkopplingen
    vibrStrength = EEPROM.read(0);  //Läs vibrationsstyrkan från minnet
}

void loop()
{   
    readBluetooth();    //Läs från appen

    int amplitude = readSignal();   //Läs amplituden
    bluetoothSerial.print(amplitude);   //Skicka amplituden till appen

    //Motorn kommer vibrera sålänge ljudsignalen når över tröskelvärdet 
    if(amplitude > threshold)        //Om  intläst ljudsignal är över tröskelvärdet
    {
        analogWrite(motorPin, vibrStrength);     //Driv motorn på maxfart
        digitalWrite(ledPin, HIGH);     //Tänd lysdioden
    }
    else    //Så fort signalen inte längre är över tröskelvärdet
    {
        analogWrite(motorPin, 0);       //Stäng av motorn
        digitalWrite(ledPin, LOW);      //Släck lysdioden      
    }
}

void readBluetooth()
{
    int temp = bluetoothSerial.read();    //Läser in från blåtandsuppkopplingen
    
    if(temp)    //Om variabeln inte är 0 är det ett värde för vibrationsstyrkan
    {
        //Om det skickade värdet är låg, mellan eller hög, tilldela relevanta värden
        if(temp == 1)   vibrStrength = 100; 
        if(temp == 2)   vibrStrength = 200;
        if(temp == 3)   vibrStrength = 255; 
        
        EEPROM.write(0, vibrStrength);  //Skriv det nya värdet till minnet
    }
}

int readSignal()    //Läser ljud under en förbestämd tid och returnerar den största inlästa peak to peak amplituden
{
   const int sampleTime = 50;       //Hur många ms programmet ska sampla ljud
   
   unsigned long start = millis();  //Påbörja tidräkningen 
   unsigned int peakToPeak = 0;     //Amplituden mellan den högsta och lägsta spiken
  
   unsigned int signalMax = 0;      //Signalens maxvärde
   unsigned int signalMin = 1023;   //Signalens minvärde

   unsigned int input;              //Det inlästa värdet från mikrofonen

   while (millis() - start < sampleTime)  //Samla data under den förbestämda tiden 
   {
     input = analogRead(inputPin); //Läs det analoga värdet
  
     if (input > signalMax) //Om inläsningen är större än det tidigare maxvärdet
        signalMax = input;  //Spara det nya maxvärdet
     
     else if (input < signalMin)    //Om inläsningen är mindre än det tidigare minvärdet
        signalMin = input;          //Spara det nya minvärdet
   }
   peakToPeak = signalMax - signalMin;  //Beräknar peak to peak amplituden
   
   return peakToPeak;   //Returnerar amplituden
}
