/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef PID_HPP_
#define PID_HPP_

namespace calc {

class Pid_f {
public:
	Pid_f(float target = 0.0, float kp = 1.0, float ki = 0.1, float kd = 0.0, float min = 1.0, float max = 0.0);

	void reset();

	inline void set_kp(float v){ m_kp = v; }
	inline void set_ki(float v){ m_ki = v; }
	inline void set_kd(float v){ m_kd = v; }
	inline void set_max(float v){ m_max = v; }
	inline void set_min(float v){ m_min = v; }
	inline void set_target(float v){ m_target = v; }

	inline float kp() const { return m_kp; }
	inline float ki() const { return m_ki; }
	inline float kd() const { return m_kd; }
	inline float max() const { return m_max; }
	inline float min() const { return m_min; }
	inline float target() const { return m_target; }

	float calc_value(float input);

private:
	float m_target;
	float m_error;
	float m_kp;
	float m_ki;
	float m_kd;
	float m_integral;
	float m_max;
	float m_min;
};

}

#endif /* PID_HPP_ */
