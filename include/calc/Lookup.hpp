/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_CALC_LOOKUP_HPP_
#define SAPI_CALC_LOOKUP_HPP_

#include "../api/CalcObject.hpp"
#include "../arg/Argument.hpp"

namespace calc {

/*! \brief Lookup Table Template Class
 * \details This class is for implementing lookup tables
 * using linear extrapolation. The template type
 * must be signed or overflow errors will cause
 * problems with the calculations.
 *
 * ```
 * //md2code:include
 * #include <sapi/calc.hpp>
 *	#define ENTRIES 4
 *
 * const float lookup_table[ENTRIES*2] = {  //for each entry there are 2 float values
 * 	0.0, 2.0,  //this is an x,y pair where x is 0.0 and y is 2.0, list must have x values in ascending order
 * 	1.0, 4.0,
 * 	2.0, 16.0,
 * 	3.0, 25.0
 * };
 * ```
 *
 * ```
 * //md2code:main
 *	Lookup<float> lookup(lookup_table, ENTRIES);
 * printf("Value is %0.2f\n", lookup.calculate(1.5f));
 * ```
 *
 */
template<typename T>class Lookup : public api::WorkObject {
public:
	/*! \details Constructs a lookup table object.
	 *
	 * @param table
	 */
	Lookup(
			const T * table /*! A pointer to a table with x and y values alternating, x values must be in ascending order */,
			u32 count /*! The number of entries in the table */
			){
		m_table = table;
		m_count = count;
	}

	/*! \details Calculates the y value using linear interpolation.
	 *
	 * @param x Input value
	 * @return y Value calculated using linear interpolation
	 */
	T calculate(T x){
		unsigned int p1, p2;
		unsigned int i;
		T delta_x, delta_y;
		T output;

		i = 0;
		while( (x > m_table[i+2]) && (i < (m_count-2)*2) ){
			i+=2;
		}

		p1 = i;
		p2 = i+2;

		//now calculate the slope between the y values
		delta_x = m_table[p1] - m_table[p2];
		if( delta_x == 0 ){
			return m_table[p1+1];
		}
		delta_y = m_table[p1+1] - m_table[p2+1];
		output = ((x - m_table[p1]) * delta_y + delta_x/2) / delta_x + m_table[p1+1];


		return output;
	}

private:
	const T * m_table;
	unsigned int m_count;
};

}

#endif /* SAPI_CALC_LOOKUP_HPP_ */
