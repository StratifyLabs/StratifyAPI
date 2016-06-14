//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <cstring>
#include "var/Packet.hpp"

using namespace var;

Packet::Packet() {
	// TODO Auto-generated constructor stub

}



int Packet::send(const void * data, int nbyte){
	//construct the packet
	int ret;
	header_t * hdr;
	char buffer[nbyte + sizeof(header_t)]; //start (2 bytes) size (2 bytes) and checksum (1 byte)
	int len = nbyte + sizeof(header_t);

	hdr = (header_t*)buffer;
	hdr->start = START;
	hdr->size = nbyte;
	memcpy(buffer+len, data, nbyte);
	buffer[nbyte+len-1] = calc_checksum(buffer,len-1);

	ret = write(buffer, len);
	if( ret != (len) ){
		return -1;
	}
	return nbyte;
}

int Packet::recv(void * data, int nbyte, int timeout){

	return 0;
}

uint16_t Packet::calc_checksum(const void * data, int nbyte){
	uint8_t checksum = 0;
	uint8_t * p = (uint8_t*)data;
	int i;
	for(i=0; i < nbyte; i++){
		checksum ^= p[i];
	}
	return checksum;
}
