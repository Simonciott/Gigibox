#include "GigiConstants.hpp"

#include "GigiBuffers.hpp"

using sf::Color;
using sf::Vector2u;

// i buffer per disegnare pixel per pixel
// i buffer vengono scambiati avanti e indietro. quello dietro viene processato mentre quello davanti viene mostrato a schermo
Image Gigi_Buffers[2] = { Image({ SCREEN_WIDTH, SCREEN_HEIGHT }, Color::Black), Image({ SCREEN_WIDTH, SCREEN_HEIGHT }, Color::Black) };

// texture che cambia fra un buffer e l'altro

bool Gigi_BackBufferTexture::firstBuffer;

Texture Gigi_BackBufferTexture::texture = Texture(Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT));

Image* Gigi_BackBufferTexture::getBuffer(bool firstb) {
	return &Gigi_Buffers[(int)firstb];
}

Image* Gigi_BackBufferTexture::getBackBuffer(bool firstb) {
	return &Gigi_Buffers[(int)!firstb];
}

void Gigi_BackBufferTexture::updateTexture() {
	texture.update(*getBuffer(firstBuffer));
}
void Gigi_BackBufferTexture::swapBuffers() {
	firstBuffer = !firstBuffer;
	updateTexture();
}

void Gigi_BackBufferTexture::clearBackBuffer(Color color) {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			Gigi_BackBufferTexture::getBackBuffer()->setPixel(Vector2u(x, y), color);
		}
	}
}