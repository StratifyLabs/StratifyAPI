//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved


#include <cstdio>
#include "calc/Pid.hpp"

using namespace calc;

PidF32::PidF32() {
	m_kp = 1.0f;
	m_ki = 0.0f;
	m_kd = 0.0f;
	m_max = 0.0f;
	m_min = -0.0f;
	m_target = 0.0f;
	reset();
}

void PidF32::reset(){
	m_integral = 0.0;
	m_error = 0.0;
}

float PidF32::calculate_control_variable(float input){
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

