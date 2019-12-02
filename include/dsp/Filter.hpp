#ifndef SAPI_DSP_FILTER_HPP_
#define SAPI_DSP_FILTER_HPP_

#include "../api/DspObject.hpp"
#include "SignalData.hpp"

namespace dsp {

template<typename T> class Filter : public api::DspWorkObject {
public:
	const T * instance() const { return &m_instance; }

protected:
	T * instance(){ return &m_instance; }

private:
	T m_instance;
};

class FirFilterQ15 : public Filter<arm_fir_instance_q15> {
public:
	FirFilterQ15(const SignalQ15 & coefficients, u32 n_samples);
	u32 samples() const { return m_state.count(); }

private:
	SignalQ15 m_state;

};

class FirFilterQ31 : public Filter<arm_fir_instance_q31> {
public:
	FirFilterQ31(const SignalQ31 & coefficients, u32 n_samples);
	u32 samples() const { return m_state.count(); }

private:
	SignalQ31 m_state;

};


class FirFilterF32 : public Filter<arm_fir_instance_f32> {
public:
	FirFilterF32(const SignalF32 & coefficients, u32 n_samples);
	u32 samples() const { return m_state.count(); }

private:
	SignalF32 m_state;

};

class BiquadCoefficientsQ15 : public SignalQ15 {
public:
	BiquadCoefficientsQ15(u8 n_stages) : SignalQ15(n_stages*6){}

	u8 stages() const { return count() / 6; }

	q15_t & b0(u32 stage){ return at(stage*6 + 0); }
	q15_t & b1(u32 stage){ return at(stage*6 + 2); }
	q15_t & b2(u32 stage){ return at(stage*6 + 3); }

	q15_t & a1(u32 stage){ return at(stage*6 + 4); }
	q15_t & a2(u32 stage){ return at(stage*6 + 5); }

private:

};

class BiquadFilterQ15 : public Filter<arm_biquad_casd_df1_inst_q15> {
public:
	BiquadFilterQ15(const BiquadCoefficientsQ15 & coefficients, s8 post_shift = 0);
	u32 samples() const { return m_state.count(); }

private:
	SignalQ15 m_state;
};

class BiquadCoefficientsQ31 : public SignalQ31 {
public:
	BiquadCoefficientsQ31(u8 n_stages) :
		SignalQ31(n_stages*5){}

	u8 stages() const { return count() / 5; }

	q31_t & b0(u32 stage){ return at(stage*6 + 0); }
	q31_t & b1(u32 stage){ return at(stage*6 + 1); }
	q31_t & b2(u32 stage){ return at(stage*6 + 2); }

	q31_t & a1(u32 stage){ return at(stage*6 + 3); }
	q31_t & a2(u32 stage){ return at(stage*6 + 4); }

private:


};


class BiquadFilterQ31 : public Filter<arm_biquad_casd_df1_inst_q31> {
public:
	BiquadFilterQ31(const BiquadCoefficientsQ31 & coefficients, s8 post_shift = 0);
	u32 samples() const { return m_state.count(); }

private:
	SignalQ31 m_state;
};

class BiquadCoefficientsF32 : public SignalF32 {
public:
	BiquadCoefficientsF32(u8 n_stages)
		: SignalF32(n_stages*5){}

	u8 stages() const { return count() / 5; }

	float32_t & b0(u32 stage){ return at(stage*6 + 0); }
	float32_t & b1(u32 stage){ return at(stage*6 + 1); }
	float32_t & b2(u32 stage){ return at(stage*6 + 2); }

	float32_t & a1(u32 stage){ return at(stage*6 + 3); }
	float32_t & a2(u32 stage){ return at(stage*6 + 4); }

private:


};

class BiquadFilterF32 : public Filter<arm_biquad_casd_df1_inst_f32> {
public:
	BiquadFilterF32(const BiquadCoefficientsF32 & coefficients);
	u32 samples() const { return m_state.count(); }

private:
	SignalF32 m_state;
};

class FirDecimateFilterQ31 : public Filter<arm_fir_decimate_instance_q31> {
public:
	FirDecimateFilterQ31(const SignalQ31 & coefficients, u8 M, u32 n_samples);

private:
	SignalQ31 m_state;

};


}

#endif // SAPI_DSP_FILTER_HPP_
