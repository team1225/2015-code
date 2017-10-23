/*
 * BinLift.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: johnmuse
 */

#include <BinLift.h>

BinLift::BinLift() {
	topSwitch = new DigitalInput(1); // switch on top of bin lifting arm
	flexreader = new AnalogInput(0); // the flex sensor
	recycleLift = new Jaguar(4);
	speed = 0.65; // default speed
	fixStateUp = false;
	fixToggleUp = false;
	fixStateDown = false;
	fixToggleDown = false;
	stillOp = false;
}

BinLift::~BinLift() {
	delete topSwitch;
	delete flexreader;
	delete recycleLift;
}

/*
 * Re-maps a number from one range to another.
 * That is, a value of fromLow would get mapped to toLow, a value of
 * fromHigh to toHigh, values in-between
 * to values in-between.
 */
long BinLift::map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*
 * This method take the raw data from the analog channel the flex sensor is on,
 * and refines it into a more "readable" form
 */
long BinLift::refineFlexData(long raw) {
	return map(raw, 300, 500, 0, 100); // Sorta dummy value, maybe.
}

/*
 * Returns true is there is a dangerous amount of flex
 * on the lifting bar.
 */
bool BinLift::dangerFlex(long flex) {
	if (flex > 50) // This is just a dummy value
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * Gets a refined value of the flex amount
 */
long BinLift::getFlex()
{
	long flex = flexreader->GetAverageValue();
	flex = refineFlexData(flex);
	return flex;
}

/*
 * attempts to auto fix any dangerous flex
*/
void BinLift::fixFlex() {
	stillOp = true; // lets the Robot.cpp know if the robot is autonmously still trying to fix the flex
	float direction = recycleLift->Get(); // gets the directions the lifter claw was last traveling
	long lastFlex = getFlex();
	if (direction > 0) {
		if (!fixToggleUp) {
			fixToggleUp = true;
			fixStateUp = !fixStateUp;
		}
	} else {
		if (!fixToggleDown) {
			fixToggleDown = true;
			fixStateDown = !fixStateDown;
		}
	};
	while (dangerFlex(getFlex())) {
		Timer timer;
		timer.Start();

		if (fixStateUp) // if the lifter was going up when the robot detected dangerous flex, it will go the opposite way to try to fix the flex
			liftUp(true);
		else if (fixStateDown)
			liftDown(true);

		if (timer.Get() >= 1.0 && getFlex() > lastFlex) // if the time has been over one second and the flex is increasing, it will hand exit this method
		{
			liftHalt();
			break;
		}
	}
	fixStateUp = false;
	fixToggleUp = false;
	fixStateDown = false;
	fixToggleDown = false;
	stillOp = false;
}

/*
 * Lifts up the bin lifter if the argument returns true. Typically the argumetn will be the digital button from a joystick controller
 */
void BinLift::liftUp(bool button)
{
	if (button)
	{
		speed = 1.0;
		recycleLift->Set(-(speed)); // Just change the negative sign for this and the liftUp method to change the directions
	}
	else
	{
		speed = 0.65;
		recycleLift->Set(-(speed)); // Just change the negative sign for this and the liftUp method to change the directions
	}
}

/*
 * Lowers the bin lifter if the argument returns true. Typically the argumetn will be the digital button from a joystick controller
 */
void BinLift::liftDown(bool button)
{
	if (button)
	{
		speed = 1.0;
		recycleLift->Set((speed)); // Just change the negative sign for this and the liftUp method to change the directions
	}
	else
	{
		speed = 0.65;
		recycleLift->Set((speed)); // Just change the negative sign for this and the liftUp method to change the directions
	}
}

/*
 * This method will halt the motor if this method is called
 */
void BinLift::liftHalt()
{
	recycleLift->Set(0.0);
}
