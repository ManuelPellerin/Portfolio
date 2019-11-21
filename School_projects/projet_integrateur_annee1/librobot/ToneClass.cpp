#include "ToneClass.h"

#include "debug.h"

Tone::Tone(): Pwm(0,0,CNT0), pwmValue_(0){
  DDRB |= (1 << PD2);
  PORTB |= (1 << PD2); 
}

void Tone::stopTone(){
  stopPwm();
}

void Tone::midiToPwm(uint8_t& midiIndex) {
  double frequency = frequencies[midiIndex - 45];
  // Using the formula found in atmel's datasheet
  // to find the pwm value for a set frequency
  pwmValue_ = uint8_t((F_CPU/(2*256*frequency))-1); 
}

void Tone::playTone( uint8_t midiIndex){
	startPwm();
	pwmValue_ = 0x00;
	midiToPwm(midiIndex);
	setMatchA(pwmValue_);
  startPwm();
}


