/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_DSP_TRANSFORM_HPP_
#define SAPI_DSP_TRANSFORM_HPP_

#include "../api/DspObject.hpp"
#include "SignalData.hpp"

namespace dsp {

template<typename T, typename SignalType> class Fft : public api::DspWorkObject {
public:
	const T * instance() const { return &m_instance; }
	T * instance(){ return &m_instance; }

	virtual u32 samples() const = 0;

	/*! \details Returns a signal capable of holding FFT source data and IFFT
	  * destination data.
	  */
	SignalType create_time_signal() const {
		SignalType output( samples() );
		return output;
	}

	/*! \details Returns a signal capable of holding FFT destination data and IFFT
	  * source data.
	  */
	SignalType create_frequency_signal() const {
		SignalType output( samples()*2 );
		return output;
	}


private:
	T m_instance;
};

class FftComplexQ15 : public Fft<arm_cfft_instance_q15, SignalComplexQ15> {
public:
	FftComplexQ15(u32 n_samples);
	u32 samples() const { return instance()->fftLen; }
};

/*! \brief Complex FFT for Fixed Point q1.31 format
 *
 * The source data for an FFT transform should match
 * the number of samples the FFT computes. The destination
 * should be the number of samples times two.
 *
 * When performing an inverse, FFT the source data is the
 * number of samples times two and the destination matches
 * the number of samples.
 *
 *
 */
class FftComplexQ31 : public Fft<arm_cfft_instance_q31, SignalComplexQ31> {
public:

	/*! \details Contructs a new object.
	  *
	  * @param n_samples The number of samples to compute on each transform
	  *
	  * The object is passed to the signal transform() methods.
	  *
	  *
	  */
	FftComplexQ31(u32 n_samples);

	/*! \details Returns the number of samples used on each computation. */
	u32 samples() const { return instance()->fftLen; }

};

class FftComplexF32 : public Fft<arm_cfft_instance_f32, SignalComplexF32> {
public:
	FftComplexF32(u32 n_samples);
	u32 samples() const { return instance()->fftLen; }

};

class FftRealQ15 : public Fft<arm_rfft_instance_q15, SignalComplexQ15> {
public:
	FftRealQ15(u32 n_samples);
	u32 samples() const {
		return instance()->fftLenReal;
	}

private:
};

/*!
 * \brief Real FFT on q1.31 Complex data
 * \details The Real FFT object for q1.31.
 *
 *
 * \code
 * RealFftQ31 fft(32);
 * SignalComplexQ31 source(32);
 * SignalComplexQ31 destination;
 * destination = source.transform(fft);
 *
 * //subsequent calls can avoid dyncamic memory allocation
 * source.transform(destination, fft);
 *
 * //Now the inverse
 * destination.transform(source, fft, true);
 *
 * \endcode
 */
class FftRealQ31 : public Fft<arm_rfft_instance_q31, SignalComplexQ31> {
public:

	/*! \details Contructs an object.
	  *
	  * @param n_samples The number of samples to compute on each transform
	  *
	  * The n_samples value must be a power of 2 between 32 and 2048.
	  *
	  */
	FftRealQ31(u32 n_samples);


	/*! \details Returns the number of samples computed on each transform. */
	u32 samples() const { return instance()->fftLenReal; }


private:

};

class FftRealF32 : public Fft<arm_rfft_fast_instance_f32, SignalComplexF32> {
public:
	FftRealF32(u32 n_samples);
	u32 samples() const {
		return instance()->fftLenRFFT;
	}

private:

};

//Add DCT classes

}

#endif // SAPI_DSP_TRANSFORM_HPP_
