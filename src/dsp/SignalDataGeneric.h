
native_type SignalType::mean() const {
    native_type result;
    arm_dsp_api_function()->mean((native_type*)data_const(), count(), &result);
    return result;
}

big_type SignalType::power() const {
    big_type result;
    arm_dsp_api_function()->power((native_type*)data_const(), count(), &result);
    return result;
}

native_type SignalType::variance() const {
    native_type result;
    arm_dsp_api_function()->var((native_type*)data_const(), count(), &result);
    return result;
}

native_type SignalType::rms() const {
    native_type result;
    arm_dsp_api_function()->rms((native_type*)data_const(), count(), &result);
    return result;
}

native_type SignalType::std() const {
    native_type result;
    arm_dsp_api_function()->std((native_type*)data_const(), count(), &result);
    return result;
}

native_type SignalType::min() const {
    native_type result;
    u32 idx;
    arm_dsp_api_function()->min((native_type*)data_const(), count(), &result, &idx);
    return result;
}

native_type SignalType::min(u32 & idx) const {
    native_type result;
    arm_dsp_api_function()->min((native_type*)data_const(), count(), &result, &idx);
    return result;
}

native_type SignalType::max() const {
    native_type result;
    u32 idx;
    arm_dsp_api_function()->max((native_type*)data_const(), count(), &result, &idx);
    return result;
}

native_type SignalType::max(u32 & idx) const {
    native_type result;
    arm_dsp_api_function()->max((native_type*)data_const(), count(), &result, &idx);
    return result;
}

SignalType SignalType::abs() const {
    SignalType ret(count());
    arm_dsp_api_function()->abs(
                (native_type *)data_const(),
                ret.vector_data(),
                count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalType::abs(SignalType & output) const {
    arm_dsp_api_function()->abs(
                (native_type *)data_const(),
                output.vector_data(),
                count());
}

big_type SignalType::dot_product(const SignalType & a) const {
    big_type result;
    arm_dsp_api_function()->dot_prod((native_type*)vector_data_const(), (native_type*)a.vector_data_const(), count(), &result);
    return result;
}

SignalType SignalType::negate() const {
    SignalType ret(count());
    arm_dsp_api_function()->negate((native_type*)vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

void SignalType::negate(SignalType & output) const {
    arm_dsp_api_function()->negate((native_type*)vector_data_const(), output.vector_data(), count());
}


SignalType SignalType::convolve(const SignalType & a) const {
    SignalType ret(count() + a.count() - 1);
#if IS_FLOAT == 0
    arm_dsp_api_function()->conv_fast((native_type*)vector_data_const(), count(), (native_type*)a.vector_data_const(), a.count(), ret.vector_data());
#else
    arm_dsp_api_function()->conv((native_type*)vector_data_const(), count(), (native_type*)a.vector_data_const(), a.count(), ret.vector_data());
#endif
    ret.set_transfer_ownership();
    return ret;
}

void SignalType::convolve(SignalType & output, const SignalType & a) const {
    //check output length?
#if IS_FLOAT == 0
    arm_dsp_api_function()->conv_fast((native_type*)vector_data_const(), count(), (native_type*)a.vector_data_const(), a.count(), output.vector_data());
#else
    arm_dsp_api_function()->conv((native_type*)vector_data_const(), count(), (native_type*)a.vector_data_const(), a.count(), output.vector_data());
#endif
}

#if IS_FLOAT == 0
void SignalType::shift(SignalType & output, s8 value) const {
    arm_dsp_api_function()->shift((native_type*)vector_data_const(), value, output.vector_data(), count());
}

SignalDataType SignalType::shift(s8 value) const {
    SignalType ret(count());
    arm_dsp_api_function()->shift((native_type*)vector_data_const(), value, ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

SignalDataType & SignalType::shift_assign(s8 value){
    arm_dsp_api_function()->shift((native_type*)vector_data_const(), value, vector_data(), count());
    return *this;
}
#endif

SignalType SignalType::scale(native_type scale_fraction, s8 shift) const {
    SignalType ret(count());
#if IS_FLOAT == 0
    arm_dsp_api_function()->scale((native_type*)vector_data_const(), scale_fraction, shift, ret.vector_data(), count());
#else
    arm_dsp_api_function()->scale((native_type*)vector_data_const(), scale_fraction, ret.vector_data(), count());
#endif
    ret.set_transfer_ownership();
    return ret;
}

void SignalType::scale(SignalType & output, native_type scale_fraction, s8 shift) const {
#if IS_FLOAT == 0
    arm_dsp_api_function()->scale((native_type*)vector_data_const(), scale_fraction, shift, output.vector_data(), count());
#else
    arm_dsp_api_function()->scale((native_type*)vector_data_const(), scale_fraction, output.vector_data(), count());
#endif
}


SignalDataType SignalType::add(native_type offset_value) const {
    SignalType ret(count());
    arm_dsp_api_function()->offset((native_type*)vector_data_const(), offset_value, ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

SignalDataType & SignalType::add_assign(native_type offset_value){
    arm_dsp_api_function()->offset((native_type*)vector_data_const(), offset_value, vector_data(), count());
    return *this;
}

SignalDataType SignalType::add(const SignalType & a) const {
    SignalType ret(count());
    arm_dsp_api_function()->add((native_type*)vector_data_const(), (native_type*)a.vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}


SignalDataType & SignalType::add_assign(const SignalType & a){
    arm_dsp_api_function()->add((native_type*)vector_data_const(), (native_type*)a.vector_data_const(), vector_data(), count());
    return *this;
}

SignalDataType SignalType::multiply(native_type value) const {
    return scale(value);
}

SignalDataType & SignalType::multiply_assign(native_type value){
    scale(*this, value);
    return *this;
}

SignalDataType SignalType::multiply(const SignalType & a) const {
    SignalType ret(count());
    arm_dsp_api_function()->mult((native_type*)vector_data_const(), (native_type*)a.vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

SignalDataType & SignalType::multiply_assign(const SignalType & a){
    arm_dsp_api_function()->mult((native_type*)vector_data_const(), (native_type*)a.vector_data_const(), vector_data(), count());
    return *this;
}

//subtract
SignalDataType SignalType::subtract(const SignalType & a) const{
    SignalType ret(count());
    arm_dsp_api_function()->sub((native_type*)vector_data_const(), (native_type*)a.vector_data_const(), ret.vector_data(), count());
    ret.set_transfer_ownership();
    return ret;
}

SignalDataType & SignalType::subtract_assign(const SignalType & a){
    arm_dsp_api_function()->sub((native_type*)vector_data_const(), (native_type*)a.vector_data_const(), vector_data(), count());
    return *this;
}

SignalType SignalType::filter(const BiquadFilterType & filter) const {
    SignalType ret(count());
#if IS_FLOAT == 0
    arm_dsp_api_function()->biquad_cascade_df1_fast(filter.instance(), (native_type*)vector_data_const(), ret.vector_data(), count());
#else
    arm_dsp_api_function()->biquad_cascade_df1(filter.instance(), (native_type*)vector_data_const(), ret.vector_data(), count());
#endif
    ret.set_transfer_ownership();
    return ret;
}

void SignalType::filter(SignalType & output, const BiquadFilterType & filter) const {
#if IS_FLOAT == 0
    arm_dsp_api_function()->biquad_cascade_df1_fast(filter.instance(), (native_type*)vector_data_const(), output.vector_data(), count());
#else
    arm_dsp_api_function()->biquad_cascade_df1(filter.instance(), (native_type*)vector_data_const(), output.vector_data(), count());
#endif
}

SignalType SignalType::filter(const FirFilterType & filter) const {
    SignalType ret(count());
#if IS_FLOAT == 0
    arm_dsp_api_function()->fir_fast(filter.instance(), (native_type*)vector_data_const(), ret.vector_data(), count());
#else
    arm_dsp_api_function()->fir(filter.instance(), (native_type*)vector_data_const(), ret.vector_data(), count());
#endif
    ret.set_transfer_ownership();
    return ret;
}

void SignalType::filter(SignalType & output, const FirFilterType & filter) const {
#if IS_FLOAT == 0
    arm_dsp_api_function()->fir_fast(filter.instance(), (native_type*)vector_data_const(), output.vector_data(), count());
#else
    arm_dsp_api_function()->fir(filter.instance(), (native_type*)vector_data_const(), output.vector_data(), count());
#endif
}

#if IS_FLOAT == 1
SignalType SignalType::create_sin_wave(native_type wave_frequency, native_type sampling_frequency, u32 nsamples, native_type phase){
#else
SignalType SignalType::create_sin_wave(u32 wave_frequency, u32 sampling_frequency, u32 nsamples, native_type phase){
#endif
    SignalType ret(nsamples);
    u32 i;
#if IS_FLOAT == 1
    native_type theta = phase; //theta 0 to max is 0 to 2*pi
    native_type theta_step = wave_frequency * sampling_frequency;
#else
    unsigned_native_type theta = phase; //theta 0 to max is 0 to 2*pi
    unsigned_native_type theta_step = (big_type)wave_frequency * LOCAL_INT_MAX / sampling_frequency;
#endif

    //fsamp = 16K and fmax = 8k - theta step is pi or (1/2)
    //theta step is f / fsamp
    for(i=0; i < nsamples; i++){
        ret[i] = arm_dsp_api_function()->sin(theta);
        theta += theta_step;
#if IS_FLOAT == 0
        if( theta > LOCAL_INT_MAX ){
            theta -= (LOCAL_INT_MAX);
        }
#endif
    }

    ret.set_transfer_ownership();
    return ret;
}



void SignalComplexType::transform(FftComplexType & fft, bool is_inverse, bool is_bit_reversal){
    arm_dsp_api_function()->cfft(fft.instance(), (native_type*)vector_data(), is_inverse, is_bit_reversal);
}

void SignalComplexType::transform(SignalComplexType & output, FftRealType & fft, bool is_inverse){
#if IS_FLOAT == 0
    fft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_function()->rfft(fft.instance(), (native_type*)vector_data(), (native_type*)output.vector_data());
#else
    arm_dsp_api_function()->rfft_fast(fft.instance(), (native_type*)vector_data(), (native_type*)output.vector_data(), is_inverse);
#endif
}

SignalComplexType SignalComplexType::transform(FftRealType & fft, bool is_inverse){
    int samples = count();
    if( !is_inverse ){ samples *= 2; }
    SignalComplexType ret(samples);
#if IS_FLOAT == 0
    fft.instance()->ifftFlagR = is_inverse;
    arm_dsp_api_function()->rfft(fft.instance(), (native_type*)vector_data(), (native_type*)ret.vector_data());
#else
    arm_dsp_api_function()->rfft_fast(fft.instance(), (native_type*)vector_data(), (native_type*)ret.vector_data(), is_inverse);
#endif
    ret.set_transfer_ownership();
    return ret;
}


