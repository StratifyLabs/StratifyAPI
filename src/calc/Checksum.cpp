#include <cstdio>
#include "calc/Checksum.hpp"

using namespace calc;

u8 Checksum::calc_zero_sum(const u8 * data, int size){
	int i;
	u8 sum = 0;
	int count = size/sizeof(u8) - 1;
	for(i=0; i < count; i++){
		sum += data[i];
	}

	return (0 - sum);
}

bool Checksum::verify_zero_sum(const u8 * data, int size){
	int i;
	u8 sum = 0;
	int count = size/sizeof(u8);
	for(i=0; i < count; i++){
		sum += data[i];
	}

	return (sum == 0);
}

u32 Checksum::calc_zero_sum8(const var::Data & data){
	return calc_zero_sum((const u8*)data.data(), data.size());
}

bool Checksum::verify_zero_sum8(const var::Data & data){
	return verify_zero_sum((const u8*)data.data(), data.size());
}

u32 Checksum::calc_zero_sum32(const var::Data & data){
	return calc_zero_sum((const u32*)data.data(), data.size());
}

bool Checksum::verify_zero_sum32(const var::Data & data){
	return verify_zero_sum((const u32*)data.data(), data.size());
}

u32 Checksum::calc_zero_sum(const u32 * data, int size){
	int i;
	u32 sum = 0;
	int count = size/sizeof(u32) - 1;
	for(i=0; i < count; i++){
		sum += data[i];
	}

	return (0 - sum);
}

bool Checksum::verify_zero_sum(const u32 * data, int size){
	int i;
	u32 sum = 0;
	int count = size/sizeof(u32);
	for(i=0; i < count; i++){
		sum += data[i];
	}

	return (sum == 0);
}

