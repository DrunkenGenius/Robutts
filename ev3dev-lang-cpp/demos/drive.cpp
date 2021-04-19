#include "ev3dev.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#ifndef NO_LINUX_HEADERS
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#define KEY_RELEASE 0
#define KEY_PRESS   1
#define KEY_REPEAT  2
#endif

using namespace std;
using namespace ev3dev;


class control
{
public:
  control();

  void drive(int speed, int time=0);
  void turn(int direction);
  void stop();
  void reset();

  bool initialized() const;

  void terminate_on_key();
  void panic_if_touched();

  void remote_loop();
  void drive_autonomously();


protected:
  large_motor     _motor_left;
  large_motor     _motor_right;
  large_motor     _motor_front;
  touch_sensor    _sensor_touch;
  ultrasonic_sensor _sensor_us;
};

control::control() :
  _motor_left(OUTPUT_B),
  _motor_right(OUTPUT_C),
  _motor_front(OUTPUT_D)
{
}

bool control::initialized() const
{
  return (_motor_left .connected() &&
          _motor_right.connected() &&
          _sensor_touch.connected() &&
		  _sensor_us.connected());
}

void control::drive(int speed, int time)
{
	while(1) {
		bool x = _sensor_touch.value();
		float usVal = _sensor_us.value();
		cout << usVal;
		if (x){
		  _motor_left.set_speed_sp(-speed);
		
		  _motor_right.set_speed_sp(-speed);
		
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
  }
}

int main(){
	
	control c;
	c.initialized();
	c.drive( 500, -1);

}