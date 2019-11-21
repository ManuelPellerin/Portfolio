#include "LineFollower.h"
#include "debug.h"

//Needed a second interrupt on
//timer2. This works but isn't
//such a great way to do it.
volatile uint16_t timerCounter = 0;
ISR(TIMER2_OVF_vect){
	timerCounter++;
	TCNT2 = 2;
}

LineFollower::LineFollower(uint8_t currentSegment) : 
    MotorControl(0, 0, true, ROBOT39),
	currentSegment_(currentSegment),
    lastSegment_(0),
	baseSpeed_(baseSpeed),
	kp_(highKp){

	tonePwm_.stopTone();
}

void LineFollower::nextMeme() {
	switch (currentSegment_) {
	case 1:
		segment1();
		break;
	case 2:
		segment2();
		break;
	case 3:
		segment3();
		break;
	case 4:
		segment4();
		break;
	}

	if (currentSegment_ == 4) {
		currentSegment_ = 1;
	}
	else {
		currentSegment_++;
	}
}

void LineFollower::setCurrentSegment(uint8_t currentSegment) {
	currentSegment_ = currentSegment;
    if(currentSegment == 1){
        lastSegment_ = 4;
    }
    else {
        lastSegment_ = (currentSegment + 3) % 4;
    }
    
}

void LineFollower::segment1() {
	
	//PART1 : follow line up to segment -------------------------------
	bool segmentEnd = false;
	uint8_t sensorState = sensor_.getSensorState();
	int lastError = 0, rightMotorSpeed = 0, leftMotorSpeed = 0;
	kp_ = highKp;

	while (sensorState != (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4)) {
		sensorState = sensor_.getSensorState();
		followLine(rightMotorSpeed, leftMotorSpeed, lastError);
	}
	sensorState = 0;										//reset sensorState for later use
	lastError = 0, rightMotorSpeed = 0, leftMotorSpeed = 0; // reset values for later use

	brakeGears();
	adjustSpeed(0, 0); //set speed to 0
	_delay_ms(1500);   // settling time
					   //PART2 GET DOT FROM IR ----------------------------------
	uint8_t dot = 0;
	
	Receiver irReceiver;
	while ((dot < 1) || (dot > 9)) {
		uint16_t packet = irReceiver.receiveInARow();
		// Isolate the first 7 bits of the packet
		dot = packet & 0x7f;
        DEBUG_PRINT((dot));
	}

	//PART2 GET DOT FROM IR ----------------------------------
	uint8_t column = dot % 3; // get col value from dot
	if (column == 0)
		column = 3;
	uint8_t row = ((dot - 1) / 3) + 1; // get the row value from the dot
	//DEBUG_PRINT((row));

	turn90Deg(LEFT);
	seekDot(column, row); //position robot on dot

	tonePwm_.playTone(50); //play sound
	_delay_ms(3000);
	tonePwm_.stopPwm();
	_delay_ms(2000);
	turn90Deg(RIGHT);
	tonePwm_.playTone(50);
	_delay_ms(3000);
	tonePwm_.stopPwm();
	seekSeg2(column); //seek segment 2 from row position

	while (segmentEnd != true) {
		sensorState = sensor_.getSensorState();
		switch (sensorState) {
		case (0):

			break;
		case (SENSOR0 | SENSOR1 | SENSOR2):
		case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3):
            if(currentSegment_ == lastSegment_){
                adjustSpeed(0,0);
                tonePwm_.playTone(67);
                _delay_ms(2000);
                tonePwm_.stopTone();
            }
            else {
               turnLeft(); 
            }
			segmentEnd = true;
			break;

		default:
			followLine(rightMotorSpeed, leftMotorSpeed, lastError);
			break;
		}
	}
}

void LineFollower::segment2() {
	bool segmentDone = false;
	int lastError = 0;
	int rightMotorSpeed = 0;
	int leftMotorSpeed = 0;
	uint8_t straightSection = 0;
	kp_ = highKp;

	// Init timer counter
	Pwm counterPwm(0, 0, CNT2);
	TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22);
	TCNT2 = 2;
	OCR2A = 1;
	TIMSK2 = (1 << TOIE2);

	// Initial speed boost
	adjustSpeed(150, 150);
	_delay_ms(300);

	while (!segmentDone) {
		// Speed section
		if (straightSection == 0 &&
			timerCounter > 70) {
			TIMSK2 = 0; //disable interrupt
			timerCounter = 0;
			setDirection(false);
			_delay_ms(50);
			setDirection(true);
			baseSpeed_ = lowerSpeed;
			straightSection++;
		}
		else if (straightSection == 1 &&
				 timerCounter > 170) {
			setDirection(false);
			_delay_ms(50);
			setDirection(true);
			baseSpeed_ = lowerSpeed;
			TIMSK2 = 0; //disable interrupt
			timerCounter = 0;
			kp_ = lowKp;
			straightSection++;
		}

		// Sensor section
		uint8_t sensorState = sensor_.getSensorState();
		switch (sensorState) {
		case (0):
			break;
		case (SENSOR0 | SENSOR1 | SENSOR2):
		case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3):
			adjustSpeed(lowerSpeed, lowerSpeed);
			_delay_ms(1000);
            if(currentSegment_ == lastSegment_){
                adjustSpeed(0,0);
                tonePwm_.playTone(67);
                _delay_ms(2000);
                tonePwm_.stopTone();
            }
            else {
                turnLeft();
            }
			segmentDone = true;
			baseSpeed_ = baseSpeed;
			adjustSpeed(0, 0);
			break;
		case (SENSOR2 | SENSOR3 | SENSOR4):
		case (SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4):
			turnRight();
			adjustSpeed(150, 150);
			_delay_ms(300);
			baseSpeed_ = baseSpeed;
			TIMSK2 = (1 << TOIE2); //enable interrupt
			break;
		default:
			followLine(rightMotorSpeed, leftMotorSpeed, lastError);
			break;
		}
	}
}

void LineFollower::segment3() {
	bool segmentDone = false;
	int rightMotorSpeed = 0;
	int leftMotorSpeed = 0;
	int lastError = 0;
	uint16_t bothSides = 0;
	uint16_t leftSide = 0;
	uint16_t rightSide = 0;
	bool bothSidesUnlock = false;
	bool leftSideUnlock = false;
	bool rightSideUnlock = false;
	bool leftFirst = false;
	bool rightFirst = false;
	kp_ = lowKp;

	while (!segmentDone) {
		uint8_t sensorState = sensor_.getSensorState();
		switch (sensorState) {
		case (0):
			break;
		case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4):
			segmentDone = true;
			adjustSpeed(0, 0);
			break;
		default:
			followLine(rightMotorSpeed, leftMotorSpeed, lastError);
			break;
		}
	}
	segmentDone = false;
	while (1) {
		if (digitalRead(D, 0x04)) {
			_delay_ms(15);
			if (digitalRead(D, 0x04))
				break;
		}
	}

	while (!segmentDone) {

		uint8_t sensorState = sensor_.getSensorState();

		switch (sensorState) {
		case (0):
			break;

		case (SENSOR0 | SENSOR1):
		case (SENSOR0 | SENSOR1 | SENSOR2):
		case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3):

			_delay_ms(30);
			if ((sensor_.getSensorState() == (SENSOR0 | SENSOR1)) || (sensor_.getSensorState() == (SENSOR0 | SENSOR1 | SENSOR2)) || (sensor_.getSensorState() == (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3))) {
				bothSidesUnlock = false;
				if (rightSideUnlock) {
					rightSideUnlock = false;
					rightFirst = true;
				}
				leftSideUnlock = true;
			}
			break;

		case (SENSOR3 | SENSOR4):
		case (SENSOR2 | SENSOR3 | SENSOR4):
		case (SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4):

			_delay_ms(30);
			if ((sensor_.getSensorState() == (SENSOR3 | SENSOR4)) || (sensor_.getSensorState() == (SENSOR2 | SENSOR3 | SENSOR4)) || (sensor_.getSensorState() == (SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4))) {
				bothSidesUnlock = false;
				if (leftSideUnlock) {
					leftSideUnlock = false;
					leftFirst = true;
				}
				rightSideUnlock = true;
			}
			break;
		case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4):
			_delay_ms(50);
			if (sensor_.getSensorState() == (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4))
				bothSidesUnlock = true;
			break;

		// Dead cases
		case (SENSOR0):
		case (SENSOR4):
			break;

		default:
			followLine(rightMotorSpeed, leftMotorSpeed, lastError);
			break;
		}
		if (bothSidesUnlock)
			bothSides++;
		if (leftSideUnlock)
			leftSide++;
		if (rightSideUnlock)
			rightSide++;

		if (leftFirst && (rightSide != 0)) {
			adjustSpeed(0, 0);
			segmentDone = true;
		}
		else if (rightFirst && (leftSide != 0)) {
			adjustSpeed(0, 0);
			segmentDone = true;
		}
	}

	if (rightFirst) {
		DELControl(C, 0x7C, OFF);
		double distanceRatio = rightSide / bothSides;
		if (distanceRatio > 1.5) {
			DELControl(C, 0x10, ON);
			_delay_ms(2000);
			DELControl(C, 0x10, OFF);
		}

		else if (distanceRatio < 1.5) {
			DELControl(C, 0x20, ON);
			_delay_ms(2000);
			DELControl(C, 0x20, OFF);
		}
	}
	else if (leftFirst) {
		DELControl(C, 0x7C, OFF);
		double distanceRatio = leftSide / bothSides;
		if (distanceRatio > 1.5) {
			DELControl(C, 0xFF, OFF);
			DELControl(C, 0x04, ON);
			_delay_ms(2000);
			DELControl(C, 0x04, OFF);
		}

		else if (distanceRatio < 1.5) {
			DELControl(C, 0xFF, OFF);
			DELControl(C, 0x08, ON);
			_delay_ms(2000);
			DELControl(C, 0x08, OFF);
		}
	}

	while (1) {
		if (digitalRead(D, 0x04)) {
			_delay_ms(15);
			if (digitalRead(D, 0x04))
				break;
		}
	}
	segmentDone = false;
	while (!segmentDone) {
		uint8_t sensorState = sensor_.getSensorState();
		switch (sensorState) {
		case (0):
			break;
		case (SENSOR0 | SENSOR1 | SENSOR2):
		case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3):
            if(currentSegment_ == lastSegment_){
                adjustSpeed(0,0);
                tonePwm_.playTone(67);
                _delay_ms(2000);
                tonePwm_.stopTone();
            }
            else {
                turnLeft();
            }
			segmentDone = true;
			adjustSpeed(0, 0);
			break;
		default:
			followLine(rightMotorSpeed, leftMotorSpeed, lastError);
			break;
		}
	}
}


void LineFollower::segment4()
{
	kp_ = midKp;

	bool segmentDone = false;
	for (int i = 0; i < NB_BOITES + 1; i++) {
		uint8_t sensorState = 0;
		int lastError = 0, rightMotorSpeed = 0, leftMotorSpeed = 0;
		while (sensorState != (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4) && !segmentDone) {
			sensorState = sensor_.getSensorState();
			switch (sensorState) {
			case (0):

				break;
			case (SENSOR0 | SENSOR1 | SENSOR2):
			case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3):
				if (i == NB_BOITES) {
                    if(currentSegment_ == lastSegment_){
                        adjustSpeed(0,0);
                        tonePwm_.playTone(67);
                        _delay_ms(2000);
                        tonePwm_.stopTone();
                    }
                    else {
                        turnLeft();
                    }
					
					segmentDone = true;
                    adjustSpeed(0,0);
				}
				break;
			
			case (SENSOR1 | SENSOR2 | SENSOR3):
			case (SENSOR2 | SENSOR3 | SENSOR4):
			case (SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4):
			case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4):
				adjustSpeed(BASE_SPEED, BASE_SPEED);
				break;
			default:
				followLine(rightMotorSpeed, leftMotorSpeed, lastError);
				break;
			}
		}

		//TODO playTOne
		if (i < NB_BOITES) {
			seg4PlaySound();
			followBox();
			seg4PlaySound();
		}
	}
	adjustSpeed(0, 0);
}

Pwm LineFollower::getTonePwm() const {
	return tonePwm_;
}

void LineFollower::calibrate() {

	sensor_.calibrateSensor(BLACK);
	_delay_ms(25);
	setDirection(true);
	_delay_ms(25);
	adjustSpeed(250, 250);
	_delay_ms(300);
	adjustSpeed(0, 0);
	_delay_ms(2000);
	sensor_.calibrateSensor(WHITE);
	PORTC = 0XFC;
	_delay_ms(500);
	PORTC = 0X00;
}

void LineFollower::followLine(int rightMotorSpeed, int leftMotorSpeed, int lastError) {
	uint16_t position = sensor_.getPosition();
	int error = position - 200;
	int motorSpeed = kp_ * error + Kd * (error - lastError);
	lastError = error;
	rightMotorSpeed = baseSpeed_ - motorSpeed;
	leftMotorSpeed = baseSpeed_ + motorSpeed;
	if (rightMotorSpeed > maxSpeed) {
		rightMotorSpeed = maxSpeed;
	}
	if (leftMotorSpeed > maxSpeed) {
		leftMotorSpeed = maxSpeed;
	}
	if (rightMotorSpeed < 0) {
		rightMotorSpeed = 0;
	}
	if (leftMotorSpeed < 0) {
		leftMotorSpeed = 0;
	}
	adjustSpeed(leftMotorSpeed, rightMotorSpeed);
}

void LineFollower::followBox() {
	uint8_t sensorState = 0;
	uint8_t counter = 0;
	while (sensorState != (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4)) {
		sensorState = sensor_.getSensorState();
		_delay_ms(25);
		switch (sensorState) {

		case 0:
			++counter;
			if (counter > 20) {
				adjustSpeed(BASE_SPEED, BASE_SPEED);
				counter = 0;
			}
			break;
		case SENSOR0:
			adjustSpeed((BASE_SPEED), BASE_SPEED * SMALL_INCREMENT);
			break;

		case SENSOR4:
			adjustSpeed((baseSpeed_ * SMALL_INCREMENT), baseSpeed_);
			break;

		case (SENSOR0 | SENSOR1 | SENSOR2 | SENSOR3 | SENSOR4):
			adjustSpeed((BASE_SPEED), BASE_SPEED);
			break;
		}
	}
}

void LineFollower::turnRight() {
	// Set a lower speed
	adjustSpeed(lowerSpeed, lowerSpeed);
	_delay_ms(100);
	// Debounce to make sure it isn't
	// a full line
	uint8_t sensorState = sensor_.getSensorState();
	if ((sensorState & SENSOR0) == SENSOR0) {
		return;
	}
	// Go forward to put the center of
	// rotation on the corner
	_delay_ms(1800);
	adjustSpeed(0, 0);
	_delay_ms(500);
	// Start turning to the right by giving
	// a small boost at the begining
	setDirectionPD5(false);
	adjustSpeed(200, 200);
	_delay_ms(50);
	adjustSpeed(100, 100);
	// Align the robot on the center sensor
	alignCenter(sensorState);
	// Reset the direction befor exiting.
	setDirection(true);
}

void LineFollower::turnLeft() {
	// Set a lower speed
	adjustSpeed(lowerSpeed, lowerSpeed);
	_delay_ms(100);
	// Debounce to make sure it isn't
	// a full line
	uint8_t sensorState = sensor_.getSensorState();
	if ((sensorState & SENSOR4) == SENSOR4) {
		return;
	}
	// Go forward to put the center of
	// rotation on the corner
	_delay_ms(800);
	adjustSpeed(0, 0);
	_delay_ms(500);
	// Start turning to the right by giving
	// a small boost at the begining
	setDirectionPD4(false);
	adjustSpeed(200, 200);
	_delay_ms(50);
	adjustSpeed(100, 100);
    _delay_ms(500);
	// Align the robot on the center sensor
	alignCenter(sensorState);
	// Reset the direction befor exiting.
	setDirection(true);
}

void LineFollower::turn90Deg(uint8_t direction) { // select direction ( LEFT or RIGHT)
	if (direction == LEFT) {
		setDirectionPD4(BACKWARD);  //start turn
		setDirectionPD5(FORWARD);
		startBoost();
		adjustSpeed(110, 110);
		_delay_ms(1000);  
	}

	if (direction == RIGHT) {
		setDirectionPD4(FORWARD);  //start turn
		setDirectionPD5(BACKWARD);
		startBoost();
		adjustSpeed(110, 110);
		_delay_ms(960);  
	}



	if (direction == LEFT) {
		setDirectionPD4(FORWARD);  //start turn
		setDirectionPD5(BACKWARD);
		_delay_ms(150);
	}

	if (direction == RIGHT) {
		setDirectionPD4(BACKWARD);  //start turn
		setDirectionPD5(FORWARD);
		_delay_ms(150);
	}

	_delay_ms(150);
	adjustSpeed(0, 0);
	setDirection(FORWARD);
	_delay_ms(1000); // settling time.
}

void LineFollower::alignCenter(uint8_t &sensorState) {
	// Turn until we've hit on the middle
	// sensor for X cycles of 1 ms
	uint16_t waitCounter = 0;
	while (waitCounter < 700) {
		sensorState = sensor_.getSensorState();
		switch (sensorState) {
		case SENSOR2:
			adjustSpeed(0, 0);
			waitCounter++;
			break;
		case SENSOR2 | SENSOR1:
		case SENSOR1:
			waitCounter = 0;
			setDirectionPD4(false);
			setDirectionPD5(true);
			adjustSpeed(100, 100);
			break;
		case SENSOR2 | SENSOR3:
		case SENSOR3:
			waitCounter = 0;
			setDirectionPD4(true);
			setDirectionPD5(false);
			adjustSpeed(100, 100);
			break;
		}
		_delay_ms(1);
	}
}

void LineFollower::seg4PlaySound()
{
	_delay_ms(80);
	tonePwm_.playTone(67);
	_delay_ms(50);
	tonePwm_.stopPwm();
	_delay_ms(20);
	tonePwm_.playTone(67);
	_delay_ms(50);
	tonePwm_.stopPwm();
}

//seek the selected column
void LineFollower::seekColumn(uint8_t column){
	if(column == 1){
		startBoost();
		adjustSpeed(150,150);
		_delay_ms(COL1_DELAY);
		brakeGears();
		adjustSpeed(0,0);
		_delay_ms(SETTLING_TIME);
	}

	if(column == 2){
		startBoost();
		adjustSpeed(150,150);
		_delay_ms(COL2_DELAY);
		brakeGears();
		adjustSpeed(0,0);
		_delay_ms(SETTLING_TIME);
	}

	if(column == 3){
		startBoost();
		adjustSpeed(150,150);
		_delay_ms(COL3_DELAY);
		brakeGears();
		adjustSpeed(0,0);
		_delay_ms(SETTLING_TIME);
	}
}
//seek the selected row
void LineFollower::seekRow(uint8_t row) {
	//DEBUG_PRINT((row));
	if(row == 1){
		startBoost();
		adjustSpeed(150,150);
		_delay_ms(ROW1_DELAY);
		brakeGears();
		adjustSpeed(0,0);
		_delay_ms(SETTLING_TIME);
	}

	if(row == 2){
		startBoost();
		adjustSpeed(150,150);
		_delay_ms(ROW2_DELAY);
		brakeGears();
		adjustSpeed(0,0);
		_delay_ms(SETTLING_TIME);
	}

	if(row == 3){
		startBoost();
		adjustSpeed(150,150);
		_delay_ms(ROW3_DELAY);
		brakeGears();
		adjustSpeed(0,0);
		_delay_ms(SETTLING_TIME);
	}
}

void LineFollower::seekDot(uint8_t column, uint8_t row) {

	seekColumn(column);
	turn90Deg(RIGHT); // orient the robot towards dots
	seekRow(row);
}

//seek segment 3
void LineFollower::seekSeg2(uint8_t col) {

	adjustSpeed(150, 150);

	startBoost();
	adjustSpeed(150,150);

	if(col == 1){
		_delay_ms(1750);  // TODO adjust delay  
	}

	if(col == 2){
		_delay_ms(1450);  //TODO adjust delay
	}

	if(col == 3){
		_delay_ms(950); //TODO adjust delay
	}
	
	brakeGears();
	adjustSpeed(0,0); // stop robot
	_delay_ms(SETTLING_TIME);

	setDirectionPD4(BACKWARD);  //start Left turn
	setDirectionPD5(FORWARD);
	startBoost();
	adjustSpeed(110, 110);
	_delay_ms(750);  // OPTIMAL DELAY  --TO BE CALIBRATED EXPERIMENTALLY

	setDirectionPD4(FORWARD);  //start turn
	setDirectionPD5(BACKWARD);
	_delay_ms(150);
	adjustSpeed(0, 0);
	setDirection(FORWARD);
	_delay_ms(1000); // settling time.

	startBoost();
	adjustSpeed(100, 100);

	uint8_t sensorState = sensor_.getSensorState(); // robot goes forward until seg2 Line detected
	while (sensorState == 0) {
		sensorState = sensor_.getSensorState();
	}
}

//Brakes with the gears. Also breaks them.
void LineFollower::brakeGears(){
		setDirection(false);
		_delay_ms(150);
		setDirection(true);
}

void LineFollower::startBoost(){
	adjustSpeed(255,245);
	_delay_ms(100);
}