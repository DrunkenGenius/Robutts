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

  int state = 1;

  int hpCounter = 3;
  int redColor = 5;
  int greenColor = 3;
  int yellowColor = 4;
  int whiteColor = 6;
  int blackColor = 1;
    int noColor = 0;
    int brownColor = 7;
  //Set value for sensor
  int redColorValue;
  int greenColorValue;
  int blueColorValue;

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
  redColorValue = _sensor_col_color.value(0);
  greenColorValue = _sensor_col_color.value(1);
  blueColorValue = _sensor_col_color.value(2);
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
    
    if (bumperSensor == true)
    {
      std::cout << "Red: " << redColorValue << ", Green: " << greenColorValue << ", Blue: " << blueColorValue << std::endl;
    }
  }
}

      //--------------------------------------------------BEHAVIOUR END---------------------------------------//
      //--------------------------------------------------MAIN START ---------------------------------------//

      int main()
      {
        control c;
        c.initialized();
        c.mainControl();
      }
