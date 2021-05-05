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
	float ArrayAverage(int colorValues[], int colorValue);
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

	int hitCounter = 0;
	int redColor = 5;
	int greenColor = 3;
	//Set value for sensor
	int colorValue;

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
	ultraSoundValue = _sensor_us_dist_cm.value(); //x value er i mm ikke cm
	colorValue = _sensor_col_color.value();
	bumperSensor = _sensor_touch.value();
}

float control::ArrayAverage(int colorValues[], int colorValue)
{
	int sum = 0;
	for (int i = 9; i >= 0; i--)
	{
		if (i != 0)
		{
			colorValues[i] = colorValues[i - 1];
		}
		else
		{
			colorValues[0] = colorValue;
		}
		sum += colorValues[i];
	}

	float average = sum / 10;
	return average;
}

void control::mainControl()
{
	//Set mode
	_sensor_us_dist_cm.set_mode(ultrasonic_sensor::mode_us_dist_cm);
	//_sensor_col_color.set_mode(color_sensor::mode_col_color);
	_sensor_col_color.set_mode(color_sensor::mode_rgb_raw);

	int redColorArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int greenColorArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int blueColorArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int distanceArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	while (1)
	{
		updateSensorInput();

		float redAverage = ArrayAverage(redColorArray, redColorValue);
		float greenAverage = ArrayAverage(greenColorArray, greenColorValue);
		float blueAverage = ArrayAverage(blueColorArray, blueColorValue);
		float distanceAverage = ArrayAverage(distanceArray, ultraSoundValue);

		/*if (greenColor == false && ultraSoundValue > threshold && bumperSensor == false && hpCounter > 0)
		{
		//sweepArea();
		  state = 1;
		}
		else if (colorValue == greenColor)
		{
		  std::cout << "Attackmode" << std::endl;
		  state = 3;
		}*/
		if (ultraSoundValue < threshold)
		{
			std::cout << "US triggered" << std::endl;
			state = 2;
		}
		else if (bumperSensor == true)
		{
			std::cout << "BumperSensor triggered" << std::endl;
			hitCounter++;
			state = 2;
			if (hitCounter >= 3)
			{
				state = 4;
			}
		}

		// De mulige states bliver sat i enumeratoren i toppen enum state { sweep, charge, avoid, changepos, dead };
		switch (state)
		{
		case 1:
			if (!sweeping)
			{
				std::cout << "StartSweeping" << std::endl;
				std::cout << colorValue << std::endl;
				startSweepTime = time(NULL);
				endSweepTime = startSweepTime + 5;
				turnLeft(100, -1);
				sweeping = true;
			}
			else if (sweeping)
			{
				std::cout << "Sweeping" << std::endl;
				if (colorValue == greenColor)
				{
					//If enemy -> charge
					state = 3;
					sweeping = false;
				}
				else if (colorValue == redColor)
				{
					//If friend -> avoid
					state = 2;
					sweeping = false;
				}
				else if (endSweepTime < time(NULL))
				{
					//Hvis vi går over end time bruger vi modulu til at skifte imellem at køre ligeud og dreje til venstre indtil vi detecter en color, eller skal avoide
					if (((time(NULL) - endSweepTime) % 6) > 2)
					{
						driveForward(100, -1);
					}
					else
					{
						turnLeft(100, -1);
					}
				}
			}
			break;

		case 2:
			std::cout << "Avoid" << std::endl;
			sweeping = false;
			//avoidObstacle(); //backsoff and goes back to sweepArea
			driveBackwards(100, -1);
			if (ultraSoundValue > threshold)
			{
				turnLeft(300, 400);
				usleep(1 * microsecond);
				state = 1;
			}
			break;

		case 3:
			std::cout << "Attack" << std::endl;
			sweeping = false;
			//attackMode(); //attacks the enenmy and goes to either sweep og avoidObstacle depending on surcummm.
			int initialThreshold = threshold;
			threshold = 0;
			driveForward(500, -1);
			//Colorsensor skal være over threshold ellers gå i sweep state
			if (bumperSensor == true)
			{
				threshold = initialThreshold;
				hitCounter++;
				if (hitCounter >= 3)
					state = 4;
				state = 2;
			}
			break;
		case 4:
			//Play victory sound
			turnRight(500, -1);
			sound::speak("Hello, I am Robot!", true);
			sound::tone({{392, 350, 100}, {392, 350, 100}, {392, 350, 100}, {311.1, 250, 100}, {466.2, 25, 100}, {392, 350, 100}, {311.1, 250, 100}, {466.2, 25, 100}, {392, 700, 100}, {587.32, 350, 100}, {587.32, 350, 100}, {587.32, 350, 100}, {622.26, 250, 100}, {466.2, 25, 100}, {369.99, 350, 100}, {311.1, 250, 100}, {466.2, 25, 100}, {392, 700, 100}, {784, 350, 100}, {392, 250, 100}, {392, 25, 100}, {784, 350, 100}, {739.98, 250, 100}, {698.46, 25, 100}, {659.26, 25, 100}, {622.26, 25, 100}, {659.26, 50, 400}, {415.3, 25, 200}, {554.36, 350, 100}, {523.25, 250, 100}, {493.88, 25, 100}, {466.16, 25, 100}, {440, 25, 100}, {466.16, 50, 400}, {311.13, 25, 200}, {369.99, 350, 100}, {311.13, 250, 100}, {392, 25, 100}, {466.16, 350, 100}, {392, 250, 100}, {466.16, 25, 100}, {587.32, 700, 100}, {784, 350, 100}, {392, 250, 100}, {392, 25, 100}, {784, 350, 100}, {739.98, 250, 100}, {698.46, 25, 100}, {659.26, 25, 100}, {622.26, 25, 100}, {659.26, 50, 400}, {415.3, 25, 200}, {554.36, 350, 100}, {523.25, 250, 100}, {493.88, 25, 100}, {466.16, 25, 100}, {440, 25, 100}, {466.16, 50, 400}, {311.13, 25, 200}, {392, 350, 100}, {311.13, 250, 100}, {466.16, 25, 100}, {392.00, 300, 150}, {311.13, 250, 100}, {466.16, 25, 100}, {392, 700}},
						true);
			break;
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
