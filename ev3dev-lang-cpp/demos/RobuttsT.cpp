#include "ev3dev.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#ifndef NO_LINUX_HEADERS
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#define KEY_RELEASE 0
#define KEY_PRESS 1
#define KEY_REPEAT 2
#endif

using namespace std;
using namespace ev3dev;

class control
{
public:
  control();

  void driveForward(int speed, int time = 0);
  void driveBackwards(int speed, int time = 0);
  void turnRight(int speed, int time = 0);
  void turnLeft(int speed, int time = 0);
  void mainControl();
  void updateSensorInput();

  void stop();
  void reset();

  bool initialized() const;

  void terminate_on_key();
  void panic_if_touched();

  void remote_loop();
  void drive_autonomously();

  unsigned int microsecond = 1000000;

  bool enemyColorDetected = false;
  bool bumperSensor = false;

  int case = 1;

  int hpCounter = 3;
  int redColor = 5;
  int greenColor = 3;
  //Set value for sensor
  int colorValue;

  int speed = 500;
  float ultraSoundValue;
  float threshold = 80;
  float backOffthresHold = 80;

  //Sweep
  time_t currentTime;
  time_t startSweepTime;
  time_t endSweepTime;
  bool sweeping = false;

  //--------------------------------------------------VARIABLES AND VOIDS  END ---------------------------------------//
  //--------------------------------------------------SENSOR INSTANTIATION START ---------------------------------------//
protected:
  large_motor _motor_left;
  large_motor _motor_right;
  touch_sensor _sensor_touch;
  ultrasonic_sensor _sensor_us_dist_cm;
  color_sensor _sensor_col_color;
};

control::control() : _motor_left(OUTPUT_B),
                     _motor_right(OUTPUT_C),
                     _sensor_touch(INPUT_1),
                     _sensor_col_color(INPUT_2),
                     _sensor_us_dist_cm(INPUT_3)
{
}

bool control::initialized() const
{
  return (_motor_left.connected() &&
          _motor_right.connected() &&
          //Color sensor
          _sensor_col_color.connected() &&
          //Ultralydssensor
          _sensor_us_dist_cm.connected() &&
          _sensor_touch.connected());
}
//--------------------------------------------------SENSOR INSTANTIATION END ---------------------------------------//
//---------------------------------------------------BASIC MOVEMENT START--------------------------------------------//

void control::driveForward(int speed, int time)
{
  _motor_left.stop_action();
  _motor_right.stop_action();
  _motor_left.set_speed_sp(speed);
  _motor_right.set_speed_sp(speed);

  if (time > 0)
  {
    _motor_left.set_time_sp(time).run_timed();
    _motor_right.set_time_sp(time).run_timed();
  }
  else
  {
    _motor_left.run_forever();
    _motor_right.run_forever();
  }
}

void control::driveBackwards(int speed, int time)
{
  _motor_left.stop_action();
  _motor_right.stop_action();
  _motor_left.set_speed_sp(-speed);
  _motor_right.set_speed_sp(-speed);
  usleep(1 * microsecond); //sleeps for 3 second
  if (time > 0)
  {
    _motor_left.set_time_sp(time).run_timed();
    _motor_right.set_time_sp(time).run_timed();
  }
  else
  {
    _motor_left.run_forever();
    _motor_right.run_forever();
  }
}

void control::turnRight(int speed, int time)
{
  _motor_left.stop_action();
  _motor_right.stop_action();
  _motor_left.set_speed_sp(speed);
  _motor_right.set_speed_sp(-speed);
  usleep(1 * microsecond); //sleeps for 1 second
  if (time > 0)
  {
    _motor_left.set_time_sp(time).run_timed();
    _motor_right.set_time_sp(time).run_timed();
  }
  else
  {
    _motor_left.run_forever();
    _motor_right.run_forever();
  }
}

void control::turnLeft(int speed, int time)
{
  _motor_left.stop_action();
  _motor_right.stop_action();
  _motor_left.set_speed_sp(-speed);
  _motor_right.set_speed_sp(speed);
  usleep(1 * microsecond); //sleeps for 1 second
  if (time > 0)
  {
    _motor_left.set_time_sp(time).run_timed();
    _motor_right.set_time_sp(time).run_timed();
  }
  else
  {
    _motor_left.run_forever();
    _motor_right.run_forever();
  }
}

//---------------------------------------------------BASIC MOVEMENT END--------------------------------------------//

//--------------------------------------------------BEHAVIOUR START------- ---------------------------------------//

void control::updateSensorInput()
{
  //Set value for sensor
  ultraSoundValue = _sensor_us_dist_cm.value(); //x value er i mm ikke cm
  colorValue = _sensor_col_color.value();
  bumperSensor = _sensor_touch.value();
}

void control::mainControl()
{
  //Set mode
  _sensor_us_dist_cm.set_mode(ultrasonic_sensor::mode_us_dist_cm);
  _sensor_col_color.set_mode(color_sensor::mode_rgb_raw);

  while (1)
  {
    updateSensorInput();
    if (greenColor == false && ultraSoundValue > threshold && bumperSensor == false && hpCounter > 0)
    {
    //sweepArea();
      case = 0;
    }
    else if (colorValue == greenColor)
    {
      std::cout << "Attackmode" << std::endl;
      case = 2;
    }
    else if (ultraSoundValue < threshold)
    {
      std::cout << "US triggered" << std::endl;
      case = 1;
    }
    else if (bumperSensor == true)
    {
      std::cout << "BumperSensor triggered" << std::endl;
      case = 1;
    }
    else if (hpCounter == 0)
    {
      std::cout << "DEAD" << std::endl;
      case = -1;
    }

// De mulige cases bliver sat i enumeratoren i toppen enum case { sweep, charge, avoid, changepos, dead };
    switch (case) {
	  case 0:
        if (!sweeping)
        {
          startTime = time(NULL);
          endtime = startTime + 5;
          turnLeft(100, -1);
          sweeping = true;
        }
        else if (sweeping)
        {
          if (colorSensor == greenColor)
          {
          case = 2;
          sweeping = false;
        }else if(endtime < time(NULL)){
          //Hvis vi går over end time bruger vi modulu til at skifte imellem at køre ligeud og dreje til venstre indtil vi detecter en color, eller skal avoide
          if(((time(NULL) - endtime)%6) > 2){
            driveForward(100, -1)
          }else{
            turnLeft(100, -1);
          } 
        }
      }
      break;

    case 1:
            //avoidObstacle(); //backsoff and goes back to sweepArea
            driveBackwards(100, -1);
            if (ultraSoundValue > threshold)
            {
              turnLeft(100, 400);
              usleep(0.4 * microsecond);
            }
            break;

    case 2:
      //attackMode(); //attacks the enenmy and goes to either sweep og avoidObstacle depending on surcummm.
      break;

    case 3:
      int initialThreshold = threshold;
      threshold = 0;
      driveForward(500, -1);
       if (bumperSensor == true)
       {
        threshold = initialThreshold;
       }
       break;

    default:
      _motor_left.stop_action();
      _motor_right.stop_action();
      break;
     }
   }
}

      void control::dead()
      { //robot dies and spins around like a dead fly.
        _motor_left.stop_action();
        _motor_right.stop_action();
      }

      //--------------------------------------------------BEHAVIOUR END---------------------------------------//
      //--------------------------------------------------MAIN START ---------------------------------------//

      int main()
      {
        control c;
        c.initialized();
        c.mainControl();
      }
