#include "max6675.h"
                  //    SO  CS  SCK       - different pins of MAX6675 temperature module for arduino
MAX6675 sensors[] {   { 2,   3,  4},     // initiate array of sensors
                      { 5,   6,  7},
                      { 8,   9,  10},
                      { 11,  12, 13} };

constexpr int nSensors{ (int)(sizeof(sensors)/sizeof(sensors[0])) }; // C-cast array of sensors to int as sizeof returns size_t
char* sensorNames[] {  "Tbot", "Tmid", "Ttop", "Teehee" };

float temp[nSensors]{};  // pre-allocate data points for temperatures


double timeNow{ 0.0f };
int iterator{ 0 };
constexpr int interval{ 2 };           





// Print function
static void printTemp(double time, float temperature[]) noexcept
{
  static long int timeHour{ 0 };
  static long int timeMin{ 0 };
  
  static double timeSecPrev{ ((double)millis())/1000.0 };     // C-cast to double, get start time
  static double timeSecNow{ 0 };

  timeSecNow += time - timeSecPrev;                           // update time steps
  timeSecPrev = time;

  if (timeSecNow > 60)                                        // reset time variables
  {
    ++timeMin;
    timeSecNow -= 60.0;
  }
  if (timeMin >= 60)
  {
    timeMin -= 60;
    ++timeHour;
  }

  
  // Print time
  Serial.print(timeHour);
  Serial.print(" hr - ");
  Serial.print(timeMin);
  Serial.print(" min - ");
  Serial.print(timeSecNow);

  Serial.print(" sec, ");
  Serial.print(time/60);


  // print sensors
  for (int i{0}; i < nSensors; ++i)
  {
    Serial.print(", ");
    Serial.print(sensorNames[i]);
    Serial.print(" , ");
    Serial.print(temp[i]);
  }
  Serial.println();

}

void setup() 
{
  Serial.begin(9600); // 9600 serial baud data transfer rate

  Serial.print("Time,Time_min");
  for(int i{0}; i < nSensors; ++i)
  {
    Serial.print(",Cat");
    Serial.print(i+1);
    Serial.print(",Sensor");
    Serial.print(i+1);
  }
  Serial.println();
}

void loop() 
{
  
  if (iterator%interval == 0)
  {
      iterator = 0;                                       // reset iterator
      for (int i{0}; i < nSensors; ++i)                   // read sensors
      {
        temp[i] = sensors[i].readCelsius();
      }

      timeNow = ((double)millis())/1000.0 ;               // convert time millis -> seconds
      printTemp(timeNow, temp);                           // print the temperature
  }
  ++iterator;
  delay(1000);                                            // 1e3 millisecond delay -> 1 sec delay, 
                                                          // require 1 sec delay for sensors to update


}
