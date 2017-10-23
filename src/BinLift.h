/*
 * BinLift.h
 *  Created on: Apr 5, 2015
 *  Author: John Muse
 * 	Description: This class will handle anything to do with controlling the recycle bin lifting mechanism.
 */

#ifndef SRC_BINLIFT_H_
#define SRC_BINLIFT_H_

#include "WPILib.h"

class BinLift {
public:
	DigitalInput *topSwitch; // the top switch on top of the bin lifting arm
	AnalogInput *flexreader; // the flex sensor
	Jaguar *recycleLift; // The object for the CIM motor that controls the lifting mechanism
	float speed; // speed of the lifting mechanism
	bool fixStateUp, fixToggleUp, fixStateDown, fixToggleDown, stillOp; // used for fixFlex() method

public:
	BinLift(); // constructor
	virtual ~BinLift(); // destructor
	long map(long x, long in_min, long in_max, long out_min, long out_max); // Formula used to refine the flex data
	long refineFlexData(long raw); // Uses the "map()" method to refine the flex data
	bool dangerFlex(long flex); // This will return true if the flex is too great
	void fixFlex(); // This method takes the refined flex sensor data and attempts to fix the problem
	long getFlex(); // Gets the refined flex data (mostly used for other methods)
	void liftUp(bool button); // lifts the claw
	void liftDown(bool button); // Lowers the claw
	void liftHalt(); // This method stops any motor movement
};

#endif /* SRC_BINLIFT_H_ */
