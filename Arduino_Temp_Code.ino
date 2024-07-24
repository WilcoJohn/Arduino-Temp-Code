#include "max6675.h"

struct ThermoCouple
{
  const int SO;                     // custom struct to define SCK, CS and SO for amplifier
  const int CS;
  const int SCK;
};


ThermoCouple thermo1{2,3,4};
ThermoCouple thermo2{5,6,7};
ThermoCouple thermo3{8,9,10};
ThermoCouple thermo4{11,12,13};

MAX6675 sensor1{ thermo1.SCK, thermo1.CS, thermo1.SO };     // initiate the sensore
MAX6675 sensor2{ thermo2.SCK, thermo2.CS, thermo2.SO };
MAX6675 sensor3{ thermo3.SCK, thermo3.CS, thermo3.SO };
MAX6675 sensor4{ thermo4.SCK, thermo4.CS, thermo4.SO };

float temp1{};  // pre-allocate data points for temperatures
float temp2{};
float temp3{};
float temp4{};


double timeNow{ 0.0f };
int iterator{ 0 };
constexpr int interval{ 2  };           





// Print function

static void printTemp(double time, float temp1=9999.9f,float temp2=9999.9f, float temp3=9999.9f, float temp4=9999.9f) noexcept
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

  
  // Print the stuff

  Serial.print(timeHour);
  Serial.print(" hr - ");
  Serial.print(timeMin);
  Serial.print(" min - ");
  Serial.print(timeSecNow);

  Serial.print(" sec, ");
  Serial.print(time/60);

  Serial.print(", Tbot, ");
  Serial.print(temp1);
  Serial.print(", Tcat, ");
  Serial.print(temp2);
  Serial.print(", Tcond, ");
  Serial.print(temp3);
  Serial.print(", T3top, ");
  Serial.println(temp4);
}

void setup() 
{
  Serial.begin(9600); // 9600 serial baud data transfer rate

  Serial.println("Time,Time_min,Cat1,Sensor1,Cat2,Sensor2,Cat3,Sensor3,Cat4,Sensor4");    // indicate column names
}

void loop() 
{
  
  if (iterator%interval == 0)
  {
      iterator = 0; // reset 

      temp1 = sensor1.readCelsius();
      temp2 = sensor2.readCelsius();
      temp3 = sensor3.readCelsius();
      temp4 = sensor4.readCelsius();

      timeNow = ((double)millis())/1000.0 ;               // convert millis -> seconds

      printTemp(timeNow, temp1, temp2, temp3, temp4);
  }
  ++iterator;
  delay(1000); //1e3 millisecond delay -> 1 sec delay


}
