#pragma once

#include <SFML/Graphics.hpp>

#include "Constants.hpp"

using sf::Image;
using sf::Texture;
using sf::Color;

namespace Gigi {

	// i buffer per disegnare pixel per pixel
	// i buffer vengono scambiati avanti e indietro. quello dietro viene processato mentre quello davanti viene mostrato a schermo
	extern sf::Image Buffers[2];

	// texture che cambia fra un buffer e l'altro
	struct BuffersManager {
		static bool firstBuffer;

		static Texture texture;

		static sf::Image* getBuffer(bool firstb = firstBuffer);
		static sf::Image* getBackBuffer(bool firstb = firstBuffer);

		static void updateTexture();
		static void swapBuffers();

		static void clearBackBuffer(Color color);
	};
}