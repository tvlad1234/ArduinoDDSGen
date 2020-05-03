#include <EEPROM.h>
#include <TimerOne.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#define FREQ 100
#define waveAd 0
#define freqAd 1
#define dutyAd 3
#define SS 10
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET  -1
#define SET 4 //2
#define down 2 //6
#define up 3 //7
#define selw 5 //3
#define dutyup 7
#define dutydown 6
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned int FREQ;
float dutycycle;
byte i=0;              // counter
byte waveform; //sine=0, tr=1, rampU=2, rampD=3, sq=4

const byte sine[256] ={
128,131,134,137,140,143,146,149,
152,155,158,162,165,167,170,173,
176,179,182,185,188,190,193,196,
198,201,203,206,208,211,213,215,
218,220,222,224,226,228,230,232,
234,235,237,238,240,241,243,244,
245,246,248,249,250,250,251,252,
253,253,254,254,254,255,255,255,
255,255,255,255,254,254,254,253,
253,252,251,250,250,249,248,246,
245,244,243,241,240,238,237,235,
234,232,230,228,226,224,222,220,
218,215,213,211,208,206,203,201,
198,196,193,190,188,185,182,179,
176,173,170,167,165,162,158,155,
152,149,146,143,140,137,134,131,
128,124,121,118,115,112,109,106,
103,100,97,93,90,88,85,82,
79,76,73,70,67,65,62,59,
57,54,52,49,47,44,42,40,
37,35,33,31,29,27,25,23,
21,20,18,17,15,14,12,11,
10,9,7,6,5,5,4,3,
2,2,1,1,1,0,0,0,
0,0,0,0,1,1,1,2,
2,3,4,5,5,6,7,9,
10,11,12,14,15,17,18,20,
21,23,25,27,29,31,33,35,
37,40,42,44,47,49,52,54,
57,59,62,65,67,70,73,76,
79,82,85,88,90,93,97,100,
103,106,109,112,115,118,121,124
};



const byte tr[256] ={
2,4,6,8,10,12,14,16,
18,20,22,24,26,28,30,32,
34,36,38,40,42,44,46,48,
50,52,54,56,58,60,62,64,
66,68,70,72,74,76,78,80,
82,84,86,88,90,92,94,96,
98,100,102,104,106,108,110,112,
114,116,118,120,122,124,126,128,
129,131,133,135,137,139,141,143,
145,147,149,151,153,155,157,159,
161,163,165,167,169,171,173,175,
177,179,181,183,185,187,189,191,
193,195,197,199,201,203,205,207,
209,211,213,215,217,219,221,223,
225,227,229,231,233,235,237,239,
241,243,245,247,249,251,253,255,
253,251,249,247,245,243,241,239,
237,235,233,231,229,227,225,223,
221,219,217,215,213,211,209,207,
205,203,201,199,197,195,193,191,
189,187,185,183,181,179,177,175,
173,171,169,167,165,163,161,159,
157,155,153,151,149,147,145,143,
141,139,137,135,133,131,129,128,
126,124,122,120,118,116,114,112,
110,108,106,104,102,100,98,96,
94,92,90,88,86,84,82,80,
78,76,74,72,70,68,66,64,
62,60,58,56,54,52,50,48,
46,44,42,40,38,36,34,32,
30,28,26,24,22,20,18,16,
14,12,10,8,6,4,2,0
};


void sinewave(){

  
  SPI.transfer(sine[i]);
  PORTB |= 1UL << 2; //ss high
  PORTB &= ~(1UL << 2); //ss low
  i++;
}

void triwave(){
  
  SPI.transfer(tr[i]);
  PORTB |= 1UL << 2; //ss high
  PORTB &= ~(1UL << 2); //ss low
  i++;
}

void sawwave1(){
  SPI.transfer(i);
  PORTB |= 1UL << 2; //ss high
  PORTB &= ~(1UL << 2); //ss low
  i++;
}

void sawwave2(){
  SPI.transfer(i);
  PORTB |= 1UL << 2; //ss high
  PORTB &= ~(1UL << 2); //ss low
  i--;
}

void noise(){
  SPI.transfer(random(0,255));
  PORTB |= 1UL << 2; //ss high
  PORTB &= ~(1UL << 2); //ss low
}

void atint(unsigned int i){
  if(i==0)
    Timer1.attachInterrupt(sinewave);
  else if(i==1)
    Timer1.attachInterrupt(triwave);
  else if(i==2)
    Timer1.attachInterrupt(sawwave1);
  else if(i==3)
    Timer1.attachInterrupt(sawwave2);
  else if(i==4)
  {
   Timer1.setPeriod(1/(0.000256*450));
    Timer1.attachInterrupt(noise);
  }
    
  else if(i==5)
  {
    Timer1.setPeriod(1/(0.000001*FREQ));
    Timer1.pwm(9,(dutycycle/100)*1023);
  }
    
}


void showInfo(){
  
  display.clearDisplay();
    display.setCursor(0,0);
    display.print(FREQ);
    display.println("Hz");
    if(waveform==0)
      display.print("Sine");
    else if(waveform==1)
      display.print("Triangle");
    else if(waveform==2)
      display.print("RampUp");
    else if(waveform==3)
      display.print("RampDn");
    else if(waveform==4)
    {
      display.clearDisplay();
    display.setCursor(0,0);
    display.print("Noise");
    }
     else if(waveform==5)
     {
       display.print("Square");
       display.setCursor(70,0);
       display.print((int)dutycycle);
       display.print('%');
     }
      
    display.display();

}

void setup(){
   //delay(100);
  //noInterrupts();
   delay(100);
   EEPROM.get(freqAd, FREQ);
   EEPROM.get(waveAd, waveform);
   EEPROM.get(dutyAd, dutycycle);
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.setTextColor(WHITE);
   display.setTextSize(2);
   showInfo();
  pinMode(up, INPUT_PULLUP);
  pinMode(down,INPUT_PULLUP);
  pinMode(SET, INPUT_PULLUP);
  pinMode(selw, INPUT_PULLUP);
  pinMode(dutyup, INPUT_PULLUP);
  pinMode(dutydown, INPUT_PULLUP);
  pinMode(SS, OUTPUT);
 // SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();
  
 
 //interrupts();
  Timer1.initialize(1/(0.000256*FREQ));
// Timer1.initialize(65);
  atint(waveform);
 // Timer1.pwm(9, 512);
  

}

void loop(){

  if(!digitalRead(SET))
  {
    Timer1.stop();
    Timer1.detachInterrupt();
  while(!digitalRead(SET))
  {
    
  if(!digitalRead(up))
  {
    //Timer1.detachInterrupt();
    if(FREQ<210 || waveform==5)
      FREQ+=10;
    EEPROM.put(freqAd, FREQ);
    showInfo();
   // period=1/(0.000256*FREQ)
    Timer1.setPeriod(1/(0.000256*FREQ));
    delay(150);
    //Timer1.attachInterrupt(wave);
  }

  if(!digitalRead(down) && FREQ>10)
  {
    //Timer1.detachInterrupt();
    
    FREQ-=10;
    EEPROM.put(freqAd, FREQ);
    showInfo();
    Timer1.setPeriod(1/(0.000256*FREQ));
    delay(150);
    //Timer1.attachInterrupt(wave);
  }

  if(!digitalRead(selw)){
    waveform++;
    EEPROM.put(waveAd, waveform);
    if(waveform > 5)
    {
      Timer1.setPeriod(1/(0.000256*FREQ));
      waveform=0;
      if(FREQ>210)
        FREQ=210;
    }
      
    showInfo();
    delay(150);
  }

  if(!digitalRead(dutydown) && dutycycle>0)
  {
    dutycycle-=10;
    EEPROM.put(dutyAd, dutycycle);
    showInfo();
    delay(150);
  }
   if(!digitalRead(dutyup) && dutycycle<100)
  {
    dutycycle+=10;
    EEPROM.put(dutyAd, dutycycle);
    showInfo();
    delay(150);
  }
  }
  Timer1.start(); 
  atint(waveform);
  //Timer1.attachInterrupt(wave);
  }
}
