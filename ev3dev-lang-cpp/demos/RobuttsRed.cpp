#include "ev3dev.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include<string>

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

	//http://www.legoengineering.com/ev3-sensors/ 
	//https://docs.ev3dev.org/projects/lego-linux-drivers/en/ev3dev-stretch/sensor_data.html#lego-ev3-color 
	//https://docs.ev3dev.org/projects/lego-linux-drivers/en/ev3dev-stretch/sensor_data.html#lego-ev3-touch 
	//https://docs.ev3dev.org/projects/lego-linux-drivers/en/ev3dev-stretch/sensor_data.html#lego-nxt-us 

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

//This function starts and stops the motors for a specified time to make the robot drive forward for a moment.
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
//This function starts and stops the motors for a specified time to make the robot drive backwards for a moment.
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

//This function starts and stops the motors individually for a specified time to make the robot turn right aproximately 90 degrees.
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

//This function starts and stops the motors individually for a specified time to make the robot turn left aproximately 90 degrees.
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
	redColorValue = _sensor_col_color.value(0);
	greenColorValue = _sensor_col_color.value(1);
	blueColorValue = _sensor_col_color.value(1);
	bumperSensor = _sensor_touch.value();
}

//This function is a calculator that returns the average value of 10 values in an array, which will be used later for the colors and the distance.
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

	//An array of the 10 latest values of every color and the distance
	int redColorArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int greenColorArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int blueColorArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int distanceArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	float calibratedUSValue = 0;

	bool redColorFound = false;
	bool greenColorFound = false;

	bool allyFound = false;
	bool enemyFound = false;

	while (1)
	{
		updateSensorInput();
		//Simpel kalibrering af ultralydssensor
		/*DISTANCE CM
		US SENSOR        80.2    70.2    59.8    39.9    19.9    14.9    12.4    10.1    4.9    3
		ACTUAL VALUE        78.3    68.8    59.5    40.1    20.5    15.4    13    10.9    6    3.3*/
		//Måske behøves kalibrering ikke da det er en meget lav fejl margin
		if (ultraSoundValue < 40)
		{
			calibratedUSValue = ultraSoundValue + 0.58571;
		}
		else
		{
			calibratedUSValue = ultraSoundValue - 1.2;
		}

		float redAverage = ArrayAverage(redColorArray, redColorValue);
		float greenAverage = ArrayAverage(greenColorArray, greenColorValue);
		float blueAverage = ArrayAverage(blueColorArray, blueColorValue);
		float distanceAverage = ArrayAverage(distanceArray, ultraSoundValue);

		/*
					Actual value        		 80.2  70.2   59.8       39.9         19.9        14.9       12.4         10.1      4.9        3
					Adjusted value        		  79    69    58.6    40.48571    20.48571    15.48571    12.98571    10.68571    5.48571    3.58571
			Color sensor values 0-1024      R      0     0     0           0            0           0           1   	    2       15         72
											G      0     0     0           0            0           0           0   	    0       1          8
											B      0     0     0           0            0           0           0   	    0       2          8
		*/

		//12.98 is the first value so every range above this has to have a color threshold above this
		//Vi tjekker for rød vs distance for at være sikre på at den farve vi kigger på er rød og ikke en random value, selvom value er et mean.
		//Røds tendens gør muligvis den her algoritme kalibrering ting overflødig- kunne give mening hvis der var flere farver end rød og grøn
		//Det giver mening at vi har kalibreret for vores specifikke røde farve for at vi ikke angriber en anden rød farve.

		//Optimalt ville vi have lavet flere målinger på farven og derfra kunne vi lave en funktion som kan bestemme hvornår en farve er rød i relation til distancen.
		if (calibratedUSValue <= 12.98 && calibratedUSValue >= 10.68 && redAverage >= 1 && greenAverage < 1)
		{
			redColorFound = true;
		}
		else if (calibratedUSValue >= 5.48 && redAverage >= 2 && greenAverage <= 1)
		{
			redColorFound = true;
		}
		else if (calibratedUSValue >= 3.585 && redAverage >= 8 && greenAverage <= 5)
		{
			redColorFound = true;
		}
		else if (calibratedUSValue <= 3.58 && redAverage >= 25 && greenAverage <= 15)
		{
			redColorFound = true;
		}
		else
		{
			//std::cout << "nothingDetected" << std::endl;
			redColorFound = false;
		}


		//		Distance	15,4	10	7,6		5,5		3,6		3
		//			r		0		0	1		4		10		45
		//			g		1		2	4		8		22		112
		//			b		1		2	3		6		13		47

				//Mangler målinger
				//Optimalt ville vi have lavet flere målinger på farven og derfra kunne vi lave en funktion som kan bestemme hvornår en farve er rød i relation til distancen.
		if (ultraSoundValue <= 15.4 && ultraSoundValue >= 10 && greenAverage >= 1 && redAverage < 1)
		{
			std::cout << "green1" << std::endl;
			greenColorFound = true;
		}
		else if (ultraSoundValue >= 7.6 && greenAverage >= 2 && redAverage < greenAverage * 0.5)
		{
			std::cout << "green2" << std::endl;
			greenColorFound = true;
		}
		else if (ultraSoundValue >= 5.5 && greenAverage >= 4 && redAverage <= greenAverage * 0.5)
		{
			std::cout << "green3" << std::endl;
			greenColorFound = true;
		}
		else if (ultraSoundValue >= 3.6 && greenAverage >= 8 && redAverage <= greenAverage * 0.5)
		{
			std::cout << "green4" << std::endl;
			greenColorFound = true;
		}
		else if (ultraSoundValue >= 3 && greenAverage >= 20 && redAverage < greenAverage * 0.5)
		{
			std::cout << "green5" << std::endl;
			greenColorFound = true;
		}
		else if (ultraSoundValue <= 3 && greenAverage >= 60 && redAverage <= greenAverage * 0.5)
		{
			std::cout << "green6" << std::endl;
			greenColorFound = true;
		}
		else
		{
			std::cout << "nothing" << std::endl;
			//std::cout << "nothingDetected" << std::endl;
			greenColorFound = false;
		}


		//Ally og enemy bools sættes til grøn eller rød color found hver især, efter hvilket team robotten skal være på.
		enemyFound = greenColorFound;
		allyFound = redColorFound;

		//Undgår at avoide når vi er i charge mode eller når vi har vundet
		if (state != 3 && state != 4 && state != 5) {
			//I alle andre states skal vi kunne gå i avoid mode når vi er under threshold på US eller vi bumper sensor
			if (ultraSoundValue < threshold)
			{
				std::cout << "US triggered" << std::endl;
				state = 2;
			}
			else if (bumperSensor == true)
			{
				std::cout << "BumperSensor triggered" << std::endl;
				state = 2;
			}
		}

		// De mulige states bliver sat i enumeratoren i toppen enum state { sweep, charge, avoid, changepos, dead };
		switch (state)
		{
		case 1:
			//Start timer
			if (!sweeping)
			{
				std::cout << "StartSweeping" << std::endl;
				//std::cout << colorValue << std::endl;
				startSweepTime = time(NULL);
				endSweepTime = startSweepTime + 5;
				turnLeft(100, -1);
				sweeping = true;
			}
			else if (sweeping)
			{
				//std::cout << "Sweeping" << std::endl;
				if (enemyFound)
				{
					//If enemy -> charge
					state = 3;
					sweeping = false;
				}
				else if (allyFound)
				{
					//If friend -> avoid
					state = 2;
					sweeping = false;
				}
				else if (endSweepTime < time(NULL))
				{
					//Hvis vi går over end time bruger vi modulu til at skifte imellem at køre ligeud og dreje til venstre indtil vi detecter en color, eller skal avoide
					if (((time(NULL) - endSweepTime) % 8) > 2)
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
			driveBackwards(100, 2000);
			if (ultraSoundValue > threshold)
			{
				turnLeft(300, 400);
				usleep(1 * microsecond);
				state = 1;
			}
			break;

		case 3: {
			std::cout << "Attack" << std::endl;
			sweeping = false;
			//attackMode(); //attacks the enenmy and goes to either sweep og avoidObstacle depending on surcummm.
			int initialThreshold = threshold;
			threshold = 0;
			driveForward(300, -1);
			//Colorsensor skal være over threshold ellers gå i sweep state
			if (!enemyFound) {
				state = 1;
			}
			else if (bumperSensor == true)
			{
				threshold = initialThreshold;
				hitCounter++;
				//std::cout << "Point Gained" << std::endl;
				//sound::speak("Point Gained", true);
				string str = to_string(hitCounter);
				//sound::speak(str, true);
				//Win eller avoid
				if (hitCounter >= 3) {
					state = 4;
				}
				else {
					state = 2;
				}

			}
		}
			  break;

		case 4:
			//Play victory sound
			turnRight(500, -1);
			sound::speak("Hahaha my team won!", true);
			sound::tone({ {392, 350, 100}, {392, 350, 100}, {392, 350, 100}, {311.1, 250, 100}, {466.2, 25, 100}, {392, 350, 100}, {311.1, 250, 100}, {466.2, 25, 100}, {392, 700, 100}, {587.32, 350, 100}, {587.32, 350, 100}, {587.32, 350, 100}, {622.26, 250, 100}, {466.2, 25, 100}, {369.99, 350, 100}, {311.1, 250, 100}, {466.2, 25, 100}, {392, 700, 100}, {784, 350, 100}, {392, 250, 100}, {392, 25, 100}, {784, 350, 100}, {739.98, 250, 100}, {698.46, 25, 100}, {659.26, 25, 100}, {622.26, 25, 100}, {659.26, 50, 400}, {415.3, 25, 200}, {554.36, 350, 100}, {523.25, 250, 100}, {493.88, 25, 100}, {466.16, 25, 100}, {440, 25, 100}, {466.16, 50, 400}, {311.13, 25, 200}, {369.99, 350, 100}, {311.13, 250, 100}, {392, 25, 100}, {466.16, 350, 100}, {392, 250, 100}, {466.16, 25, 100}, {587.32, 700, 100}, {784, 350, 100}, {392, 250, 100}, {392, 25, 100}, {784, 350, 100}, {739.98, 250, 100}, {698.46, 25, 100}, {659.26, 25, 100}, {622.26, 25, 100}, {659.26, 50, 400}, {415.3, 25, 200}, {554.36, 350, 100}, {523.25, 250, 100}, {493.88, 25, 100}, {466.16, 25, 100}, {440, 25, 100}, {466.16, 50, 400}, {311.13, 25, 200}, {392, 350, 100}, {311.13, 250, 100}, {466.16, 25, 100}, {392.00, 300, 150}, {311.13, 250, 100}, {466.16, 25, 100}, {392, 700} },
				true);
			state = 5;
			break;
		case 5:
			//Play victory sound
			turnRight(0, -1);
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
