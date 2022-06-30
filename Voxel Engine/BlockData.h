#pragma once
#include <stdint.h>

#pragma packed
struct Block
{
	uint8_t back_face;
	uint8_t front_face;
	uint8_t left_face;
	uint8_t right_face;
	uint8_t bottom_face;
	uint8_t top_face;

	uint8_t ID;
};