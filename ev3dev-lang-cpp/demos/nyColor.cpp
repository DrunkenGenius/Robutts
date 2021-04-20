#include "ev3dev.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#ifndef NO_LINUX_HEADERS
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#endif

using namespace std;
using namespace ev3dev;


class control
{
public:
	control();
	void drive(int speed, int time = 0);

	bool initialized() const;

protected:
	large_motor     _motor_left;
	large_motor     _motor_right;
	color_sensor    _sensor_col_color;

};

control::control() :
	_motor_left(OUTPUT_B),
	_motor_right(OUTPUT_C)
{
}

bool control::initialized() const
{
	return (_motor_left.connected() &&
		_motor_right.connected() &&
		_sensor_col_color.connected());
}


void control::drive(int speed, int time)
{
	while (1) {
		int x = _sensor_col_color.value();
		if (x == 3) {
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
		else if (x == 5) {
			_motor_left.set_speed_sp(-speed);

			if (time > 0)
			{
				_motor_left.set_time_sp(time).run_timed();
			}
			else
			{
				_motor_left.run_forever();
			}
		}
	}
}

int main() {

	control c;
	c.initialized();
	c.drive(500, 1);

}