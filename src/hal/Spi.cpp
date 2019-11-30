//Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#if !defined __link
#include "fs/Aio.hpp"
#include "chrono/Timer.hpp"
#endif

#include "hal/Spi.hpp"

using namespace hal;

Spi::Spi(port_t port) : Periph(CORE_PERIPH_SPI, port){}


int Spi::swap(int byte) const {
	return ioctl(
				IoRequest(I_SPI_SWAP),
				IoIntArgument(byte)
				);
}


#if !defined __link
int Spi::transfer(
		const arg::SourceBuffer write_data,
		arg::DestinationBuffer read_data,
		arg::Size nbytes
		){

	fs::Aio aio = fs::Aio(
				read_data,
				nbytes
				);

	int result;

	result = read(aio);
	if( result < 0 ){
		return result;
	}

	result = write(
				write_data,
				nbytes
				);

	while( aio.is_busy() ){ //aio must live until the read completes -- or big problems will happen
		chrono::wait(chrono::Microseconds(200));
	}

	return result;
}
#endif

SpiInfo Spi::get_info() const {
	spi_info_t info;
	if( get_info(info) < 0 ){
		return SpiInfo();
	}

	return SpiInfo(info);
}
