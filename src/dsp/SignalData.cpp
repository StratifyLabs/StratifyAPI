#include "dsp/SignalData.hpp"
#include "dsp/Transform.hpp"

using namespace dsp;

void SignalQ31::fft(const CfftQ31 & fft, bool is_bit_reversal){
    arm_dsp_api_q31()->cfft(fft.instance(), vector_data(), 0, is_bit_reversal);
}

void SignalQ31::ifft(const CfftQ31 & fft, bool is_bit_reversal){
    arm_dsp_api_q31()->cfft(fft.instance(), vector_data(), 1, is_bit_reversal);
}

SignalQ31 SignalQ31::fft(const RfftQ31 & fft){
    SignalQ31 ret;
    arm_dsp_api_q31()->rfft(fft.instance(), vector_data(), ret.vector_data());
    ret.set_transfer_ownership();
    return ret;
}

SignalQ31 SignalQ31::ifft(const RfftQ31 & fft){
    RfftQ31 rfft = fft;
    SignalQ31 ret;
    rfft.instance()->ifftFlagR = 1;
    arm_dsp_api_q31()->rfft(rfft.instance(), vector_data(), ret.vector_data());
    ret.set_transfer_ownership();
    return ret;
}
