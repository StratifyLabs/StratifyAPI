#ifndef DSP_SIGNAL_DATA_HPP
#define DSP_SIGNAL_DATA_HPP

#include <arm_dsp_api.h>
#include "../var/Data.hpp"

namespace dsp {

template<typename signal_type> class SignalData : public var::Data {
public:
    SignalData(int count) : Data(count*sizeof(signal_type)){
        m_count = count;
    }

    SignalData(void * buffer, int count) : Data(buffer, count*sizeof(signal_type)){
        m_count = count;
    }

    int resize(u32 count){
        int ret;
        ret = Data::resize(count * sizeof(signal_type));
        if( ret < 0 ){ return ret; }
        m_count = count;
        return 0;
    }

    u32 count() const { return m_count; }

    signal_type * data(){ return (signal_type *)Data::data(); }
    const signal_type * data() const { return (const signal_type *)Data::data_const(); }
private:
    u32 m_count;
};

class SignalQ7 : public SignalData<q7_t> {
public:
    SignalQ7(int count) : SignalData(count){}
    SignalQ7(void * buffer, int count): SignalData(buffer, count){}
};

class SignalQ15 : public SignalData<q15_t> {
public:
    SignalQ15(int count) : SignalData(count){}
    SignalQ15(void * buffer, int count): SignalData(buffer, count){}
};

class SignalQ31 : public SignalData<q31_t> {
public:
    SignalQ31(int count) : SignalData(count){}
    SignalQ31(void * buffer, int count): SignalData(buffer, count){}
};

class SignalF32 : public SignalData<float32_t> {
public:
    SignalF32(int count) : SignalData(count){}
    SignalF32(void * buffer, int count): SignalData(buffer, count){}
};

typedef SignalF32 SignalFloat32;

}

#endif // DSP_SIGNAL_DATA_HPP
