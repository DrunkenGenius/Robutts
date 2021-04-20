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
  void sweepArea();
  void avoidObstacle();
  void attackMode();
  void mainControl();
  void dead();
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

  int hpCounter = 3;
  int redColor = 5;
  int greenColor = 3;
  //Set value for sensor
  int colorValue;

  int speed = 500;
  float ultraSoundValue;
  float threshold = 80;
  float backOffthresHold = 80;

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

void control::sweepArea()
{
  driveForward(500, -1);
}

void control::avoidObstacle()
{
  while (ultraSoundValue < backOffthresHold)
  { //vi bakker indtil at vi er en bestemt længde væk fra væggen
    driveBackwards(500,-1);
  }
  turnLeft(500, 200);
}

void control::attackMode()
{
  //If statement that takes value from color-sensor and does something with the value
  if (colorValue == redColor)
  {
    //I stedet for dette forneden, kunne vi k�re en anden funktion, eks. 'driveForward();'
    while (bumperSensor == false)
    {
      driveForward(1000, -1);
    }
  }
  else if (colorValue == greenColor)
  {
    driveBackwards(1000, 200);
  }
}

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

  while (1)
  {
    updateSensorInput();
    if (enemyColorDetected == false && ultraSoundValue > threshold && bumperSensor == false && hpCounter > 0)
    {
      sweepArea();
    }
    else if (colorValue == greenColor)
    {
	  std::cout << "Attackmode" << std::endl;
      attackMode();
    }
    else if (ultraSoundValue < threshold)
    {
      std::cout << "US triggered" << std::endl;
      driveBackwards(500, 200);
      turnLeft(500, 400);
      
    }
    else if (bumperSensor == true)
    {
      std::cout << "BumperSensor triggered" << std::endl;
      driveBackwards(500, 200);
      turnLeft(500, 400);
    }

    else if (hpCounter == 0)
    {
       std::cout << "DEAD" << std::endl;
      dead();
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
