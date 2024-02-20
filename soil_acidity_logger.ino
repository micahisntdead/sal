#define Vref 4.95
#include <SD.h>
#include <SPI.h>
#include <DS3231.h>
#include <Wire.h>

File myFile;
DS3231  rtc(SDA, SCL);

float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
int pinCS = 53; // Pin 10 on Arduino Uno
unsigned long int avgValue;     //Store the average value of the sensor feedback
int i=0;
void setup()
{
    Serial.begin(9600);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    Serial.begin(9600);
    pinMode(pinCS, OUTPUT);
  
    // SD Card Initialization
    if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  rtc.begin();    
}

void loop()
{
    float sensorValue;
    int m;
    long sensorSum;
    int buf[10];                //buffer for read analog
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(A1);//Connect the PH Sensor to A1 port
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        int temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
       avgValue=0;
 
      for(int i=2;i<8;i++)                      //take the average value of 6 center sample
      avgValue+=buf[i];
    
     sensorValue =   avgValue/6;
     //Serial.print(sensorValue);               //dit kan aan als je de ruwe data wil aflezen
     //Serial.println(" ");

    Serial.print("crowtailA,");
    Serial.print("\t");
  Serial.print(rtc.getTimeStr());
  Serial.print(",");
  Serial.print(int(rtc.getTemp()));
  Serial.print(",");
  
        myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {   
    myFile.print("crowtailA,"); 
    myFile.print(rtc.getTimeStr());
    myFile.print(",");    
    myFile.print(int(rtc.getTemp()));
    myFile.print(",");  
    Serial.print(7-1000*(sensorValue-365)*Vref/59.16/1023,2);
    myFile.close(); // close the file
  }
  else {
    Serial.println("error opening test.txt");
  }

  delay(3000);
  Serial.println();

//BEGIN CODE CROWTAILB
{
        float sensorValue;
    int m;
    long sensorSum;
    int buf[10];                //buffer for read analog
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(A2);//Connect the PH Sensor to A2 port
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        int temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
       avgValue=0;
 
      for(int i=2;i<8;i++)                      //take the average value of 6 center sample
      avgValue+=buf[i];
    
     sensorValue =   avgValue/6;
     //Serial.print(sensorValue);               //dit kan aan als je de ruwe data wil aflezen
     //Serial.println(" ");

    Serial.print("crowtailB,");
    Serial.print("\t");
  Serial.print(rtc.getTimeStr());
  Serial.print(",");
  Serial.print(int(rtc.getTemp()));
  Serial.print(",");
        myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {   
    myFile.print("crowtailB,"); 
    myFile.print(rtc.getTimeStr());
    myFile.print(",");    
    myFile.print(int(rtc.getTemp()));
    myFile.print(",");  
    Serial.print(7-1000*(sensorValue-365)*Vref/59.16/1023,2);
    myFile.close(); // close the file
  }
  else {
    Serial.println("error opening test.txt");
  }

  delay(3000);
  Serial.println();

  //BEGIN CODE DFROBOT

    Serial.print("dfrobot,");
    Serial.print("\t");
  Serial.print(rtc.getTimeStr());
  Serial.print(",");
  Serial.print(int(rtc.getTemp()));
  Serial.print(",");
   
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A3);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6; //kalibreren
 float ph_act = -5.70 * volt + 15.5;
  Serial.println(ph_act);
   // if the file didn't open, print an error:
  
    myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {   
    myFile.print("mtr01,"); 
    myFile.print(rtc.getTimeStr());
    myFile.print(",");    
    myFile.print(int(rtc.getTemp()));
    myFile.print(",");  
    myFile.println(ph_act);
    myFile.close(); // close the file
  }
  else {
    Serial.println("error opening test.txt");
  }

  delay(3000);
}

}

  
