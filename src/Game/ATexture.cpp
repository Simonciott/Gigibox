#include "ATexture.hpp"

#include <stdexcept>

using Gigi::Assembly::Image;

Gigi::Image Image::Smiley = Gigi::Image::Smiley;

Image::Image(short* address, uint8_t* data8b, size_t size) {
	pixelsSize = size;
	try {
		processImageData(data8b);
	}
	catch (const std::out_of_range& err) { // imposta texture default in caso di errore
		processImageData(Smiley);
	}
}
Image::Image(short* address, Gigi::Image img) {
	pixelsSize = img.pixelsSize;
	try {
		processImageData(img);
	}
	catch (const std::out_of_range& err) { // imposta texture default in caso di errore
		processImageData(Smiley);
	}
}

void Image::processImageData(uint8_t* data) {
	for(int i = 0; i < 5; i++){
		*(index + 1 + i) = data[i] + (int)(i < 3);
	}

	pixels = (uint8_t*)malloc(index[1] * index[2]);

	for (int i = 5; i < 5 + index[1] * index[2]; i++) {
		pixels[i - 5] = data[i];
	}
}

void Image::processImageData(Gigi::Image& img) {
	index[1] = img.width;
	index[2] = img.height;

	index[3] = img.paletteSize;

	index[4] = img.alpha;
	index[5] = img.alphaColor;

	pixels = (uint8_t*)malloc(img.width * img.height);

	for (int i = 5; i < 5 + img.width * img.height; i++) {
		pixels[i - 5] = img.pixels[i];
	}
}