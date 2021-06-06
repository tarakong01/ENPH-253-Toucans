#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define PIN PA7
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible

#define SAMPLE_SIZE 200
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int readWave(int waveform[]);

void setup()
{
  pinMode(PIN, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  delay(500);
}

void loop()
{
  // variable declaration
  int Waveform[200] = {0};
  double onek[SAMPLE_SIZE*2] = {0};
  double tenk[SAMPLE_SIZE*2] = {0};
  double onekcorr[SAMPLE_SIZE]={0};
  double tenkcorr[SAMPLE_SIZE]={0};

  // Read wave form
  int elapsed_time = readWave(Waveform);

  double onekT = SAMPLE_SIZE/(double)elapsed_time;
  for (int i=0; i<2*SAMPLE_SIZE; i++){
    onek[i]=sin(i*TWO_PI/onekT);
    tenk[i]=sin(i*10*TWO_PI/onekT); //generate 1khz and 10khz waveforms
  }

  for (int k=0; k<SAMPLE_SIZE; k++) {
    onekcorr[k]=0;
    tenkcorr[k]=0;
    for (int i=0;i<SAMPLE_SIZE;i++){
      onekcorr[k]+=Waveform[i]*onek[k+i]; //cross correlate waveform with 1 and 10k
      tenkcorr[k]+=Waveform[i]*tenk[k+i];
    }
  }

  double max_one=0;
  double max_ten=0;
  for (int i=0;i<SAMPLE_SIZE;i++) {
    if (onekcorr[i]>max_one) max_one=onekcorr[i]; // find correlation peaks
    if (tenkcorr[i]>max_ten) max_ten=tenkcorr[i];
  }

  // print the wave frequency on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  if (max_one > max_ten) {
    display.println("1kHz");
  } else {
    display.println("10kHz");
  }
  display.display();
}

/**
 * Reads waveforms and stores in an array
 * Param: Waveforms[] the array to store analog values
 * 
 * Return: the elapsed time in ms
 */
int readWave(int Waveform[])
{
  int time1 = millis();
  for (int i = 0; i < SAMPLE_SIZE; i++)
  {
    int val = analogRead(PIN);
    Waveform[i] = val;
  }
  int time2 = millis();
  return time2 - time1;
}
