#ifndef DC_MOTOR

class DcMotor{
	private:
		short speed;
		short pin1;
		short pin2;
		short pinSpeed;

	public:
		DcMotor(short pin1_, short pin2_, short pinSpeed_, short speedDefault_){
			speed = speedDefault_;
			pin1 = pin1_;
			pin2 = pin2_;
			pinSpeed = pinSpeed_;
		}
		
		void setSpeed(short speed_){
			speed = speed_;
		}

		void go(){
			digitalWrite(pin1, 1);
			digitalWrite(pin2, 0);
		}
		
		void back(){
			digitalWrite(pin1, 0);
			digitalWrite(pin2, 1);
		}
		
		void goSpeed(short speed){
			digitalWrite(pin1, 1);
			digitalWrite(pin2, 0);
			analogWrite(pinSpeed, speed);
		}

		void backSpeed(short speed){
			digitalWrite(pin1, 0);
			digitalWrite(pin2, 1);
			analogWrite(pinSpeed, speed);
		}

		void stop(){
			digitalWrite(pin1, 1);
			digitalWrite(pin2, 1);
		}
};

#endif