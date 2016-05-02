/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef LOOKUP_HPP_
#define LOOKUP_HPP_

namespace Calc {

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
		table_ = table;
		size_ = size;
	}

	/*! \details This calculates the y value using linear extrapolation.
	 *
	 * @param x
	 * @return y value using linear extrapolation
	 */
	data_type value(data_type x){
		unsigned int p1, p2;
		unsigned int i;
		data_type delta_x, delta_y;
		data_type output;

		i = 0;
		while( (x >= table_[i+2]) && (i < (size_-2)*2) ){
			i+=2;
		}

		p1 = i;
		p2 = i+2;

		//now calculate the slope between the y values
		delta_x = table_[p1] - table_[p2];
		if( delta_x == 0 ){
			return -1;
		}
		delta_y = table_[p1+1] - table_[p2+1];
		output = ((x - table_[p1]) * delta_y) / delta_x + table_[p1+1];

		return output;
	}

private:
	const data_type * table_;
	unsigned int size_;
};

};

#endif /* LOOKUP_HPP_ */
