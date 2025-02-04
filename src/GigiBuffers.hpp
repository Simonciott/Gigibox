#pragma once

#include <SFML/Graphics.hpp>

#include "GigiConstants.hpp"

using sf::Image;
using sf::Texture;
using sf::Color;


// i buffer per disegnare pixel per pixel
// i buffer vengono scambiati avanti e indietro. quello dietro viene processato mentre quello davanti viene mostrato a schermo
extern Image Gigi_Buffers[2];

// texture che cambia fra un buffer e l'altro
struct Gigi_BackBufferTexture {
	static bool firstBuffer;

	static Texture texture;

	static Image* getBuffer(bool firstb = firstBuffer);
	static Image* getBackBuffer(bool firstb = firstBuffer);

	static void updateTexture();
	static void swapBuffers();

	static void clearBackBuffer(Color color);
};