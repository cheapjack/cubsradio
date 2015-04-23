
long millisAtStart=0;
long millisAtEnd=0;

const long period_broadcast=8; //period of shortest broadcast (256 port changes)

//#define LENGTH_DIT 64
#define LENGTH_DIT 128
//#define LENGTH_DIT 256
//number of period_broadcasts in one 'dit', 
//all other lengths are scaled from this

const int length_dit=LENGTH_DIT;//number of periods for dit
const int pause_dit=LENGTH_DIT;//pause after dit
const int length_dah=3*LENGTH_DIT;//number of persots for dah
const int pause_dah=LENGTH_DIT;//pause after dah
const int length_pause=7*LENGTH_DIT;//pause between words

void dit(void); 
void dah(void); 
void pause(void);
void broadcast(int N_cycles);
void dontbroadcast(int N_cycles);

// ### INC ### Increment Register (reg = reg + 1)
#define ASM_INC(reg) asm volatile ("inc %0" : "=r" (reg) : "0" (reg)) 

int incomingByte = 0;
//int morseOutput = 13; //Pin for receiving Morse output - attach LED to this pin
int interval = 120;   //length of time for one dot - basic unit of measurement;  one interval occurs between each dot or dash; dash is equivalent to 3 dots; 7 dots between words

String alphabet = "abcdefghijklmnopqrstuvwxyz1234567890 ";
String ALPHABET = alphabet;

String morseCode[] = { 
    ".-", // A
    "-...", // B
    "-.-.", // C
    "-..", // D
    ".", // E
    "..-.", // F
    "--.", // G
    "....", // H
    "..", // I
    ".---", // J
    "-.-", // K
    ".-..", // L
    "--", // M
    "-.", // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.", // R
    "...",  // S
    "-", // T
    "..-", // U
    "...-", // V
    ".--", // W
    "-..-", // X
    "-.--", // Y
    "--..", // Z
    ".----", // 1
    "..---", // 2
    "...--", // 3
    "....-", // 4
    ".....", // 5
    "-....", // 6
    "--...", // 7
    "---..", // 8
    "----.", // 9
    "-----", // 0
    " "   //space character
};


void setup()  
{

 Serial.begin(9600);
 DDRB=0xFF;  //Port B all outputs
 //Do one dit to determine approximate frequency
 millisAtStart=millis();
 dit();
 millisAtEnd=millis();
 Serial.print(millisAtEnd-millisAtStart);  
 Serial.print(" ");
 Serial.print((length_dit+pause_dit)*period_broadcast*256/(millisAtEnd-millisAtStart)/2);
 Serial.print("kHz ");
 Serial.println(); 
 
 ALPHABET.toUpperCase();
 hello();
}

void loop()  
{
 if (Serial.available() > 0) {
    incomingByte = Serial.read();
    char c = (char) incomingByte;
    sendLetter(c);
 } 
    
}

void hello(){
  dah();
  dah();
dah();
dah();
pause();
dit();
pause();
dit();
dah();
dit();
dit();
pause();
dit();
dah();
dit();
dit();
pause();
pause();
pause();
dit();
dah();
dah();
pause();
dah();
dah();
dah();
pause();
dit();
dah();
dit();
pause();
dit();
dah();
dit();
dit();
pause();
dah();
dit();
dit();
}

void dit(void)
{
 for(int i=0;i<length_dit;i++)
 {
   broadcast(period_broadcast);
 }
 for(int i=0;i<pause_dit;i++)
 {
   dontbroadcast(period_broadcast);
 }
}


void dah(void)
{
 for(int i=0;i<length_dah;i++)
 {
   broadcast(period_broadcast);
 }
 for(int i=0;i<pause_dah;i++)
 {
   dontbroadcast(period_broadcast);
 }
}

void pause(void)
{
 for(int i=0;i<length_pause;i++)
 {
   dontbroadcast(period_broadcast);
 }
}

void broadcast(int N_cycles)
{
 unsigned int portvalue;
 for (int i=0;i<N_cycles;i++)
 {
   portvalue=0;
   do
   {
     PORTB=portvalue;
     ASM_INC(portvalue);
   }
   while(portvalue<255);
 }
}

void dontbroadcast(int N_cycles)
{
 unsigned int portvalue;
 PORTB=0x00;
 for (int i=0;i<N_cycles;i++)
 {
   portvalue=0;
   do
   {
     ASM_INC(portvalue);
     //add some assembly No OPerations to keep timing the same
     asm volatile ("NOP");  
   }
   while(portvalue<255);
 }
}


void sendLetter (char c) {
   int i;
   for (i = 0; i < alphabet.length(); i = i + 1) {
       if (alphabet[i] == c || ALPHABET[i] == c) {
           Serial.print(c);
           sendMorseCode(morseCode[i]);
           return;
       }
   }
   if (c == '\n')
      Serial.println();
}

void sendMorseCode (String tokens) {
   int i;
   for (i = 0; i < tokens.length(); i = i + 1) {
       switch (tokens[i]) {
           case '-':
               sendDash();
               break;
           case '.':
               sendDot();
               break;
           case ' ':
               sendEndOfWord();
               break;
       }
   }
   //morseOutputOff(2);
   pause();
   Serial.print(" ");
}

void sendEndOfWord() {
   pause();
   Serial.print("  ");
}

//basic functions - Morse code concepts  
void sendDot() {
   dit();
   Serial.print(".");
}
void sendDash() {
   dah();
   Serial.print("-");
}


//Low level functions - how the actions are accomplished
// n = number of intervals 
// interval is a fixed length of time determined at start, for example 200 milliseconds
/*void morseOutputOn (int n) {
//   example: morseOutputOn(1) means turn output on and keep it on for 1 interval 
//            morseOutputOn(3) means turn output on and keep it on for 3 intervals 
//   
   digitalWrite(morseOutput, HIGH);
   delay(n * interval);
}

void morseOutputOff (int n) {
   digitalWrite(morseOutput, LOW);
   delay(n * interval);
}*/

