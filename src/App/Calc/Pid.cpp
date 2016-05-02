//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include "calc/Pid.hpp"

using namespace Calc;

Pid_f::Pid_f(float target, float kp, float ki, float kd, float min, float max) {
	_kp = kp;
	_ki = ki;
	_kd = kd;
	_max = max;
	_min = min;
	_target = target;
	reset();
}

void Pid_f::reset(void){
	_integral = 0.0;
	_error = 0.0;
}

float Pid_f::calc(float input){
	float err;
	float output;
	float de;

	err = (_target - input);
	de = err - _error; //new value minus older value
	_error = err;

	output = err * _kp  + (_integral + err) * _ki + de*_kd;
	printf("%f = %f * %f + (%f + %f) * %f + %f*%f\n",
			output,
			err,
			_kp,
			(_integral),
			err,
			_ki,
			de,
			_kd);
	if( output > _max ){
		output = _max;
	} else if( output < _min ){
		output = _min;
	} else {
		_integral += (err);
	}

	printf("Final output is %f\n", output);
	return output;
}

