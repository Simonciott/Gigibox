#include "GigiTexture.hpp"


Gigi_Image::Gigi_Image(uint8_t* data8b, size_t size) {
	pixelsSize = size;

	width = data8b[0] + 1;
	height = data8b[1] + 1;

	for (int i = 0; i < data8b[2] + 1; i++) {
		palette.push_back(Color(data8b[3 * (i + 1)], data8b[3 * (i + 1) + 1], data8b[3 * (i + 1) + 2]));
	}

	int indexAfterPalette = 3 + 3 * (data8b[2] + 1) + 2;

	alpha = (bool)data8b[indexAfterPalette + 0];
	alphaColor = palette[data8b[indexAfterPalette + 1]];

	pixels = (uint8_t*)malloc(width * height);

	for (int i = indexAfterPalette; i < indexAfterPalette + width * height; i++) {
		pixels[i - indexAfterPalette] = data8b[i];
	}

}