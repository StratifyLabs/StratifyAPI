//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include "calc/Pid.hpp"

using namespace calc;

Pid_f::Pid_f(float target, float kp, float ki, float kd, float min, float max) {
	m_kp = kp;
	m_ki = ki;
	m_kd = kd;
	m_max = max;
	m_min = min;
	m_target = target;
	reset();
}

void Pid_f::reset(){
	m_integral = 0.0;
	m_error = 0.0;
}

float Pid_f::calc_control_variable(float input){
	float err;
	float output;
	float de;

	err = (m_target - input);
	de = err - m_error; //new value minus older value
	m_error = err;

	output = err * m_kp  + (m_integral + err) * m_ki + de*m_kd;

	if( output > m_max ){
		output = m_max;
	} else if( output < m_min ){
		output = m_min;
	} else {
		m_integral += (err);
	}

	return output;
}

