#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Texture.hpp"

using std::cout;
using std::endl;

using std::vector;

using sf::Color;

namespace Gigi::Assembly {

	struct Image {

		short* index;

		uint8_t* pixels;
		size_t pixelsSize;

		static Gigi::Image Smiley;


		Image(short* address, uint8_t* data8b, size_t size = 0);

	private:
		void processImageData(uint8_t* data);
		void processImageData(Gigi::Image& img);
	};
}