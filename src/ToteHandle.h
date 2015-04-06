/*
 * ToteHandle.h
 *
 *  Created on: Apr 3, 2015
 *      Author: johnmuse
 */

#ifndef SRC_TOTEHANDLE_H_
#define SRC_TOTEHANDLE_H_

#include "WPILib.h"
#include <cmath>

class ToteHandle {
public:
	DoubleSolenoid *toteLifter1;
	DoubleSolenoid *toteLifter2;
	DigitalInput *toteSensor;
	Jaguar *conveyor1;
	Jaguar *conveyor2;
	bool liftState;
	double stdSpeed, pilotSpeed;

public:
	ToteHandle();
	virtual ~ToteHandle();
	void liftTotes();
	void setToteState(bool x);
	bool getToteState();
	bool getSensorRead();
	void conveyorAuto();
	void pilotTrigger();
	void manualConveyor(bool s1, bool s2);
	void setSpeed(float s);
};
#endif /* SRC_TOTEHANDLE_H_ */
