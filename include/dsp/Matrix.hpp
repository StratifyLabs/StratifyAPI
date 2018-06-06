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

class MatrixQ15 : public Matrix<q15_t, arm_matrix_instance_q15> {
public:

    MatrixQ15(){
        arm_dsp_api_q15()->mat_init(instance(), 0, 0, 0);
    }

    MatrixQ15(u16 rows, u16 columns) : Matrix<q31_t>(rows, columns){
        arm_dsp_api_q15()->mat_init(instance(), rows, columns, vector_data());
    }

    MatrixQ15 operator + (const MatrixQ15 & a) const {
        MatrixQ15 ret(rows(), columns());
        arm_dsp_api_q15()->mat_add(instance(), a.instance(), ret.instance());
        ret.set_transfer_ownership();
        return ret;
    }

    MatrixQ15 operator - (const MatrixQ15 & a) const {
        MatrixQ15 ret(rows(), columns());
        arm_dsp_api_q15()->mat_sub(instance(), a.instance(), ret.instance());
        ret.set_transfer_ownership();
        return ret;
    }

private:



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

class MatrixF32 : public Matrix<float32_t, arm_matrix_instance_f32> {
public:

    MatrixF32(){
        arm_dsp_api_f32()->mat_init(instance(), 0, 0, 0);
    }

    MatrixF32(u16 rows, u16 columns) : Matrix<float32_t>(rows, columns){
        arm_dsp_api_f32()->mat_init(instance(), rows, columns, vector_data());
    }

    MatrixF32 operator + (const MatrixF32 & a) const {
        MatrixF32 ret(rows(), columns());
        arm_dsp_api_f32()->mat_add(instance(), a.instance(), ret.instance());
        ret.set_transfer_ownership();
        return ret;
    }

    MatrixF32 operator - (const MatrixF32 & a) const {
        MatrixF32 ret(rows(), columns());
        arm_dsp_api_f32()->mat_sub(instance(), a.instance(), ret.instance());
        ret.set_transfer_ownership();
        return ret;
    }

private:



};

#endif // DSP_MATRIX_HPP
