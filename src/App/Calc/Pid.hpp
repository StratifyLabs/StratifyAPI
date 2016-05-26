/*
 * Pid.hpp
 *
 *  Created on: Aug 13, 2015
 *      Author: tgil
 */

#ifndef PID_HPP_
#define PID_HPP_

#include <stdint.h>

namespace Dsp {


class Pid_f {
public:
	Pid_f(float target = 0.0, float kp = 1.0, float ki = 0.1, float kd = 0.0, float min = 1.0, float max = 0.0);

	void reset();

	inline void set_kp(float v){ _kp = v; }
	inline void set_ki(float v){ _ki = v; }
	inline void set_kd(float v){ _kd = v; }
	inline void set_max(float v){ _max = v; }
	inline void set_min(float v){ _min = v; }
	inline void set_target(float v){ _target = v; }

	inline float kp() const { return _kp; }
	inline float ki() const { return _ki; }
	inline float kd() const { return _kd; }
	inline float max() const { return _max; }
	inline float min() const { return _min; }
	inline float target() const { return _target; }

	float calc(float input);

private:
	float _target;
	float _error;
	float _kp;
	float _ki;
	float _kd;
	float _integral;
	float _max;
	float _min;
};

}

#endif /* PID_HPP_ */
