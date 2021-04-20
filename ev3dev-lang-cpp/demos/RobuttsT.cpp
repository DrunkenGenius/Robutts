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
  void dead();

  void stop();
  void reset();

  bool initialized() const;

  void terminate_on_key();
  void panic_if_touched();

  void remote_loop();
  void drive_autonomously();

  unsigned int microsecond = 1000000;

  bool enemyColorDetected = false;
  bool obstacleDetected = false;

  int hpCounter = 3;
  int redColor = 5;
  int greenColor = 3;


protected:
  large_motor _motor_left;
  large_motor _motor_right;
  touch_sensor _sensor_touch;
  ultrasonic_sensor	_sensor_us_dist_cm;
  color_sensor    _sensor_col_color;
};

control::control() : _motor_left(OUTPUT_B),
                     _motor_right(OUTPUT_C),
                     _sensor_touch(INPUT_1)
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

void control::sweepArea()
{
  while (enemyColorDetected == false)
  {
    c.driveForward(1000, 200);
    c.turnRight(500, 200);
    c.turnLeft(500, 200);
  }
}

void control::avoidObstacle()
{
    //Set mode
    _sensor_us_dist_cm.set_mode(ultrasonic_sensor::mode_us_dist_cm);
    //Set value for sensor
    float ultrasoundValue = _sensor_us_dist_cm.value();		//x value er i mm ikke cm 
    //If statement that takes value from distance sensor and does something with the value
    if (ultrasoundValue <= 50.0f) {
        driveBackwards();
        turnLeft();
    }
}


void control::attackMode()
{
    //Set value for sensor
    int colorValue = _sensor_col_color.value();
    //If statement that takes value from color-sensor and does something with the value
    if (colorValue == redColor) {
        //I stedet for dette forneden, kunne vi køre en anden funktion, eks. 'driveForward();'
        _motor_left.set_speed_sp(-speed);

        _motor_right.set_speed_sp(-speed);
    }
    else if (colorValue == greenColor) {
        _motor_left.set_speed_sp(-speed);
    }
}


void control::dead()
{ //robot dies and spins around like a dead fly.
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

int main()
{
  control c;
  c.initialized();

  if (enemyColorDetected == true)
  {           //checking for enemy color
    case = 3; //going to attackMode
	}

	if (obastacleDetected == true){
		case = 2;
	}

	if (hpCounter == 0){
		case = 4;
	}

    /*
    sweepArea()
    {
    if (enemyColorDetected == false && ultrasoundValue <= 50.0f)
    {
    AvoidObstacles();
    }
    else if(enemyColorDetected == true && colorValue == 3)
    {
    attackMode();
    }
    else if (bumper sensor == true)
    {
    dead();
    }
    */
 

	

  switch (case) {
	  case 1:
      sweepArea(); //explores the area for enemies then goes to either avoidObstacle or attackMode
      break;

    case 2:
      avoidObstacle(); //backsoff and goes back to sweepArea
      break;

    case 3:
      attackMode(); //attacks the enenmy and goes to either sweep og avoidObstacle depending on surcummm.
      break;

    case 4:
      dead();
      break;

      
    
    default: dead();
  }

  while (1)
      {
        c.driveForward(500, 200);
        c.driveBackwards(500, 200);
        c.turnRight(500, 200);
        c.turnLeft(500, 200);
      }
      
}
