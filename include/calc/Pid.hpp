/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef PID_HPP_
#define PID_HPP_

#include "../api/SObject.hpp"

namespace calc {

/*! \brief PID Control Loop Class (float)
 * \details This class implements a PID control loop using floating point
 * values.
 *
 * A PID control loop uses three constant values in order to calculate a control
 * variable such that the input converges on the target variable.
 *
 * The following terminology is used in this documentation.
 *
 * - kp: Proportional constant
 * - ki: Integral constant
 * - kd: Differential constant
 * - present value: the current value of the system output
 * - target variable: desired output value of the system
 * - control variable: calculated value to apply such that the input tends to the target variable
 * - error variable: the error variable is the difference between the target variable and the present value
 *
 * An example for using a PID control loop is temperature control. Consider a system
 * that uses a PWM value to heat an element and a temperature sensor to read the output.
 * The above terminology is applied as follows:
 *
 * - kp: Value selected by the developer to tune the control loop
 * - ki: Value selected by the developer to tune the control loop
 * - kd: Value selected by the developer to tune the control loop
 * - present value: the value of the temperature sensor output
 * - target variable: the desired temperature sensor output
 * - control variable: the PWM duty cycle that should be applied to the heater
 * - error variable: the different between the desired temperature output and the present temperature output
 *
 * \code
 * #include <sapi/calc.hpp>
 * #include <sapi/hal.hpp>
 *
 * Pid_f pid_loop;
 * Pwm control(0);
 * Adc present_value(0);
 * u16 adc_value;
 *
 * volatile bool is_active = true;
 *
 * pid_loop.set_kp(1.0);
 * pid_loop.set_ki(0.1);
 * pid_loop.set_kd(0.001);
 * pid_loop.set_maximum(1000); //max duty cycle
 * pid_loop.set_minimum(0); //min duty cycle
 *
 *  //Init the PWM output and ADC input
 *
 * while( is_active ){
 *  present_value.read(0, &adc_value, 2);
 *	control.set_duty( pid_looop.calc_control_variable( 3.3 * adc_value / 4096 ) );
 * }
 *
 * \endcode
 *
 *
 */
class Pid_f {
public:

	/*! \details Constructs a new PID (float) object.
	 *
	 * @param target The target value
	 * @param kp The proportional constant
	 * @param ki The integral constant
	 * @param kd The differential constant
	 * @param min The minimum value for the calculated control value
	 * @param max The maximum value for the calculated control value
	 */
	Pid_f(float target = 0.0, float kp = 1.0, float ki = 0.1, float kd = 0.0, float min = 1.0, float max = 0.0);

	/*! \details Resets the state of the PID control loop. */
	void reset();

	/*! \details Sets the proportional constant value. */
	void set_kp(float v){ m_kp = v; }
	/*! \details Sets the integral constant value. */
	void set_ki(float v){ m_ki = v; }
	/*! \details Sets the differential constant value. */
	void set_kd(float v){ m_kd = v; }
	/*! \details Sets the maximum allowed value of the target variable. */
	void set_max(float v){ m_max = v; }
	/*! \details Sets the minimum allowed value of the target variable. */
	void set_min(float v){ m_min = v; }
	/*! \details Sets the value for the target variable. */
	void set_target(float v){ m_target = v; }

	/*! \details Returns the proportional constant. */
	float kp() const { return m_kp; }
	/*! \details Returns the integral constant. */
	float ki() const { return m_ki; }
	/*! \details Returns the differential constant. */
	float kd() const { return m_kd; }
	/*! \details Returns the maximum value for the control variable. */
	float max() const { return m_max; }
	/*! \details Returns the minimum value for the control variable. */
	float min() const { return m_min; }
	/*! \details Returns the target variable. */
	float target() const { return m_target; }

	/*! \details Calculates the control variable based on
	 * the current state of the system.
	 *
	 * @param present_value The present value of the system
	 * @return The updated control variable to be applied to the system
	 */
	float calc_control_variable(float present_value);
	float calc_value(float present_value){ return calc_control_variable(present_value); }

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
