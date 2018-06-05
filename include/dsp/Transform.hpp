#ifndef DSP_TRANSFORM_HPP
#define DSP_TRANSFORM_HPP

#include "../api/DspObject.hpp"
#include "SignalData.hpp"

namespace dsp {

template<typename T> class Fft : public api::DspWorkObject {
public:
    const T * instance() const { return &m_instance; }
    T * instance(){ return &m_instance; }

private:
    T m_instance;
};

class ComplexFftQ15 : public api::DspWorkObject {
public:
    ComplexFftQ15(u32 n_samples);
    u32 samples() const { return instance()->fftLen; }

    const arm_cfft_instance_q15 * instance() const { return m_instance; }

private:
    const arm_cfft_instance_q15 * m_instance;
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
class ComplexFftQ31 : public api::DspWorkObject {
public:

    /*! \details Contructs a new object.
     *
     * @param n_samples The number of samples to compute on each transform
     *
     * The object is passed to the signal transform() methods.
     *
     *
     */
    ComplexFftQ31(u32 n_samples);

    /*! \details Returns the number of samples used on each computation. */
    u32 samples() const { return instance()->fftLen; }

    const arm_cfft_instance_q31 * instance() const { return m_instance; }

    SignalComplexQ31 create_signal() const {
        return SignalComplexQ31( samples()*2 );
    }

private:
    const arm_cfft_instance_q31 * m_instance;
};

class ComplexFftF32 : public api::DspWorkObject {
public:
    ComplexFftF32(u32 n_samples);
    u32 samples() const { return instance()->fftLen; }

    arm_cfft_instance_f32 * instance(){ return &m_instance; }
    const arm_cfft_instance_f32 * instance() const { return &m_instance; }

private:
    arm_cfft_instance_f32 m_instance;
};

class RealFftQ15 : public Fft<arm_rfft_instance_q15> {
public:
    RealFftQ15(u32 n_samples);
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
class RealFftQ31 : public Fft<arm_rfft_instance_q31> {
public:

    /*! \details Contructs an object.
     *
     * @param n_samples The number of samples to compute on each transform
     *
     * The n_samples value must be a power of 2 between 32 and 2048.
     *
     */
    RealFftQ31(u32 n_samples);


    /*! \details Returns the number of samples computed on each transform. */
    u32 samples() const { return instance()->fftLenReal; }


    /*! \details Returns a signal capable of holding FFT source data and IFFT
     * destination data.
     */
    SignalComplexQ31 create_source_signal() const {
        SignalComplexQ31 output( samples() );
        output.set_transfer_ownership();
        return output;
    }

    /*! \details Returns a signal capable of holding FFT destination data and IFFT
     * source data.
     */
    SignalComplexQ31 create_destination_signal() const {
        SignalComplexQ31 output( samples()*2 );
        output.set_transfer_ownership();
        return output;
    }

private:

};

class RealFftF32 : public Fft<arm_rfft_fast_instance_f32> {
public:
    RealFftF32(u32 n_samples);
    u32 samples() const {
        return instance()->fftLenRFFT;
    }

private:

};

//Add DCT classes

}

#endif // DSP_TRANSFORM_HPP
