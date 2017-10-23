/*
 * ToteHandle.cpp
 *
 *  Created on: Apr 3, 2015
 *      Author: johnmuse
 */

#include <ToteHandle.h>

ToteHandle::ToteHandle() {
	toteSensor = new DigitalInput(0);
	toteLifter1 = new DoubleSolenoid(0, 1);
	toteLifter2 = new DoubleSolenoid(2, 3);
	conveyor1 = new Jaguar(5);
	conveyor2 = new Jaguar(6);
	liftState = false;
}

ToteHandle::~ToteHandle() {
	delete toteSensor;
	delete toteLifter1;
	delete toteLifter2;
	delete conveyor1;
	delete conveyor2;
}

/*
 * This method will set the totestate so that "liftTotes()" method knows to lift or put totes down
 */
void ToteHandle::setToteState(bool x) {
	ToteHandle::liftState = x;
}

bool ToteHandle::getToteState() {
	return ToteHandle::liftState;
}

bool ToteHandle::getSensorRead() {
	return ToteHandle::toteSensor->Get();
}

void ToteHandle::liftTotes() {
	if (getToteState()) {
		ToteHandle::toteLifter1->Set(DoubleSolenoid::kForward);
		ToteHandle::toteLifter2->Set(DoubleSolenoid::kForward);
	} else {
		ToteHandle::toteLifter1->Set(DoubleSolenoid::kReverse);
		ToteHandle::toteLifter2->Set(DoubleSolenoid::kReverse);
	}
}

/*
 * This method is used for when the conveyor is set to auto mode
 */
void ToteHandle::conveyorAuto() {
	bool read = getSensorRead();
	if (!read) {
		ToteHandle::conveyor1->Set(0.0);
		ToteHandle::conveyor2->Set(0.0);
	} else {
		ToteHandle::conveyor1->Set(-(ToteHandle::stdSpeed));
		ToteHandle::conveyor2->Set(-(ToteHandle::stdSpeed));
	}
}

/*
 * Method used for when the lead pilot wants to shoot out the totes
 */
void ToteHandle::pilotTrigger() {
	ToteHandle::conveyor1->Set(-(ToteHandle::pilotSpeed));
	ToteHandle::conveyor2->Set(-(ToteHandle::pilotSpeed));
}

/*
 * Allows the copilot to have manual control of the conveyor belt without
 * anything interfering with it
 */
void ToteHandle::manualConveyor(bool s1, bool s2) {
	if (s1 && !s2) {
		ToteHandle::conveyor1->Set(-(ToteHandle::stdSpeed));
		ToteHandle::conveyor2->Set(-(ToteHandle::stdSpeed));
	} else if (!s1 && s2) {
		ToteHandle::conveyor1->Set((ToteHandle::stdSpeed));
		ToteHandle::conveyor2->Set((ToteHandle::stdSpeed));
	} else {
		ToteHandle::conveyor1->Set(0.0);
		ToteHandle::conveyor2->Set(0.0);
	}
}

/*
 * Sets the speed for the conveyor belt
 */
void ToteHandle::setSpeed(float s) {
	s = std::abs(s);
	ToteHandle::stdSpeed = s;
	ToteHandle::pilotSpeed = ToteHandle::stdSpeed + 0.25;
	if (s >= 0.75) {
		ToteHandle::pilotSpeed = 1.0;
		ToteHandle::stdSpeed = s;
	} else {
		ToteHandle::stdSpeed = s;
		ToteHandle::pilotSpeed = ToteHandle::stdSpeed + 0.25;
	}
}

