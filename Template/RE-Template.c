/*
Vassar Cognitive Science - Robot Ethology (Template version)

PROGRAM NAME:  ### YOUR PROGRAM NAME HERE ###

TEAM NAME:	### TEAM NAME HERE ###
STUDENTS:	### YOUR NAMES HERE ###

COURSE:			211 - Perception & Action
INSTRUCTORS:	Ken Livingston, Joshua de Leeuw
TERM:			Fall 2022

PROGRAM DESCRIPTION:
### DESCRIBE WHAT YOUR PROGRAM DOES HERE
(What is the overall novel behavior?  Can you condense the fundamental idea of the code into a brief overview here for your readers?
i.e. This program operates a KIPR-Wombat-based robot (equipped with analog photo sensors, IR sensors, bumpers)...)
###
*/

// *** Import Libraries *** //

#include <kipr/wombat.h> // KIPR Wombat native library
#include <stdlib.h>		 // library for general purpose functions
#include <stdbool.h>	 // library for boolean support

// *** Define PIN Address *** //

#define RIGHT_IR_PIN 0
#define LEFT_IR_PIN 1
#define RIGHT_PHOTO_PIN 2
#define LEFT_PHOTO_PIN 3 // analog sensors (IRs, photos)

#define FRONT_BUMP_RIGHT_PIN 0
#define FRONT_BUMP_LEFT_PIN 1
#define BACK_BUMP_RIGHT_PIN 2
#define BACK_BUMP_LEFT_PIN 3 // digital sensors (bumpers)

#define RIGHT_MOTOR_PIN 0
#define LEFT_MOTOR_PIN 1 // servos

/*
### ADD ANY OTHER SENSOR OR ACTUATOR NAMES AND THEIR PIN ADDRESSES UNDER THIS COMMENT BLOCK ###
*/

// *** Function Declarations *** //

/*
### DECLARE ALL OF YOUR FUNCTIONS UNDER THIS COMMENT BLOCK, IN THEIR RESPECTIVE SECTIONS.
What do they return (the variable type of the left) and what are their inputs (the values in the parentheses)?
###
*/

// PERCEPTION FUNCTIONS
void read_sensors();			// read all sensor values and save to global variables
bool example_check_something(); // example perception function with no input that returns a boolean

// ACTION FUNCTIONS
int example_do_something(float foo); // example action function with a float input that returns an integer

// MOTOR CONTROL
void drive(float left, float right, float delay_seconds); // drive with the specified left and right motor speeds for a number of seconds
void example_drive(float straight);						  // example motor control function with a float input that executes but not returning

// HELPER FUNCTIONS
bool timer_elapsed(); // return true if our timer has elapsed
float map(float value, float start_range_low, float start_range_high, float target_range_low, float target_range_high);
// remap a value from a source range to a new range

// *** Variable Definitions *** //

/* ### DEFINE GLOBAL VARIABLES UNDER THIS COMMENT BLOCK ### */

int example_integer_variable = 100; // this example variable stores an integer value that is declared globally

// global variables to store all current sensor values accessible to all functions and updated by the "read_sensors" function
int right_photo_value, left_photo_value, right_ir_value, left_ir_value, front_bump_right_value, front_bump_left_value, back_bump_right_value, back_bump_left_value;

// timer
int timer_duration = 500;	  // the time in milliseconds to wait between calling action commands, changed by each drive command called by actions
unsigned long start_time = 0; // store the system time each time we start an action so we can see if our time has elapsed without a blocking delay

// *** Function Definitions *** //

/* ### DEFINE FUNCTIONS UNDER THIS COMMENT BLOCK, IN THEIR RESPECTIVE SECTION THEN CALL THEM IN MAIN ### */

//==================================//
//===============MAIN===============//
//==================================//

int main()
{
	enable_servo(LEFT_MOTOR_PIN); // initialize both motors
	enable_servo(RIGHT_MOTOR_PIN);
	drive(0.0, 0.0, 1.0); // set our drive speed to zero so we aren't moving at the start

	while (true)
	{ // infinite loop (true is always true!)

		read_sensors(); // read all sensor values and set to global variables

		if (timer_elapsed())
		{ // any time a drive message is called, the timer is updated; this should always return true until it is called again
			int value = example_do_something(3.4);
			// do something random and useless (REPLACE WITH USEFUL AND INTERESTING FUNCTIONS)
		}
	}
	return 0;
}

//========================================//
//===============PERCEPTION===============//
//========================================//

/**
This function is called once per loop, and reads all the sensor values and sets the appropriate global variables defined above.
**/

void read_sensors()
{
	left_photo_value = analog_et(LEFT_PHOTO_PIN);	// read the photo sensor at the LEFT_PHOTO_PIN; *** NOTE: greater value means less light ***
	right_photo_value = analog_et(RIGHT_PHOTO_PIN); // read the photo sensor at the RIGHT_PHOTO_PIN; *** NOTE: greater value means less light ***
	left_ir_value = analog_et(LEFT_IR_PIN);			// read the sensor for the left IR at LEFT_IR_PIN
	right_ir_value = analog_et(RIGHT_IR_PIN);		// read the sensor for the right IR at RIGHT_IR_PIN

	front_bump_right_value = digital(FRONT_BUMP_RIGHT_PIN); // read the bumper at FRONT_BUMP_RIGHT_PIN
	front_bump_left_value = digital(FRONT_BUMP_LEFT_PIN);	// read the bumper at FRONT_BUMP_LEFT_PIN
	back_bump_right_value = digital(BACK_BUMP_RIGHT_PIN);	// read the bumper at BACK_BUMP_RIGHT_PIN
	back_bump_left_value = digital(BACK_BUMP_LEFT_PIN);		// read the bumper at BACK_BUMP_LEFT_PIN'
}

//====================================//
//===============ACTION===============//
//====================================//

/*
The drive function takes the left and right motor speeds and a delay time amount as inputs and triggers the wheels to drive.  

Inputs:
	[left] The left wheel speed, between -1.0 and 1.0
	[right] The right wheel speed, between -1.0 and 1.0
	[delay_seconds] The delay time in seconds (0 to MAX_FLOAT)
*/

void drive(float left, float right, float delay_seconds)
{
	// 850 is full motor speed clockwise, 1250 is full motor speed counterclockwise
	// Servo is stopped from ~1044 to 1055

	float left_speed = map(left, -1.0, 1.0, 850.0, 1250.0); // call the map function to map our speed (set between -1 and 1) to the appropriate range of motor values
	float right_speed = map(right, -1.0, 1.0, 1250.0, 850.0);

	timer_duration = (int)(delay_seconds * 1000.0); // multiply our desired time in seconds by 1000 to get milliseconds and update this global variable
	start_time = systime();							// update our start time to reflect the time we start driving (in ms)

	set_servo_position(LEFT_MOTOR_PIN, left_speed); 
	set_servo_position(RIGHT_MOTOR_PIN, right_speed); // set the servos to run at the mapped speed
}

/*
This is an example of a function, it checks if the input value is greater than three.  If it is, it returns the integer 3 and drives straight.

Inputs:
	[foo] A floating point number

Returns 3 if foo is greater than 3, otherwise it returns zero and does nothing
*/

int example_do_something(float foo)
{
	if (foo > 3.0)
	{
		drive(1.0, 1.0, 0.25);
		return 3;
	}
	else
		return 0;
}

//=====================================//
//===============HELPERS===============//
//=====================================//

/*
Checks if the timer has elapsed.

Returns true if the global variable start time + the timer duration is greater than the system clock time (in milliseconds)
*/

bool timer_elapsed()
{
	return (systime() > (start_time + timer_duration)); // return true if the current time is greater than our start time plus timer duration
}


/*
Map a value from an input range to a new value in a new range.

Inputs:
	[value] the starting number to remap
	[start_range_low] the low value of the initial bounds
	[start_range_high] the high value of the initial bounds
	[target_range_low] the low value of the target bounds
	[target_range_high] the high value of the target bounds

Returns the remapped value as a float
*/

float map(float value, float start_range_low, float start_range_high, float target_range_low, float target_range_high)
{
	return target_range_low + ((value - start_range_low) / (start_range_high - start_range_low)) * (target_range_high - target_range_low);
}
