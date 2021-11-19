#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Talkie.h"
#include "Vocab_US_Large.h"
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include "SerialMP3Player.h"
#include <math.h>
#define TX 5
#define RX 6
#define CE_PIN  9
#define CSN_PIN 10
double endpointlng = 0;
double endpointlat = 0;
unsigned long timer = 0;
bool isLeft = 0;
bool isRight = 0;
double anglegyroandgps[4];
SoftwareSerial serial_connection(8, 7); //RX=pin 7, TX=pin 8
TinyGPSPlus gps;//This is the GPS object]
SerialMP3Player mp3(RX, TX);

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

void setup()
{
  Serial.begin(9600);
  Serial.println("Receiver Setup Starting");

  radio.begin(); //RF starting
  radio.setRetries(0, 15);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  mp3.begin(9600);        // start mp3-communication
  delay(500);             // wait for init
  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(500);             // wait for init
  Serial.println("MP3 Started");

  //Gps setup
  serial_connection.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Started");

  Serial.println("Receiver Setup Complete");
}

void loop()
{

  /*if (radio.isChipConnected())
    {
    Serial.println("Chip found");
    }*/

  while (serial_connection.available()) //While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }

  /*if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
    {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:");
    Serial.println(gps.location.lng(), 6);
    Serial.println("Speed MPH:");
    Serial.println(gps.speed.mph());
    Serial.println("Altitude Feet:");
    Serial.println(gps.altitude.feet());
    Serial.println("");
    }*/

  if (radio.available())//if there's data available on rf radio
  {
    radio.read(anglegyroandgps, sizeof(anglegyroandgps));

    double turnFlag = 0;
    turnFlag = anglegyroandgps[0];

    Serial.print("Turn signal pilot bike : ");
    Serial.println(turnFlag);
    Serial.println();

    int timebike = 0;
    int numberOfClicks = anglegyroandgps[3];

    Serial.print("Number of clicks : ");
    Serial.println(numberOfClicks);

    if (turnFlag == 1) //Turn left
    {
      Serial.println("Turn Left.............................<<");
      //unsigned long StartTime = millis();
      mp3.play(012); //left
      sayNumber(numberOfClicks);
      mp3.play(014);//

      endpointlng =  anglegyroandgps[2];
      endpointlat =  anglegyroandgps[1];
    }
    else if (turnFlag == 2) //Turn right
    {
      Serial.println("Turn Right.........................>>");
      mp3.play(013);//right
      sayNumber(numberOfClicks);
      mp3.play(014);//c

      endpointlng =  anglegyroandgps[2];
      endpointlat =  anglegyroandgps[1];
    }

    double guidedbikelat = gps.location.lat();
    double guidedbikelng = gps.location.lng();

    Serial.print("Pilot bike Lng: ");
    Serial.print(endpointlng, 6);
    Serial.print(", Lat: ");
    Serial.println(endpointlat, 6);

    Serial.print("Guided bike Lng: ");
    Serial.print(guidedbikelng, 6);
    Serial.print(", Lat: ");
    Serial.println(guidedbikelat, 6);
    double latDifference = 0;
    double longDifference = 0;
    do
    {
      latDifference = abs(gps.location.lat() - endpointlat);
      longDifference = abs(gps.location.lng() - endpointlng);
      Serial.print(" Lat Difference:");
      Serial.println(latDifference, 6);
      Serial.print(" Long Difference:");
      Serial.println(longDifference, 6);
      delay(500);

    } while (not(latDifference <= 0.0003) && not(longDifference <= 0.0003));//changed threshold from 0.0003 to 0.0002

    Serial.print("Now");
    mp3.play(015);//now
  }

  if ((millis() - timer) > 5000) //if less than 5 sec then it migh be just turning back to get it straight
  {
    isRight = 0;
    isLeft = 0;
    timer = millis();
  }
}



void sayNumber(int number)
{
  switch (number) {
    case 1:
      mp3.play(001);//1
      break;
    case 2:
      mp3.play(002); //2
      break;
    case 3:
      mp3.play(003); //3
      break;
    case 4:
      mp3.play(004); //4
      break;
    case 5:
      mp3.play((byte)9); //5
      break;
    case 6:
      mp3.play((byte)8);  //6
      break;
    case 7:
      mp3.play(007); //7
      break;
    case 8:
      mp3.play(006); //8
      break;
    case 9:
      mp3.play(005); //9
      break;
  }
}


/*
  Input: Latitude bike 1, Longitude bike1, Lati
  Output: Time in seconds
  Reason: Calculates time between two moving bikes
*/
double calculateTimeBetweenTwoBikes(double lon1, double lon2, double lat1, double lat2, double speed2)
{
  double distBtwnBikes = calcDist(lon1, lon2, lat1, lat2);
  if (speed2 == 0) //assume bike is moving at very low speed of 1 m / hour
  {
    speed2 = 0.01;
  }
  double timeBtwnBikes = (distBtwnBikes / speed2) * 3600;

  Serial.println();
  Serial.print("Longitude pilot bike: ");
  Serial.println(lon1, 6);
  Serial.print("Latitude pilot bike: ");
  Serial.println(lat1, 6);
  Serial.print("Longitude guided bike: ");
  Serial.println(lon2, 6);
  Serial.print("Latitude guided bike: ");
  Serial.println(lat2, 6 );
  Serial.print("Distance between bikes: ");
  Serial.println (distBtwnBikes, 6);
  Serial.print("Speed of guided bike: ");
  Serial.println (speed2, 6);
  Serial.print("Time between Bikes in seconds: ");
  Serial.println(timeBtwnBikes);
  return timeBtwnBikes;
}

double calcDist(double lon1, double lon2, double lat1, double lat2)
{
  double R = 6371.00;
  double toDegrees = 57.295779;

  char sb[10]; //This is a haversine based distance calculation formula
  //This portion converts the current and destination GPS coords from decDegrees to Radians
  double  lonR1 = lon1 * (PI / 180);
  double lonR2 = lon2 * (PI / 180);
  double latR1 = lat1 * (PI / 180);
  double latR2 = lat2 * (PI / 180);

  //This portion calculates the differences for the Radian latitudes and longitudes and saves them to variables
  double dlon = lonR2 - lonR1;
  double dlat = latR2 - latR1;

  //This portion is the Haversine Formula for distance between two points. Returned value is in KM
  double a = (sq(sin(dlat / 2))) + cos(latR1) * cos(latR2) * (sq(sin(dlon / 2)));
  double e = 2 * atan2(sqrt(a), sqrt(1 - a)) ;
  double d = R * e;

  return d;
}
