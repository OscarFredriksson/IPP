/*  Vibrerande armband för döva
 *  ------------------------------------------------------------------------------------
 *  Programmet tar in en signal från mikrofonen och sparar endast de relevanta signalerna. 
 *  Om signalen går över ett tröskelvärde vibrerar motorn så länge ljudnivån håller sig  
 *  över tröskelvärdet.
 *  ------------------------------------------------------------------------------------
 *  Kod skriven av: Oscar Fredriksson   
 */

//Använder define istället för const int för att enkelt kunna byta mellan int och string
#define inputPin 0          //Vilken pin mikrofonen är inkopplad på
#define motorPin 3          //Vilken pin vibrationsmotorn är inkopplad på
#define ledPin LED_BUILTIN  //Vilken pin lysdioden är inkopplad på
#define threshold 1000      //Tröskelvärde för ljudnivån

void setup()
{
    Serial.begin(9600);         //Startar serial monitorn (används för tillfället endast för felsökning)
    pinMode(ledPin, OUTPUT);    //Definiera ledPin som en output
}

void loop()
{   
    //Motorn kommer vibrera sålänge ljudsignalen når över tröskelvärdet 
    if(readSignal() > threshold)        //Om  intläst ljudsignal är över tröskelvärdet
    {
        analogWrite(motorPin, 255);     //Driv motorn på maxfart
        digitalWrite(ledPin, HIGH);     //Tänd lysdioden
    }
    else    //Så fort signalen inte längre är över tröskelvärdet
    {
        analogWrite(motorPin, 0);       //Stäng av motorn
        digitalWrite(ledPin, LOW);      //Släck lysdioden      
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