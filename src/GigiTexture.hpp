#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

using std::cout;
using std::endl;

using std::vector;

using sf::Color;

struct Gigi_Image {

	uint8_t width = 1;
	uint8_t height = 1;

	vector<Color> palette;

	bool alpha;
	Color alphaColor = Color::Green;

	uint8_t* pixels;
	size_t pixelsSize;


	Gigi_Image(uint8_t* data8b, size_t size = 0);
};