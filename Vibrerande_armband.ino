/*  Vibrerande armband för döva
 *  ------------------------------------------------------------------------------------
 *  Programmet tar in en signal från mikrofonen och sparar endast de relevanta signalerna. 
 *  Om signalen går över ett tröskelvärde vibrerar motorn så länge ljudnivån håller sig  
 *  över tröskelvärdet.
 *  ------------------------------------------------------------------------------------
 *  Kod skriven av: Oscar Fredriksson
 */

const int motorPin = 6;     //Vilken pin vibrationsmotorn är inkopplad på
const int ledPin = 7;
const int threshold = 1000;  //Tröskelvärde för ljudnivån

void setup() 
{
    Serial.begin(9600); //Startar serial monitorn
    pinMode(motorPin, OUTPUT);  //Definierar motorPin som en output
}

void loop() 
{   
   //Motorn kommer vibrera sålänge ljudsignalen är över tröskelvärdet 
   if(readSignal() > threshold) //Om  intläst ljudsignal är över tröskelvärdet
    digitalWrite(motorPin, HIGH);  //Driv motorn
   else //Så fort signalen inte längre är över tröskelvärdet
   {
    digitalWrite(motorPin, LOW);  //Stäng av motorn
   }
   
}

int readSignal()
{
   const int sampleTime = 50;       //Hur länge programmet ska sampla ljud
   
   unsigned long start = millis();  //Påbörja tidräkningen 
   unsigned int peakToPeak = 0;     //Amplituden mellan den högsta och lägsta spiken
  
   unsigned int signalMax = 0;      //Signalens maxvärde
   unsigned int signalMin = 1024;   //Signalens minvärde

   unsigned int input;              //Det inlästa värdet
   
   while (millis() - start < sampleTime)  //Samla data under den förbestämda tiden 
   {
     input = analogRead(5); //Läs det analoga värdet
  
     if (input > signalMax) //Om inläsningen är större än det tidigare maxvärdet
        signalMax = input;  //Spara det nya maxvärdet
     
     else if (input < signalMin)  //Om inläsningen är mindre än det tidigare minvärdet
        signalMin = input;  //Spara det nya minvärdet
   
   }
   peakToPeak = signalMax - signalMin;  //Beräknar peak to peak amplituden
   Serial.println(peakToPeak);          //Skriver amplituden till serial monitorn
   return peakToPeak;                   //Returnerar amplituden
}

