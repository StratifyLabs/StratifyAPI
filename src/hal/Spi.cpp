//Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

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
		SourceBuffer write_data,
		DestinationBuffer read_data,
		Size nbytes
		){

	fs::Aio aio(
				Location(0),
				read_data.argument(),
				nbytes
				);

	int result;

	result = read(aio);
	if( result < 0 ){
		return result;
	}

	result = write(
				write_data.argument(),
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
