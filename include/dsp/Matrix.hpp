#ifndef DSP_MATRIX_HPP
#define DSP_MATRIX_HPP

#include "../api/DspObject.hpp"
#include "../var/Vector.hpp"

template<typename T, typename M> class Matrix : public var::Vector<T>, public api::DspWorkObject {
public:

    Matrix(){}
    Matrix(u16 rows, u16 columns){
        var::Vector<T>::resize(rows * columns);
    }

    u16 rows() const { return instance()->numRows; }
    u16 columns() const { return instance()->numCols; }

    M * instance(){ return &m_instance; }
    const M * instance(){ return &m_instance; }

private:
    M m_instance;

};

class MatrixQ31 : public Matrix<q31_t, arm_matrix_instance_q31> {
public:

    MatrixQ31(){
        arm_dsp_api_q31()->mat_init(instance(), 0, 0, 0);
    }

    MatrixQ31(u16 rows, u16 columns) : Matrix<q31_t>(rows, columns){
        arm_dsp_api_q31()->mat_init(instance(), rows, columns, vector_data());
    }

    MatrixQ31 operator + (const MatrixQ31 & a) const {
        MatrixQ31 ret(rows(), columns());
        arm_dsp_api_q31()->mat_add(instance(), a.instance(), ret.instance());
        ret.set_transfer_ownership();
        return ret;
    }

    MatrixQ31 operator - (const MatrixQ31 & a) const {
        MatrixQ31 ret(rows(), columns());
        arm_dsp_api_q31()->mat_sub(instance(), a.instance(), ret.instance());
        ret.set_transfer_ownership();
        return ret;
    }

private:



};

#endif // DSP_MATRIX_HPP
