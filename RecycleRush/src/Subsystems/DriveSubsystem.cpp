// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "DriveSubsystem.h"

#include <math.h>

#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "../Commands/DriveCommand.h"
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

#include "../UtilFun.h"
#include "../Logger.h"
#include "../LoggingComponentDefs.h"
#include "../ConfigKeys.h"
#include "../Config/ConfigInstanceMgr.h"

static double CalcPercentageOfMaxSpeed(double absoluteDistanceFromGoal,
									   double absoluteGoalDistance,
									   double absoluteSlowDownThresholdDistance);

static const float TWIST_TOLERANCE_MIN_DEFAULT = -0.33;
static const float TWIST_TOLERANCE_MAX_DEFAULT = 0.33;
static const float TWIST_SCALE_FACTOR_DEFAULT = 0.65;

static const float Y_TOLERANCE_MIN_DEFAULT = -0.25;
static const float Y_TOLERANCE_MAX_DEFAULT = 0.25;

static const float X_TOLERANCE_MIN_DEFAULT = -0.25;
static const float X_TOLERANCE_MAX_DEFAULT = 0.35;

static bool Drive_Params_Inited = false;

static float Twist_Tolerance_Min = -0.33;
static float Twist_Tolerance_Max = 0.33;
static float Twist_Scale_Factor = 0.65;

static float Y_Tolerance_Min = -0.25;
static float Y_Tolerance_Max = 0.25;

static float X_Tolerance_Min = -0.25;
static float X_Tolerance_Max = 0.35;

// Diameter of the robot's wheels in inches.
static const int WHEEL_DIAMETER_IN_INCHES_DEFAULT = 8;
static int WheelDiameterInInches = WHEEL_DIAMETER_IN_INCHES_DEFAULT;

// Circumference of the wheel;
static double WheelCircumference = M_PI * WheelDiameterInInches;

// Number of counts per shaft rotation.
static const int COUNTS_PER_SHAFT_ROTATION_DEFAULT = 1000;
static int CountsPerShaftRotation = COUNTS_PER_SHAFT_ROTATION_DEFAULT;

// Max speed to use when doing an Automatic Rotation.
static const float AUTO_ROTATE_MAX_SPEED_DEFAULT = 1.0;
static float AutoRotateMaxSpeed = AUTO_ROTATE_MAX_SPEED_DEFAULT;

// This constant is used as the threshold to begin slowing down the robot
// when it is auto-rotating and there is 45 degrees or less to go in the
// rotation.  This should be made a configuration value.
static const float AUTO_ROTATE_SLOW_DOWN_THRESHOLD_DEFAULT = 45;
static float AutoRotateSlowDownThreshold = AUTO_ROTATE_SLOW_DOWN_THRESHOLD_DEFAULT;

// Precision to use for rotation angles.
static const float ANGLE_DECIMAL_PLACES_PRECISION_DEFAULT = 100.00;
static float AngleDecimalPlacesPrecision = ANGLE_DECIMAL_PLACES_PRECISION_DEFAULT;

// Epsilon to use to figure out if close enough to the rotation target.
static const float AUTO_ROTATE_TARGET_ANGLE_EPSILON_DEFAULT = 2.0;
static float AutoRotate_TargetAngle_Epsilon = AUTO_ROTATE_TARGET_ANGLE_EPSILON_DEFAULT;

// Maximum speed to use when performing and AutoDrive in any direction.
static const float AUTO_DRIVE_HEADING_MAX_SPEED_DEFAULT = 1.0;
static float AutoDriveHeadingMaxSpeed = AUTO_DRIVE_HEADING_MAX_SPEED_DEFAULT;

// This constant is used as the threshold to begin slowing down the robot
// when it is auto-driving and there is 24 inches or less to go in the
// auto-drive.  This should be made a configuration value.
static const float AUTO_DRIVE_SLOW_DOWN_THRESHOLD_IN_INCHES_DEFAULT = 24.0;
static float AutoDriveSlowDownThresholdInInches = AUTO_DRIVE_SLOW_DOWN_THRESHOLD_IN_INCHES_DEFAULT;

static float AutoDriveSlowDownThresholdInShaftRotations = -1.0;

// Epsilon to use to figure out if close enough to the auto-drive target.
static const float AUTO_DRIVE_TARGET_DISTANCE_EPSILON_IN_INCHES_DEFAULT = 2.0;
static float AutoDriveTargetDistanceEpsilonInInches = AUTO_DRIVE_TARGET_DISTANCE_EPSILON_IN_INCHES_DEFAULT;

static float AutoDriveTargetDistanceEpsilonInCounts = -1.0;

static const bool DRIVE_SAFETY_ENABLED_DEFAULT = false;
static const float DRIVE_SAFETY_TIME_OUT_DEFAULT = 1.0;

static const std::string ForwardHeading = "Forward";
static const std::string BackHeading = "Back";
static const std::string LeftHeading = "Left";
static const std::string RightHeading = "Right";
static const std::string IllegalHeading = "Illegal Heading";

static const std::string LeftDirection = "RotateLeft";
static const std::string RightDirection = "RotateRight";
static const std::string IllegalDirection = "Illegal Direction";

static const std::string AutoModeOff = "Off";
static const std::string AutoModeDriving = "Driving";
static const std::string AutoModeRotating = "Rotating";
static const std::string IllegalAutoMode = "Illegal Auto Mode";


static const bool DriveHeadingValid(DriveHeading heading) {
	switch (heading) {
	case DriveForward:
	case DriveBack:
	case DriveLeft:
	case DriveRight:
		return true;

	default:
		return false;
	}
}

static const bool RotateDirectionValid(RotateDirection direction) {
	switch (direction) {

	case RotateLeft:
	case RotateRight:
		return true;

	default:
		return false;
	}
}

static const double LateralDistanceMultipler = 2.0;

static double DistanceToShaftRotationCount(int distanceToTravelInInches, DriveHeading heading)
{
	if (distanceToTravelInInches <= 0.0)
	{
		return 0.0;
	}

	double shaftRotationCountTarget = distanceToTravelInInches / WheelCircumference;
	shaftRotationCountTarget *= CountsPerShaftRotation;

	if ((heading == DriveLeft) || (heading == DriveRight))
	{
		shaftRotationCountTarget *= LateralDistanceMultipler;
	}

	return shaftRotationCountTarget;
}

static void InitDriveSubsystemConfiguration()
{
    ConfigMgr *configMgr = ConfigInstanceMgr::getInstance();

    if (!Drive_Params_Inited) {
    	X_Tolerance_Min = configMgr->getDoubleVal(ConfigKeys::Drive_XToleranceMin,X_TOLERANCE_MIN_DEFAULT);

    	X_Tolerance_Max = configMgr->getDoubleVal(ConfigKeys::Drive_XToleranceMax,X_TOLERANCE_MAX_DEFAULT);

    	Y_Tolerance_Min = configMgr->getDoubleVal(ConfigKeys::Drive_YToleranceMin,Y_TOLERANCE_MIN_DEFAULT);

    	Y_Tolerance_Max = configMgr->getDoubleVal(ConfigKeys::Drive_YToleranceMax,Y_TOLERANCE_MAX_DEFAULT);

    	Twist_Tolerance_Min = configMgr->getDoubleVal(ConfigKeys::Drive_TwistToleranceMin,TWIST_TOLERANCE_MIN_DEFAULT);

    	Twist_Tolerance_Max = configMgr->getDoubleVal(ConfigKeys::Drive_TwistToleranceMax,TWIST_TOLERANCE_MAX_DEFAULT);

    	Twist_Scale_Factor = configMgr->getDoubleVal(ConfigKeys::Drive_TwistScaleFactor,TWIST_SCALE_FACTOR_DEFAULT);

    	WheelDiameterInInches = configMgr->getDoubleVal(ConfigKeys::Drive_WheelDiameterInInchesKey, WHEEL_DIAMETER_IN_INCHES_DEFAULT);
    	WheelCircumference = M_PI * WheelDiameterInInches;

    	CountsPerShaftRotation = configMgr->getDoubleVal(ConfigKeys::Drive_CountsPerShaftRotationKey, COUNTS_PER_SHAFT_ROTATION_DEFAULT);

		AutoRotateMaxSpeed = static_cast<float>(configMgr->getDoubleVal(ConfigKeys::Drive_AutoRotateMaxSpeedKey, AUTO_ROTATE_MAX_SPEED_DEFAULT));

		AutoRotateSlowDownThreshold = static_cast<float>(configMgr->getDoubleVal(ConfigKeys::Drive_AutoRotateSlowDownThresholdKey, AUTO_ROTATE_SLOW_DOWN_THRESHOLD_DEFAULT));

		AngleDecimalPlacesPrecision = static_cast<float>(configMgr->getDoubleVal(ConfigKeys::Drive_AngleDecimalPlacesPrecisionKey, ANGLE_DECIMAL_PLACES_PRECISION_DEFAULT));

		AutoRotate_TargetAngle_Epsilon = static_cast<float>(configMgr->getDoubleVal(ConfigKeys::Drive_AutoRotateTargetAngleEpsilonKey, AUTO_ROTATE_TARGET_ANGLE_EPSILON_DEFAULT));

		AutoDriveHeadingMaxSpeed = static_cast<float>(configMgr->getDoubleVal(ConfigKeys::Drive_AutoDriveHeadingMaxSpeedKey, AUTO_DRIVE_HEADING_MAX_SPEED_DEFAULT));

		AutoDriveSlowDownThresholdInInches = static_cast<float>(configMgr->getDoubleVal(ConfigKeys::Drive_AutoDriveSlowDownThresholdInInchesKey, AUTO_DRIVE_SLOW_DOWN_THRESHOLD_IN_INCHES_DEFAULT));

		AutoDriveSlowDownThresholdInShaftRotations = DistanceToShaftRotationCount(AutoDriveSlowDownThresholdInInches, DriveForward);

		AutoDriveTargetDistanceEpsilonInInches = static_cast<float>(configMgr->getDoubleVal(ConfigKeys::Drive_AutoDriveTargetDistanceEpsilonInInchesKey, AUTO_DRIVE_TARGET_DISTANCE_EPSILON_IN_INCHES_DEFAULT));

		AutoDriveTargetDistanceEpsilonInCounts = DistanceToShaftRotationCount(AutoDriveTargetDistanceEpsilonInInches, DriveForward);


		Logger* logger = Logger::GetInstance();

		logger->Log(DriveSubsystemLogId,Logger::kINFO, "DriveSubsystem: X Tolerance min = %f, max = %f\n", X_Tolerance_Min, X_Tolerance_Max);
		logger->Log(DriveSubsystemLogId,Logger::kINFO, "DriveSubsystem: Y Tolerance min = %f, max = %f\n", Y_Tolerance_Min, Y_Tolerance_Max);
		logger->Log(DriveSubsystemLogId,Logger::kINFO, "DriveSubsystem: Twist Tolerance min = %f, max = %f, Scale factor = %f \n", Twist_Tolerance_Min, Twist_Tolerance_Max, Twist_Scale_Factor);

		logger->Log(DriveSubsystemLogId,Logger::kINFO, "DriveSubsystem: Wheel Diameter = %d inches\n", WheelDiameterInInches);
		logger->Log(DriveSubsystemLogId,Logger::kINFO, "DriveSubsystem: Counts Per Shaft Rotation = %d inches\n", CountsPerShaftRotation);

		logger->Log(DriveSubsystemLogId,Logger::kINFO, "DriveSubsystem: Wheel Diameter = %d inches, Circumference = %g\n",
      							   WheelDiameterInInches, WheelCircumference);

		logger->Log(DriveSubsystemLogId, Logger::kINFO, "DriveSubsystem: AutoRotateMaxSpeed = %f\n", AutoRotateMaxSpeed);
		logger->Log(DriveSubsystemLogId, Logger::kINFO, "DriveSubsystem: AutoRotateAnglePrecision = %f\n", AngleDecimalPlacesPrecision);
		logger->Log(DriveSubsystemLogId, Logger::kINFO, "DriveSubsystem: AutoRotateTargetAngleEpsilon = %f\n", AutoRotate_TargetAngle_Epsilon);

		logger->Log(DriveSubsystemLogId, Logger::kINFO, "DriveSubsystem: AutoDriveHeadingMaxSpeed = %f\n", AutoDriveHeadingMaxSpeed);
		logger->Log(DriveSubsystemLogId, Logger::kINFO, "DriveSubsystem: AutoDriveSlowDownThresholdInInches = %f\n", AutoDriveSlowDownThresholdInInches);
		logger->Log(DriveSubsystemLogId, Logger::kINFO, "DriveSubsystem: AutoDriveSlowDownThresholdInShaftRotations = %f\n", AutoDriveSlowDownThresholdInShaftRotations);
		logger->Log(DriveSubsystemLogId, Logger::kINFO, "DriveSubsystem: AutoDriveTargetDistanceEpsilonInInches = %f\n", AutoDriveTargetDistanceEpsilonInInches);
		logger->Log(DriveSubsystemLogId, Logger::kINFO, "DriveSubsystem: AutoDriveTargetDistanceEpsilonInCounts = %f\n", AutoDriveTargetDistanceEpsilonInCounts);

      	Drive_Params_Inited = true;
    }
}

const std::string& DriveHeadingToString(DriveHeading heading) {
	switch (heading) {
	case DriveForward:
		return ForwardHeading;

	case DriveBack:
		return BackHeading;

	case DriveLeft:
		return LeftHeading;

	case DriveRight:
		return RightHeading;

	default:
		return IllegalHeading;
	}

}

const std::string& RotateDirectionToString(RotateDirection direction) {
	switch (direction) {

	case RotateLeft:
		return LeftDirection;

	case RotateRight:
		return RightDirection;

	default:
		return IllegalDirection;
	}
}

const std::string& AutoModeToString(AutoMode mode) {
	switch (mode) {
	case Off:
		return AutoModeOff;

	case Driving:
		return AutoModeDriving;

	case Rotating:
		return AutoModeRotating;

	default:
		return IllegalAutoMode;
	}
}

// *********************************************************************************
// V a r i o u s  M o d e s  of the  T a l o n   M o t o r
// This sets the mode of the m_motor. The options are:
// kPercentVbus: basic throttle; no closed-loop.
// kCurrent: Runs the motor with the specified current if possible.
// kSpeed: Runs a PID control loop to keep the motor going at a constant
//   speed using the specified sensor.
// kPosition: Runs a PID control loop to move the motor to a specified move
//   the motor to a specified sensor position.
// kVoltage: Runs the m_motor at a constant voltage, if possible.
// kFollower: The m_motor will run at the same throttle as the specified other talon.
//m_motor.SetControlMode(CANSpeedController::kPosition);
// This command allows you to specify which feedback device to use when doing
// closed-loop control. The options are:
// AnalogPot: Basic analog potentiometer
// QuadEncoder: Quadrature Encoder
// AnalogEncoder: Analog Encoder
// EncRising: Counts the rising edges of the QuadA pin (allows use of a
//   non-quadrature encoder)
// EncFalling: Same as EncRising, but counts on falling edges.
// m_motor.SetFeedbackDevice(CANTalon::AnalogPot);
// This sets the basic P, I , and D values (F, Izone, and rampRate can also
//   be set, but are ignored here).
// These must all be positive floating point numbers (SetSensorDirection will
//   multiply the sensor values by negative one in case your sensor is flipped
//   relative to your motor).
// These values are in units of throttle / sensor_units where throttle ranges
//   from -1023 to +1023 and sensor units are from 0 - 1023 for analog
//   potentiometers, encoder ticks for encoders, and position / 10ms for
//   speeds.
// m_motor.SetPID(1.0, 0.0, 0.0);
// *********************************************************************************

DriveSubsystem::DriveSubsystem() : Subsystem("DriveSubsystem") {
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	cANTalon1 = RobotMap::driveSubsystemCANTalon1;
	cANTalon3 = RobotMap::driveSubsystemCANTalon3;
	cANTalon2 = RobotMap::driveSubsystemCANTalon2;
	cANTalon4 = RobotMap::driveSubsystemCANTalon4;
	robotDrive = RobotMap::driveSubsystemRobotDrive;
	gyro1 = RobotMap::driveSubsystemGyro1;
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

	autoMode = Off;
	autoActionState = NotStarted;

    ConfigMgr *configMgr = ConfigInstanceMgr::getInstance();

    if (!Drive_Params_Inited) {
    	InitDriveSubsystemConfiguration();
    }

	if (gyro1)
	{
	    useGyro = configMgr->getBoolVal(ConfigKeys::Drive_UseGyroKey, DriveSubsystemConfigValueDefaults::Drive_UseGyroDefaultValue);
	}
	else
	{
		useGyro = false;
	}

	cANTalon1->SetControlMode(CANTalon::kPercentVbus);
	cANTalon3->SetControlMode(CANTalon::kPercentVbus);
	cANTalon2->SetControlMode(CANTalon::kPercentVbus);
	cANTalon4->SetControlMode(CANTalon::kPercentVbus);
}
    
void DriveSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
	SetDefaultCommand(new DriveCommand());
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

void DriveSubsystem::MecanumDrive(float x, float y, float twist) {
	float gyroAngle = 0.0;

	// Only attempt to use the gyro if it has been created
	// and the configuration says to use it.
	if (gyro1 && useGyro)
	{
		gyroAngle = gyro1->GetAngle();
	}

	//filter the X Y and twist values to 0 if they are in a min max tolerence range
	// otherwise the original value is used

	x = ZeroIfInRangeInclusive(x, X_Tolerance_Min,X_Tolerance_Max);
	y = ZeroIfInRangeInclusive(y,Y_Tolerance_Min,Y_Tolerance_Max);
	twist = ZeroIfInRangeInclusive(twist,Twist_Tolerance_Min,Twist_Tolerance_Max);

	//Scale twist so movements are smoother.
	twist *= Twist_Scale_Factor;

	Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kINFO, "DriveSubsystem:Filtered(x,y,twist,angle)=(%f, %f, %f, %f)\n", x, y, twist, gyroAngle);

	robotDrive->MecanumDrive_Cartesian(x, y, twist, gyroAngle);

	//robotDriveMecanum->ArcadeDrive(y,twist);
}

void DriveSubsystem::EnableDriveSubsystem() {
	 ConfigMgr *configMgr = ConfigInstanceMgr::getInstance();

	 float expiration = 0.0;
	 bool enable = configMgr->getBoolVal(ConfigKeys::Drive_EnableSafety, DRIVE_SAFETY_ENABLED_DEFAULT);

	 if (enable)
	 {
		 expiration = configMgr->getDoubleVal(ConfigKeys::Drive_SafetyTimeOut, DRIVE_SAFETY_TIME_OUT_DEFAULT);
	 }

	Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kINFO, "DriveSubsystem:EnableDriveSubsystem() ->SafetyEnable=%s, timeout=%f\n",
			enable ? "TRUE" : "FALSE",
			expiration);

	SetSafetyMode(cANTalon1, enable, expiration);
	SetSafetyMode(cANTalon2, enable, expiration);
	SetSafetyMode(cANTalon3, enable, expiration);
	SetSafetyMode(cANTalon4, enable, expiration);
	robotDrive->SetSafetyEnabled(enable);

	if (enable)
	{
		robotDrive->SetSafetyEnabled(expiration);
	}

	cANTalon1->EnableControl();
	cANTalon2->EnableControl();
	cANTalon3->EnableControl();
	cANTalon4->EnableControl();


	cANTalon1->Set(0);
	cANTalon2->Set(0);
	cANTalon3->Set(0);
	cANTalon4->Set(0);
}

void DriveSubsystem::SetSafetyMode(CANTalon* motor, bool enabled, float timeout) {
	motor->SetSafetyEnabled(enabled);
	motor->SetExpiration(timeout);
}

bool DriveSubsystem::AutoDriveSetup(DriveHeading heading, float distance)
{
	if (AutoDriveTargetDistanceEpsilonInCounts < 0.0)
	{
		AutoDriveTargetDistanceEpsilonInCounts = DistanceToShaftRotationCount(AutoDriveTargetDistanceEpsilonInInches, DriveForward);
	}

	if (autoMode == Off) {
		if (DriveHeadingValid(heading) && (distance > 0.0))
		{
			autoMode = Driving;
			autoActionState = NotStarted;
			autoDrivingParams.autoHeading = heading;
			autoDrivingParams.autoDriveDistanceInInches = distance;
			autoDrivingParams.shaftCountTotalToAtDesiredPosition = DistanceToShaftRotationCount(distance, heading);

			Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kINFO,"Auto Drive Mode ENABLED, heading = %s, distance = %f inches \n", DriveHeadingToString(autoDrivingParams.autoHeading).c_str(), autoDrivingParams.autoDriveDistanceInInches);
			return true;
		}
		else{
			Logger::GetInstance()->Log(DriveSubsystemLogId, Logger::kERROR,"Auto Drive Mode NOT ENABLED, illegal heading = %s or distance = %f inches \n", DriveHeadingToString(autoDrivingParams.autoHeading).c_str(), autoDrivingParams.autoDriveDistanceInInches);
		}
	}
	else {
		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kERROR,"***Auto Drive Mode ALREADY set to %s, IGNNORING***\n", AutoModeToString(autoMode).c_str());
	}

	return false;
}

void DriveSubsystem::AutoDriveExecute()
{
	if (autoMode == Driving) {

		if (autoActionState == Completed)
		{
			return;
		}

		float distanceTraveledSoFar = 0.0;

		if (!autoActionState == NotStarted) {
			// Indicate that the rotation is now in progress.
			autoActionState = InProgress;

			// here is where we would start the PID or the manual encoder code
			cANTalon1->SetPosition(0.0);
			SetMotorSpeeds(0.0);
		}
		else {
			distanceTraveledSoFar = cANTalon1->GetPosition();
		}

		AutoDriveMakeProgress(distanceTraveledSoFar);
	}
	else {
		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kERROR,"***Auto Driving not setup or already Auto Driving (%s), IGNNORING***\n", AutoModeToString(autoMode).c_str());
	}
}

void DriveSubsystem::AutoDriveMakeProgress(float distanceTraveledSoFar) {
	//RotateDirection being a new enum with just left and right
	// or we can find the shortest distance by comparing the
	if (autoMode == Driving)
	{
		if (autoActionState == InProgress)
		{
			float distanceFromGoal = autoDrivingParams.shaftCountTotalToAtDesiredPosition - fabs(distanceTraveledSoFar);

			if (!DistanceWithinEpsilon(distanceFromGoal, AutoDriveTargetDistanceEpsilonInCounts))
			{
				// Calculate the speed that the Robot should start rotating at.
				float percentageOfMaxSpeed = CalcPercentageOfMaxSpeed(fabs(distanceFromGoal),
																	  fabs(autoDrivingParams.shaftCountTotalToAtDesiredPosition),
																	  AutoDriveSlowDownThresholdInShaftRotations);

				float absoluteDriveSpeed = AutoDriveHeadingMaxSpeed * percentageOfMaxSpeed;

				switch (autoDrivingParams.autoHeading)
				{
				case DriveForward:
					SetMotorSpeeds(absoluteDriveSpeed);
					break;

				case DriveBack:
					SetMotorSpeeds(-absoluteDriveSpeed);
					break;

				case DriveLeft:
					SetMotorSpeeds(absoluteDriveSpeed,
								   -absoluteDriveSpeed,
								   -absoluteDriveSpeed,
								   absoluteDriveSpeed);
					break;

				case DriveRight:
					SetMotorSpeeds(-absoluteDriveSpeed,
								   absoluteDriveSpeed,
								   absoluteDriveSpeed,
								   -absoluteDriveSpeed);
					break;
				}
			}
			else {
				// Hit the stopping point, shutdown.
				SetMotorSpeeds(0.0);
				autoActionState = Completed;
			}
		}
	}
	else {
		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kERROR,"***Auto Rotation not setup or already Rotating (%s), IGNNORING***\n", AutoModeToString(autoMode).c_str());
	}
}

bool DriveSubsystem::AutoDriveHasReachedLocation()
{
	if (autoMode == Driving) {
		return (autoActionState == Completed);
	}

	return false;
}


AutoMode DriveSubsystem::AutoModeState() const
{
	return autoMode;
}

void DriveSubsystem::AutoModeDisable() {
	if (autoMode != Off) {
		if (autoMode == Driving)
		{
			// Cancel Driving
			if (autoActionState == InProgress)
			{

			}
		}
		else if (autoMode == Rotating) {
			if (autoActionState == InProgress)
			{
				robotDrive->SetLeftRightMotorOutputs(0, 0);
			}
		}

		autoMode = Off;
		autoActionState = NotStarted;

		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kINFO,"Auto Drive Mode DISABLED\n");
	}
	else {
		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kINFO,"Auto Drive Mode ALREADY disabled,IGNNORING\n");
	}
}

bool DriveSubsystem::AutoRotateSetup(float angle, RotateDirection direction) {
	if (autoMode == Off) {
		if (!RotateDirectionValid(direction)) {
			Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kERROR,"Rotation direction invalid., Auto Rotation NOT enabled.\n");
			return false;
		}

		if (!InRangeInclusive(angle, 0, 180)) {
			Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kERROR,"Rotation angle outside of range, angle = %f, Auto Rotation NOT enabled. \n", angle);
			return false;
		}

		autoMode = Rotating;
		autoActionState = NotStarted;
		autoRotationParams.autoRotateInputAngle = angle;
		autoRotationParams.autoRotateDirection = direction;
		autoRotationParams.autoGyroTargetAngle = angle;

		if (direction == RotateLeft) {
			autoRotationParams.autoGyroTargetAngle += 180;
		}

		//here we need to reset the gyro
		gyro1->Reset();
		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kINFO,"Auto Rotation Mode ENABLED, Direction = %s, InputAngle = %f, GyroTargetAngle = %f.\n",
								   RotateDirectionToString(autoRotationParams.autoRotateDirection).c_str(),
							       autoRotationParams.autoRotateInputAngle,
								   autoRotationParams.autoGyroTargetAngle);

		return true;
	}
	else {
		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kERROR,"***Auto Mode ALREADY set to %s, IGNNORING***\n", AutoModeToString(autoMode).c_str());
	}

	return false;
}


static inline float NormalizeGyroAngle(float rawGyroAngle)
{
	static const int FullGyroSweep = 360 * static_cast<int>(AngleDecimalPlacesPrecision);

	rawGyroAngle *= AngleDecimalPlacesPrecision;
	rawGyroAngle = static_cast<float>((static_cast<int>(rawGyroAngle) % FullGyroSweep)) / AngleDecimalPlacesPrecision;

	return rawGyroAngle;
}

void DriveSubsystem::AutoRotateExecute()
{
	//RotateDirection being a new enum with just left and right
	// or we can find the shortest distance by comparing the
	if (autoMode == Rotating)
	{
		if (autoActionState == Completed)
		{
			return;
		}

		float currGyroAngle;

		if (!autoActionState == NotStarted) {
			// Indicate that the rotation is now in progress.
			autoActionState = InProgress;

			// Reset the gyro.
			gyro1->Reset();

			// First angle is 0 degress.
			currGyroAngle = 0.0;
		}
		else {
			currGyroAngle = NormalizeGyroAngle(gyro1->GetAngle());
		}

		AutoRotateMakeProgress(currGyroAngle);
	}
	else {
		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kERROR,"***Auto Rotation not setup or already Rotating (%s), IGNNORING***\n", AutoModeToString(autoMode).c_str());
	}
}


bool DriveSubsystem::AutoHasFinishedRotating() {

	if (autoMode == Rotating) {
		return (autoActionState == Completed);
	}

	return false;
}

void DriveSubsystem::AutoRotateMakeProgress(float gyroAngle) {
	//RotateDirection being a new enum with just left and right
	// or we can find the shortest distance by comparing the
	if (autoMode == Rotating)
	{
		if (autoActionState == InProgress)
		{
			float distanceFromGoal = autoRotationParams.autoGyroTargetAngle - gyroAngle;

			if (!DistanceWithinEpsilon(distanceFromGoal, AutoRotate_TargetAngle_Epsilon))
			{
				// Calculate the speed that the Robot should start rotating at.
				float percentageOfMaxSpeed = CalcPercentageOfMaxSpeed(fabs(distanceFromGoal),
																	  fabs(autoRotationParams.autoGyroTargetAngle),
																	  AutoRotateSlowDownThreshold);

				float absoluteRotationSpeed = AutoRotateMaxSpeed * percentageOfMaxSpeed;

				// Over shot the target, change the rotation to left.
				if (distanceFromGoal < 0.0)
				{
					autoRotationParams.autoRotateDirection = RotateLeft;
				}
				else
				{
					autoRotationParams.autoRotateDirection = RotateRight;
				}

				switch (autoRotationParams.autoRotateDirection)
				{
					case RotateLeft:
					robotDrive->SetLeftRightMotorOutputs(-absoluteRotationSpeed, absoluteRotationSpeed);
					break;

					case RotateRight:
					robotDrive->SetLeftRightMotorOutputs(absoluteRotationSpeed, -absoluteRotationSpeed); //i would implement a system to slow it down when it gets near the destination angle
					break;

					default:
					robotDrive->SetLeftRightMotorOutputs(0, 0);
					autoActionState = Completed;
					//insert logging statement here
					break;
				}
			}
			else {
				// Hit the stopping point, shutdown.
				robotDrive->SetLeftRightMotorOutputs(0, 0);
				autoActionState = Completed;
			}
		}
	}
	else {
		Logger::GetInstance()->Log(DriveSubsystemLogId,Logger::kERROR,"***Auto Rotation not setup or already Rotating (%s), IGNNORING***\n", AutoModeToString(autoMode).c_str());
	}
}


// Use a sin-wave based function to compute the slow-down trajectory percentage
// as the robot approaches its goal.
static double CalcPercentageOfMaxSpeed(double absoluteDistanceFromGoal,
									   double absoluteGoalDistance,
									   double absoluteSlowDownThresholdDistance)
{
	// If the current distance from the goal is more than the distance that
	// should cause the Robot to start slowing down then run it at 100%
    if (absoluteDistanceFromGoal >= absoluteSlowDownThresholdDistance)
    {
        return 1.0;
    }
    else
    {
        double sinoidal = sin((M_PI / 2.0) * (absoluteDistanceFromGoal / absoluteGoalDistance));

        return sinoidal;
    }
}

void DriveSubsystem::SetMotorSpeeds(float frontLeftMotorSpeed,
									float rearLeftMotorSpeed,
									float frontRightMotorSpeed,
									float rearRightMotorSpeed) {
	cANTalon1->Set(frontLeftMotorSpeed);
	cANTalon2->Set(rearLeftMotorSpeed);
	cANTalon2->Set(frontRightMotorSpeed);
	cANTalon3->Set(rearRightMotorSpeed);
}

void DriveSubsystem::SetMotorSpeeds(float speed)
{
	cANTalon1->Set(speed);
	cANTalon2->Set(speed);
	cANTalon2->Set(speed);
	cANTalon3->Set(speed);
}




