// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.
#include "RobotMap.h"
#include "LiveWindow/LiveWindow.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION
SpeedController* RobotMap::driveSubsystemSpeedController1 = NULL;
SpeedController* RobotMap::driveSubsystemSpeedController2 = NULL;
SpeedController* RobotMap::driveSubsystemSpeedController3 = NULL;
SpeedController* RobotMap::driveSubsystemSpeedController4 = NULL;
RobotDrive* RobotMap::driveSubsystemRobotDriveMecanum = NULL;
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=ALLOCATION
void RobotMap::init() {
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
	LiveWindow* lw = LiveWindow::GetInstance();
	driveSubsystemSpeedController1 = new Victor(1, 1);
	lw->AddActuator("DriveSubsystem", "Speed Controller 1", (Victor*) driveSubsystemSpeedController1);
	
	driveSubsystemSpeedController2 = new Victor(1, 2);
	lw->AddActuator("DriveSubsystem", "Speed Controller 2", (Victor*) driveSubsystemSpeedController2);
	
	driveSubsystemSpeedController3 = new Victor(1, 3);
	lw->AddActuator("DriveSubsystem", "Speed Controller 3", (Victor*) driveSubsystemSpeedController3);
	
	driveSubsystemSpeedController4 = new Victor(1, 4);
	lw->AddActuator("DriveSubsystem", "Speed Controller 4", (Victor*) driveSubsystemSpeedController4);
	
	driveSubsystemRobotDriveMecanum = new RobotDrive(driveSubsystemSpeedController1, driveSubsystemSpeedController2,
              driveSubsystemSpeedController3, driveSubsystemSpeedController4);
	
// RobotBuilder generated	driveSubsystemRobotDriveMecanum->SetSafetyEnabled(true);
	driveSubsystemRobotDriveMecanum->SetSafetyEnabled(false);
        driveSubsystemRobotDriveMecanum->SetExpiration(0.1);
        driveSubsystemRobotDriveMecanum->SetSensitivity(0.5);
        driveSubsystemRobotDriveMecanum->SetMaxOutput(1.0);
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
}