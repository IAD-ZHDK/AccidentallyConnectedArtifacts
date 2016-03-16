#include <arduino.h>

class Motor {
  int _pinX;
  int _pinY;
  int _pinE;

  bool _direction;
  bool _directionTarget;
  float _speed;
  float _speedTarget;
  
  public:
  Motor(int pin1, int pin2, int pin3){
    _direction = false;
    _directionTarget = true;
    _speed = 0;
    _speedTarget = 0;

    _pinX = pin1;
    _pinY = pin2;
    _pinE = pin3;
  }
  void setup(){
    pinMode(_pinX, OUTPUT); //DIR PIN 1
    pinMode(_pinY, OUTPUT); //DIR PIN 2
    pinMode(_pinE, OUTPUT); //SPEED PIN 
  }
  void update(){
    // ONLY CHANGE DIRECTION IF SPEED = 0 
    if(_direction != _directionTarget && _speed == 0){ 
      _direction = _directionTarget;
      if(_direction){
        digitalWrite(_pinX, LOW);
        digitalWrite(_pinY, HIGH);
      }else {
        digitalWrite(_pinX, HIGH);
        digitalWrite(_pinY, LOW);
      }
    }

    // TARGET SPEED TO ZERO ON DIRECTION CHANGE
    int _speedTemp = _speedTarget;
    if(_directionTarget != _direction){
      _speedTemp = 0;
    }
    
    // APPROACH TARGET SPEED IF NOT COMPLETED
    if(_speed != _speedTemp){
      _speed = ((_speedTemp-_speed)*0.08)+_speed; 
      // PREVENT ENDLESS ITERATION
      if(abs(_speed-_speedTemp) < 2){
        _speed = _speedTemp;
      }
      analogWrite(_pinE, int(_speed));
    }
  }
  void setSpeed(int sp){
    _speedTarget = sp;
  }
  
  void setDirection(bool dir){
    // DONT INTERRUPT A DIRECTION CHANGE
    if(_directionTarget == _direction){
      _directionTarget = dir;
    }
  }
  
  void debug(){
    Serial.print(_direction);
    Serial.print(", ");
    Serial.print(_directionTarget);
    Serial.print(" - ");
    Serial.print(_speed);
    Serial.print(", ");
    Serial.println(_speedTarget);  
  }
};
