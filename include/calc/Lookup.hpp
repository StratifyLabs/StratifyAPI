/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef LOOKUP_HPP_
#define LOOKUP_HPP_

namespace calc {

/*! \brief Lookup Table Template Class
 * \details This class is for implementing lookup tables
 * using linear extrapolation.
 */
template<typename data_type>class Lookup {
public:
	/*! \details Construct a lookup table object.
	 *
	 * @param table A pointer to a table with x and y values alternating, x values must
	 * be in ascending order
	 * @param size The number of x,y entries in the table
	 */
	Lookup(const data_type * table, int size){
		m_table = table;
		m_size = size;
	}

	/*! \details This calculates the y value using linear extrapolation.
	 *
	 * @param x
	 * @return y value using linear extrapolation
	 */
	data_type calc_value(data_type x){
		unsigned int p1, p2;
		unsigned int i;
		data_type delta_x, delta_y;
		data_type output;

		i = 0;
		while( (x >= m_table[i+2]) && (i < (m_size-2)*2) ){
			i+=2;
		}

		p1 = i;
		p2 = i+2;

		//now calculate the slope between the y values
		delta_x = m_table[p1] - m_table[p2];
		if( delta_x == 0 ){
			return -1;
		}
		delta_y = m_table[p1+1] - m_table[p2+1];
		output = ((x - m_table[p1]) * delta_y) / delta_x + m_table[p1+1];

		return output;
	}

private:
	const data_type * m_table;
	unsigned int m_size;
};

};

#endif /* LOOKUP_HPP_ */
