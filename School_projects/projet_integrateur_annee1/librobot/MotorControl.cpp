#include "MotorControl.h"


MotorControl::MotorControl(uint8_t ratioPwmPD4, uint8_t ratioPwmPD5, bool forward, uint8_t robot)
    :motorPwm_(ratioPwmPD5,ratioPwmPD4,CNT1) {
    pinMode(D, 0xC0, OUTPUT);
    if(!forward){

        digitalWrite(D, 0xC0, ON);
    }  
    else{
        digitalWrite(D, 0xC0, OFF);
    }

    //TODO determines ratios
    if(robot == ROBOT38){

        corrRatioPD4_ =100;   // temp values
        corrRatioPD5_ = 100;

    }

    else
    {

        corrRatioPD4_ = 100;
        corrRatioPD5_ = 96;

    }
    
}
	
// wheel speed
void MotorControl::adjustSpeed(uint8_t ratioPwmPD4, uint8_t ratioPwmPD5) {

    double matchA = ((corrRatioPD5_/100.0)*(ratioPwmPD5));
    double matchB = ((corrRatioPD4_/100.0)*(ratioPwmPD4));
    
    motorPwm_.setMatchA(uint8_t(matchA));
    motorPwm_.setMatchB(uint8_t(matchB));
}


// Forward backward
void MotorControl::setDirection(bool forward) {
    if(!forward)
        digitalWrite(D, 0xC0, ON);
    else
        digitalWrite(D, 0xC0, OFF);
}

void MotorControl::setDirectionPD4(bool forward) {
    if(!forward)
        digitalWrite(D, 0x40, ON);
    else
        digitalWrite(D, 0x40, OFF);
}


void MotorControl::setDirectionPD5(bool forward) {
    if(!forward)
        digitalWrite(D, 0x80, ON);
    else
        digitalWrite(D, 0x80, OFF);
}




uint8_t MotorControl::getRatioPD4() const { 
    return motorPwm_.getMatchB();
}

uint8_t MotorControl::getRatioPD5() const {
    return motorPwm_.getMatchA();
}
