/*  Vibrerande armband för döva
 *  ------------------------------------------------------------------------------------
 *  Programmet tar in en signal från mikrofonen och sparar endast de relevanta signalerna. 
 *  Om signalen går över ett tröskelvärde vibrerar motorn så länge ljudnivån håller sig  
 *  över tröskelvärdet.
 *  ------------------------------------------------------------------------------------
 *  Kod skriven av: Oscar Fredriksson
 */

void setup()    //Används inte för tillfället, behövs för att gå igenom kompileringen
{
    Serial.begin(9600);     //Startar serial monitorn
}

const int motorPin = 3;         //Vilken pin vibrationsmotorn är inkopplad på
const int threshold = 1000;     //Tröskelvärde för ljudnivån

void loop()
{   
    /*
     * Motorn kommer vibrera sålänge ljudsignalen är över tröskelvärdet
     */
    if(readSignal() > threshold)        //Om  intläst ljudsignal är över tröskelvärdet
        analogWrite(motorPin, 255);     //Driv motorn på maxfart
    else                                //Så fort signalen inte längre är över tröskelvärdet
        analogWrite(motorPin, 0);       //Stäng av motorn
}

int readSignal()    //Läser ljud under en förbestämd tid och returnerar den största inlästa peak to peak amplituden
{
   const int sampleTime = 50;       //Hur många ms programmet ska sampla ljud
   
   unsigned long start = millis();  //Påbörja tidräkningen 
   unsigned int peakToPeak = 0;     //Amplituden mellan den högsta och lägsta spiken
  
   unsigned int signalMax = 0;      //Signalens maxvärde
   unsigned int signalMin = 1023;   //Signalens minvärde

   unsigned int input;              //Det inlästa värdet

   while (millis() - start < sampleTime)  //Samla data under den förbestämda tiden 
   {
     input = analogRead(5); //Läs det analoga värdet
  
     if (input > signalMax) //Om inläsningen är större än det tidigare maxvärdet
        signalMax = input;  //Spara det nya maxvärdet
     
     else if (input < signalMin)    //Om inläsningen är mindre än det tidigare minvärdet
        signalMin = input;          //Spara det nya minvärdet
   }
   peakToPeak = signalMax - signalMin;  //Beräknar peak to peak amplituden
   
   return peakToPeak;                   //Returnerar amplituden
}

