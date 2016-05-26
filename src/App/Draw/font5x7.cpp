//Copyright 2011-2016 Tyler Gilbert; All Rights Reserved


#include <draw/Font8Util.hpp>

#include "binary.h"
//using namespace Draw;

#ifndef __link

const font8_t font8_5x7 = {
		.w = 5,
		.h = 7,
		.characters = {
				//Space
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(00000000), //7
						1
				}, //8

				// !
				{
						B8(10000000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(10000000), //4
						B8(10000000), //5
						B8(00000000), //6
						B8(10000000), //7
						1
				},
				//"
				{
						B8(10100000), //1
						B8(10100000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(00000000), //7
						3
				},		//#
				{
						B8(00000000), //1
						B8(01010000), //2
						B8(11111000), //3
						B8(01010000), //4
						B8(11111000), //5
						B8(01010000), //6
						B8(00000000), //7
						5
				},
				//$
				{
						B8(00100000), //1
						B8(01110000), //2
						B8(10100000), //3
						B8(01110000), //4
						B8(00101000), //5
						B8(01110000), //6
						B8(00100000), //7
						5
				},
				//%
				{
						B8(00000000), //1
						B8(10100000), //2
						B8(00100000), //3
						B8(01000000), //4
						B8(10000000), //5
						B8(10100000), //6
						B8(00000000), //7
						3
				},
				//&
				{
						B8(01000000), //1
						B8(10100000), //2
						B8(10100000), //3
						B8(01000000), //4
						B8(01010000), //5
						B8(10100000), //6
						B8(01010000), //7
						5
				},
				//'
				{
						B8(10000000), //1
						B8(10000000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(00000000), //7
						1
				},
				//(
				{
						B8(01000000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(10000000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(01000000), //7
						2
				},
				//)
				{
						B8(10000000), //1
						B8(01000000), //2
						B8(01000000), //3
						B8(01000000), //4
						B8(01000000), //5
						B8(01000000), //6
						B8(10000000), //7
						2
				},
				//*
				{
						B8(00000000), //1
						B8(10101000), //2
						B8(01110000), //3
						B8(11111000), //4
						B8(01110000), //5
						B8(10101000), //6
						B8(00000000), //7
						5
				},
				//+
				{
						B8(00000000), //1
						B8(00100000), //2
						B8(00100000), //3
						B8(11111000), //4
						B8(00100000), //5
						B8(00100000), //6
						B8(00000000), //7
						5
				},
				//,
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(01000000), //6
						B8(10000000), //7
						2
				},
				//-
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(00000000), //3
						B8(11100000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(00000000), //7
						3
				},
				//.
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(10000000), //7
						1
				},
				// /
				{
						B8(00001000), //2
						B8(00001000), //3
						B8(00010000), //4
						B8(00100000), //5
						B8(01000000), //6
						B8(10000000), //7
						B8(10000000), //8
						5
				},
				//0
				{
						B8(01110000), //1
						B8(10001000), //2
						B8(10011000), //3
						B8(10101000), //4
						B8(11001000), //5
						B8(10001000), //6
						B8(01110000), //7
						5
				},
				//1
				{
						B8(01000000), //1
						B8(11000000), //2
						B8(01000000), //3
						B8(01000000), //4
						B8(01000000), //5
						B8(01000000), //6
						B8(11100000), //7
						3
				},
				//2
				{
						B8(11100000), //1
						B8(00010000), //2
						B8(00010000), //3
						B8(01100000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(11110000), //7
						4
				},
				//3
				{
						B8(11100000), //1
						B8(00010000), //2
						B8(00010000), //3
						B8(01100000), //4
						B8(00010000), //5
						B8(00010000), //6
						B8(11100000), //7
						4
				},
				//4
				{
						B8(00011000), //1
						B8(00101000), //2
						B8(01001000), //3
						B8(10001000), //4
						B8(11111000), //5
						B8(00001000), //6
						B8(00001000), //7
						5
				},
				//5
				{
						B8(11110000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(11100000), //4
						B8(00010000), //5
						B8(00010000), //6
						B8(11100000), //7
						4
				},
				//6
				{
						B8(01100000), //1
						B8(10010000), //2
						B8(10000000), //3
						B8(11100000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//7
				{
						B8(11110000), //1
						B8(00010000), //2
						B8(00010000), //3
						B8(00100000), //4
						B8(01000000), //5
						B8(01000000), //6
						B8(01000000), //7
						4
				},
				//8
				{
						B8(01100000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(01100000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//9
				{
						B8(01100000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(01110000), //4
						B8(00010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//:
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(10000000), //3
						B8(00000000), //4
						B8(10000000), //5
						B8(00000000), //6
						B8(00000000), //7
						1
				},
				//;
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(01000000), //3
						B8(00000000), //4
						B8(01000000), //5
						B8(10000000), //6
						B8(00000000), //7
						2
				},
				//<
				{
						B8(00010000), //1
						B8(00100000), //2
						B8(01000000), //3
						B8(10000000), //4
						B8(01000000), //5
						B8(00100000), //6
						B8(00010000), //7
						4
				},
				//=
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(11110000), //3
						B8(00000000), //4
						B8(11110000), //5
						B8(00000000), //6
						B8(00000000), //7
						4
				},
				//>
				{
						B8(10000000), //1
						B8(01000000), //2
						B8(00100000), //3
						B8(00010000), //4
						B8(00100000), //5
						B8(01000000), //6
						B8(10000000), //7
						4
				},
				//?
				{
						B8(01110000), //1
						B8(10001000), //2
						B8(00010000), //3
						B8(00100000), //4
						B8(00100000), //5
						B8(00000000), //6
						B8(00100000), //7
						5
				},
				//@
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(00000000), //7
						5
				},
				//A
				{
						B8(01100000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(11110000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(10010000), //7
						4
				},
				//B
				{
						B8(11100000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(11100000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(11100000), //7
						4
				},
				//C
				{
						B8(01100000), //1
						B8(10010000), //2
						B8(10000000), //3
						B8(10000000), //4
						B8(10000000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//D
				{
						B8(11100000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(10010000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(11100000), //7
						4
				},
				//E
				{
						B8(11110000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(11100000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(11110000), //7
						4
				},
				//F
				{
						B8(11110000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(11100000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(10000000), //7
						4
				},
				//G
				{
						B8(01100000), //1
						B8(10010000), //2
						B8(10000000), //3
						B8(10110000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//H
				{
						B8(10010000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(11110000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(10010000), //7
						4
				},
				//I
				{
						B8(11100000), //1
						B8(01000000), //2
						B8(01000000), //3
						B8(01000000), //4
						B8(01000000), //5
						B8(01000000), //6
						B8(11100000), //7
						3
				},

				//J
				{
						B8(00111000), //1
						B8(00001000), //2
						B8(00001000), //3
						B8(00001000), //4
						B8(00001000), //5
						B8(10001000), //6
						B8(01110000), //7
						5
				},
				//K
				{
						B8(10001000), //1
						B8(10010000), //2
						B8(10100000), //3
						B8(11000000), //4
						B8(10100000), //5
						B8(10010000), //6
						B8(10001000), //7
						5
				},
				//L
				{
						B8(10000000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(10000000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(11110000), //7
						4
				},
				//M
				{
						B8(10001000), //1
						B8(11011000), //2
						B8(10101000), //3
						B8(10001000), //4
						B8(10001000), //5
						B8(10001000), //6
						B8(10001000), //7
						5
				},
				//N
				{
						B8(10001000), //1
						B8(11001000), //2
						B8(10101000), //3
						B8(10011000), //4
						B8(10001000), //5
						B8(10001000), //6
						B8(10001000), //7
						5
				},
				//O
				{
						B8(01100000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(10010000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//P
				{
						B8(11100000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(11100000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(10000000), //7
						4
				},
				//Q
				{
						B8(01110000), //1
						B8(10001000), //2
						B8(10001000), //3
						B8(10001000), //4
						B8(10101000), //5
						B8(10010000), //6
						B8(01101000), //7
						5
				},
				//R
				{
						B8(11110000), //1
						B8(10001000), //2
						B8(10001000), //3
						B8(11110000), //4
						B8(10100000), //5
						B8(10010000), //6
						B8(10001000), //7
						5
				},
				//S
				{
						B8(01100000), //1
						B8(10010000), //2
						B8(10000000), //3
						B8(01100000), //4
						B8(00010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//T
				{
						B8(11111000), //1
						B8(00100000), //2
						B8(00100000), //3
						B8(00100000), //4
						B8(00100000), //5
						B8(00100000), //6
						B8(00100000), //7
						5
				},
				//U
				{
						B8(10010000), //1
						B8(10010000), //2
						B8(10010000), //3
						B8(10010000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//V
				{
						B8(10001000), //1
						B8(10001000), //2
						B8(10001000), //3
						B8(10001000), //4
						B8(10001000), //5
						B8(01010000), //6
						B8(00100000), //7
						5
				},
				//W
				{
						B8(10001000), //1
						B8(10001000), //2
						B8(10001000), //3
						B8(10001000), //4
						B8(10101000), //5
						B8(11011000), //6
						B8(10001000), //7
						5
				},
				//X
				{
						B8(10001000), //1
						B8(10001000), //2
						B8(01010000), //3
						B8(00100000), //4
						B8(01010000), //5
						B8(10001000), //6
						B8(10001000), //7
						5
				},
				//Y
				{
						B8(10001000), //1
						B8(10001000), //2
						B8(01010000), //3
						B8(00100000), //4
						B8(00100000), //5
						B8(00100000), //6
						B8(00100000), //7
						5
				},
				//Z
				{
						B8(11111000), //1
						B8(00001000), //2
						B8(00010000), //3
						B8(00100000), //4
						B8(01000000), //5
						B8(10000000), //6
						B8(11111000), //7
						5
				},
				//[
				{
						B8(11100000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(10000000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(11100000), //7
						3
				},
				// \ j
				{
						B8(10000000), //1
						B8(10000000), //2
						B8(01000000), //3
						B8(00100000), //4
						B8(00010000), //5
						B8(00001000), //6
						B8(00001000), //7
						5
				},
				//]
				{
						B8(11100000), //1
						B8(00100000), //2
						B8(00100000), //3
						B8(00100000), //4
						B8(00100000), //5
						B8(00100000), //6
						B8(11100000), //7
						5
				},
				//^
				{
						B8(00100000), //1
						B8(01010000), //2
						B8(10001000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(00000000), //7
						5
				},
				//_
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(11111000), //7
						5
				},
				//`
				{
						B8(10000000), //1
						B8(01000000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(00000000), //7
						2
				},
				//a
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(01100000), //3
						B8(00010000), //4
						B8(01110000), //5
						B8(10010000), //6
						B8(01110000), //7
						4
				},
				//b
				{
						B8(10000000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(11100000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(11110000), //7
						4
				},
				//c
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(01100000), //4
						B8(10010000), //3
						B8(10000000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//d
				{
						B8(00010000), //1
						B8(00010000), //2
						B8(00010000), //4
						B8(01110000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(01110000), //7
						4
				},
				//e
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(01100000), //3
						B8(10010000), //4
						B8(11100000), //5
						B8(10000000), //6
						B8(01100000), //7
						4
				},
				//f
				{
						B8(00100000), //1
						B8(01010000), //2
						B8(01000000), //3
						B8(11110000), //4
						B8(01000000), //5
						B8(01000000), //6
						B8(01000000), //7
						4
				},
				//g
				{
						B8(00000000), //2
						B8(01100000), //3
						B8(10010000), //4
						B8(01110000), //5
						B8(00010000), //6
						B8(10010000), //7
						B8(01100000), //8
						4
				},
				//h
				{
						B8(10000000), //2
						B8(10000000), //3
						B8(10000000), //4
						B8(11100000), //5
						B8(10010000), //6
						B8(10010000), //7
						B8(10010000), //7
						4
				},
				//i
				{
						B8(00000000), //1
						B8(10000000), //2
						B8(00000000), //3
						B8(10000000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(10000000), //7
						1
				},
				//j
				{
						B8(00000000), //1
						B8(00100000), //2
						B8(00000000), //3
						B8(00100000), //4
						B8(00100000), //5
						B8(10100000), //6
						B8(01000000), //7
						3
				},
				//k
				{
						B8(10000000), //1
						B8(10100000), //2
						B8(10100000), //3
						B8(11000000), //4
						B8(10100000), //5
						B8(10100000), //6
						B8(10100000), //7
						3
				},
				//l
				{
						B8(10000000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(10000000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(01000000), //7
						2
				},
				//m
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(01010000), //3
						B8(10101000), //4
						B8(10001000), //5
						B8(10001000), //6
						B8(10001000), //7
						5
				},
				//n
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(11100000), //3
						B8(10010000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(10010000), //7
						4
				},
				//o
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(01100000), //3
						B8(10010000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//p
				{
						B8(00000000), //2
						B8(00000000), //2
						B8(11100000), //3
						B8(10010000), //4
						B8(11100000), //7
						B8(10000000), //8
						B8(10000000), //1
						4
				},
				//q
				{
						B8(00000000), //2
						B8(00000000), //2
						B8(01100000), //3
						B8(10010000), //4
						B8(01110000), //7
						B8(00010000), //8
						B8(00010000), //1
						4
				},
				//r
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(11100000), //3
						B8(10010000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(10000000), //7
						4
				},
				//s
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(01110000), //3
						B8(10000000), //4
						B8(01110000), //5
						B8(00001000), //6
						B8(11110000), //7
						5
				},
				//t
				{
						B8(01000000), //1
						B8(01000000), //2
						B8(11100000), //3
						B8(01000000), //4
						B8(01000000), //5
						B8(01000000), //6
						B8(00100000), //7
						3
				},
				//u
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(10010000), //3
						B8(10010000), //4
						B8(10010000), //5
						B8(10010000), //6
						B8(01100000), //7
						4
				},
				//v
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(10001000), //3
						B8(10001000), //4
						B8(10001000), //5
						B8(01010000), //6
						B8(00100000), //7
						5
				},
				//w
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(10001000), //3
						B8(10001000), //4
						B8(10001000), //5
						B8(10101000), //6
						B8(01010000), //7
						5
				},
				//x
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(10100000), //3
						B8(10100000), //4
						B8(01000000), //5
						B8(10100000), //6
						B8(10100000), //7
						3
				},
				//y
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(10010000), //3
						B8(10010000), //4
						B8(01110000), //5
						B8(00010000), //6
						B8(01100000), //7
						4
				},
				//z
				{
						B8(00000000), //1
						B8(00000000), //2
						B8(11111000), //3
						B8(00010000), //4
						B8(00100000), //5
						B8(01000000), //6
						B8(11111000), //7
						5
				},
				//{
				{
						B8(01100000), //1
						B8(01000000), //2
						B8(01000000), //3
						B8(10000000), //4
						B8(01000000), //5
						B8(01000000), //6
						B8(01100000), //7
						3
				},
				//|
				{
						B8(10000000), //1
						B8(10000000), //2
						B8(10000000), //3
						B8(10000000), //4
						B8(10000000), //5
						B8(10000000), //6
						B8(10000000), //7
						1
				},
				//}
				{
						B8(11000000), //1
						B8(01000000), //2
						B8(01000000), //3
						B8(00100000), //4
						B8(01000000), //5
						B8(01000000), //6
						B8(11000000), //7
						3
				},
				//~
				{
						B8(11000100), //1
						B8(10111000), //2
						B8(00000000), //3
						B8(00000000), //4
						B8(00000000), //5
						B8(00000000), //6
						B8(00000000), //7
						5
				}
		}

};

#endif
