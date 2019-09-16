#include "calc/Filter.hpp"

using namespace calc;


HighPassFilterF32::HighPassFilterF32(float start, float r_value){
	m_r_value = r_value;
	reset(start);
}

void HighPassFilterF32::reset(float start){
	m_last_input = start;
	set_present_value(0.0f);
}

void HighPassFilterF32::set_r_value(float r_value){
	m_r_value = r_value;
}

float HighPassFilterF32::calculate(float input){
	m_present_value = input - m_r_value * m_last_input + m_present_value;
	m_last_input = input;
	return m_present_value;
}

LowPassFilterF32::LowPassFilterF32(float start, float alpha){ m_alpha = alpha; m_present_value = start; }

float LowPassFilterF32::calculate(float in){
	return (m_present_value = in * (m_alpha) + m_present_value * (1.0f - m_alpha));
}

void LowPassFilterF32::reset(float start){
	set_present_value(start);
}

float LowPassFilterF32::calculate_alpha(float sampling_frequency, float magnitude){

	return -1;
}
