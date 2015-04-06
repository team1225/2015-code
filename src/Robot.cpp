/**
 * Author: Brian Kowald, John Muse, Henry Fiantaca
 * Team: FRC Team 1225 | The Gorillas
 * Note To Programmer: This program is a little different than previous years because rather than having stored in one class,
 * 					   multiple functions of the program of the robot are split up into many classes. Hopefully this will make everything
 * 					   easier to read and modify. This class will act as the main class and control the robot driving for the most part
 * 					   and compressor and the controls.
 */

#include "WPILib.h"
#include "ToteHandle.h"
#include "BinLift.h"

class Robot: public SampleRobot {
	// Channels for the wheels
	const int frontLeftMotor = 2;
	const int rearLeftMotor = 1;
	const int frontRightMotor = 3;
	const int rearRightMotor = 0;

	RobotDrive *myDrive; // Robot drive system
	Joystick leadPilot, copilot;
	Compressor *compressor;
	ToteHandle *tote;
	BinLift *bin;

public:
	Robot() :
			leadPilot(0), copilot(1)
	{
		tote = new ToteHandle();
		bin = new BinLift();
		compressor = new Compressor();
		myDrive = new RobotDrive(frontLeftMotor, rearLeftMotor, frontRightMotor,
				rearRightMotor);
		myDrive->SetExpiration(0.1);
		myDrive->SetInvertedMotor(RobotDrive::kRearRightMotor, true);
		myDrive->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
	}

	~Robot() {
		delete myDrive;
		delete compressor;
		delete tote;
		delete bin;
	}

	void Autonomous() {
		compressor->Start();
	}

	void OperatorControl() {
		compressor->Start();

		while (IsOperatorControl() && IsEnabled()) {
			// This part deals with tote loading and off loading
			tote->setSpeed(copilot.GetX());
			SmartDashboard::PutNumber("Conveyor Speed: ", tote->stdSpeed);
			if (copilot.GetRawButton(1) || copilot.GetRawButton(2))
				tote->manualConveyor(copilot.GetRawButton(1), copilot.GetRawButton(2));
			else if (copilot.GetRawButton(3))
				tote->conveyorAuto();
			else if (leadPilot.GetRawButton(6))
				tote->pilotTrigger();

			tote->setToteState(copilot.GetRawButton(4));
			tote->liftTotes();
			// End of tote unloading and offloading

			// This deals with the recycle bin lifter
			SmartDashboard::PutBoolean("Recycle Bin Lifter Health: ", bin->dangerFlex(bin->getFlex()));

			if (copilot.GetRawButton(4))
			{
				if (bin->dangerFlex(bin->getFlex()))
				{
					bin->fixFlex();
					if (!bin->stillOp) // gives driver manual control if still in danger mode and the time has been spent up of how long the robot could try to fix the lifter itself
					{
						if (leadPilot.GetRawButton(5) && !leadPilot.GetRawButton(7))
							bin->liftUp(leadPilot.GetRawButton(2));
						else if (leadPilot.GetRawButton(7) && !leadPilot.GetRawButton(5))
							bin->liftDown(leadPilot.GetRawButton(2));
						else
							bin->liftHalt();
					}
				}
				else
				{
					if (leadPilot.GetRawButton(5) && !leadPilot.GetRawButton(7))
						bin->liftUp(leadPilot.GetRawButton(2));
					else if (leadPilot.GetRawButton(7) && !leadPilot.GetRawButton(5))
						bin->liftDown(leadPilot.GetRawButton(2));
					else
						bin->liftHalt();
				}
			}
			// End of code for the recycle bin lifter

			// Dedicated to driving system
			float x = leadPilot.GetX(), y = leadPilot.GetY(), z =
					leadPilot.GetZ(), g = 0.0;
			if (leadPilot.GetRawButton(11)) // for turbo speed
				myDrive->MecanumDrive_Cartesian(-y, x, z, g);
			else {
				x = x * 0.5;
				y = y * -0.5;
				z = z * 0.5;
				myDrive->MecanumDrive_Cartesian(y, x, z, g);
			}
			// End of driving system code
			Wait(0.005);
		}
	}

	void Test() {

	}
};

START_ROBOT_CLASS(Robot);
