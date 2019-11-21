#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debounce.h"
#include "MotorControl.h"
#include "DELControl.h"
#include "SensorClass.h"
#include "ToneClass.h"
#include "Receiver.h"
#define CNT_TOP 0xFF
//kp 0.25
#define highKp 0.4
#define midKp 0.3
#define lowKp 0.2
#define Kd 0.01
#define maxSpeed 160
#define baseSpeed 120
#define lowerSpeed 80

const double LARGE_INCREMENT  = 0.75;
const double SMALL_INCREMENT  = 0.77;
const int NB_BOITES = 3;
const uint8_t BASE_SPEED = 120;
enum DIRECTION{LEFT,RIGHT};

enum COLUMNS{COL1 = 1, COL2, COL3};
enum ROWS{ROW1 = 1, ROW2, ROW3};
enum COLUMN_DELAYS{COL1_DELAY = 1850, COL2_DELAY = 1550, COL3_DELAY = 1125};	//cols identified from left to right ( e.g left col is col1 )
enum ROW_DELAYS{ROW1_DELAY = 1900, ROW2_DELAY = 1450, ROW3_DELAY = 1150}; // rows identified from top to bottom ( e.g top row is row1 ) 
const int SETTLING_TIME = 2000;
enum SENSORS{
	SENSOR0 = (1 << 0),
	SENSOR1 = (1 << 1),
	SENSOR2 = (1 << 2),
	SENSOR3 = (1 << 3),
	SENSOR4 = (1 << 4)
};

class LineFollower : public MotorControl{
public: 
	//ratioPwmPD4 and ratioPwmPD5 are values between 0 and 100 inclusively 
	LineFollower(uint8_t currentSegment = 1);

	void nextMeme();

	void setCurrentSegment(uint8_t currentSegment);
	
	//getters
	Pwm getTonePwm() const;


	void calibrate();

	void segment1();
	void segment1Test();
	void segment2();
	void segment3();
	void segment4();
	
	void followLine(int rightMotorSpeed, int leftMotorSpeed, int lastError);
	void followBox();
	void turnRight();
	void turnLeft();
	void alignCenter(uint8_t& sensorState);
	void turn90Deg(uint8_t direction);
	void seg4PlaySound();
	void seekColumn(uint8_t col);
	void seekRow(uint8_t row);
	void seekDot(uint8_t col, uint8_t row);
	void seekSeg2(uint8_t row);
	void brakeGears(); //pun Intended
	void startBoost();
	
private:

	Tone tonePwm_;
	Sensor sensor_;
	float kp_;
	uint8_t currentSegment_;
	uint8_t lastSegment_;
	uint8_t checkpoint_;
	uint8_t baseSpeed_;	
};
#endif

