
#include "SensorClass.h"
#include "debug.h"


Sensor::Sensor()
	:sensorState_(0x00)
	,upperThreshold_{255,255,255,255,255}
	,lowerThreshold_{0,0,0,0,0}{
	DDRC |= 0xFC;//lazy coding, I need to change that
}

uint8_t Sensor::getSensorState(){
	updateSensorState();
	return sensorState_;
}

void Sensor::calibrateSensor(GROUND ground){
	for(uint8_t i = 0; i < 5; i++){
		uint8_t analogValue = uint8_t(can_.lecture(i+1) >> 2);
		if(ground == BLACK){
			lowerThreshold_[i] = analogValue + 25;
			
		}
		else if(ground == WHITE){
			upperThreshold_[i] = analogValue - 25;
			
		}
	}	
}

//middle position = 200
uint16_t Sensor::getPosition(){
	//loop dat whole sensor
	uint16_t position = 0;
	uint8_t activeCounter = 0;
	for(uint8_t i = 0; i < 5; i++) {
		if((1 << i) & sensorState_) {
			position += (i * 100);
			activeCounter++;
		}
	}

	if(activeCounter){
		position /= activeCounter;
	}
	else{
		//division by zero :O
		position = 20000; //random high number
	}
	return position;
}

void Sensor::updateSensorState(){
	for(uint8_t i = 0; i < 5; i++) {
		uint8_t analogValue = uint8_t(can_.lecture(i+1) >> 2);

		if (analogValue > upperThreshold_[i]
			&& (sensorState_ & (1 << i))) {
			//DEBUG_PRINT((1));
			sensorState_ &= ~(1 << i);
		}
		else if (analogValue < lowerThreshold_[i]
				 && !(sensorState_ & (1 << i))) {
			//DEBUG_PRINT((2));
			sensorState_ |= (1 << i);
		}
	}
	PORTC = (sensorState_ << 2);
}





