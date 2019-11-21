#ifndef SENSOR_H
#define SENSOR_H
#include <avr/io.h>
#include "can.h"

enum GROUND { BLACK, WHITE };

class Sensor{
public:
	Sensor();

	uint8_t getSensorState();

	void calibrateSensor(GROUND ground);
	uint16_t getPosition();
	void updateSensorState();

private:
	uint8_t sensorState_;
	uint8_t upperThreshold_[5];
	uint8_t lowerThreshold_[5];
	can can_;
};


#endif