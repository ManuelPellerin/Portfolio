#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <avr/io.h>
#include "Pwm.h"
#define CNT_TOP 0xFF
#define BASESPEED 120
#define CURVESPEED 100

enum Direction{BACKWARD,FORWARD};
enum Robot{ROBOT38,ROBOT39};

class MotorControl {
public: 
	//ratioPwmPD4 and ratioPwmPD5 are values between 0 and 100 inclusively 
	MotorControl(uint8_t ratioPwmPD4 = 0, uint8_t ratioPwmPD5 = 0, bool forward = true, uint8_t robot = ROBOT39);

	// ajust the " speed " of the wheels
	void adjustSpeed(uint8_t ratioPwmPD4, uint8_t ratioPwmPD5);


	// back and forth
	void setDirection(bool forward);
	void setDirectionPD4(bool forward);
	void setDirectionPD5(bool forward);
	uint8_t getRatioPD4() const;
	uint8_t getRatioPD5() const;

protected:

	Pwm motorPwm_;
	bool forward_;
	float corrRatioPD4_;
	float corrRatioPD5_;

};
#endif

