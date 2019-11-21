#ifndef PWM_H
#define PWM_H

#ifndef F_CPU
#define F_CPU 8000000U
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

enum Counters {CNT0, CNT1, CNT2};
class Pwm
{
	public:
		Pwm(uint8_t compareMatchValueB = 0, uint8_t compareMatchValueA = 0, uint8_t counter = 1);
		uint8_t getMatchA() const;
		uint8_t getMatchB() const;
		uint8_t getCounter() const;
		void setMatchA (uint8_t match);  
		void setMatchB(uint8_t match);
		void stopPwm();
		void startPwm();
	private:
		uint8_t counter_;
				
		
};
#endif






