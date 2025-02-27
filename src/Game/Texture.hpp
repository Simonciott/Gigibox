#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

using std::cout;
using std::endl;

using std::vector;

using sf::Color;

namespace Gigi {

	struct Image {

		uint8_t width = 1;
		uint8_t height = 1;

		int paletteSize;

		bool alpha;
		int alphaColor;

		uint8_t* pixels;
		size_t pixelsSize;

		static Image Smiley;


		Image(uint8_t* data8b, size_t size = 0);

	private:
		void processImageData(uint8_t* data);
	};
}