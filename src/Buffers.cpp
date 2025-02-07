#include "Constants.hpp"

#include "Buffers.hpp"

using Gigi::BuffersManager;

using sf::Color;
using sf::Vector2u;

// i buffer per disegnare pixel per pixel
// i buffer vengono scambiati avanti e indietro. quello dietro viene processato mentre quello davanti viene mostrato a schermo
sf::Image Gigi_Buffers[2] = { sf::Image({ SCREEN_WIDTH, SCREEN_HEIGHT }, Color::Black), Image({ SCREEN_WIDTH, SCREEN_HEIGHT }, Color::Black) };

// texture che cambia fra un buffer e l'altro

bool BuffersManager::firstBuffer;

Texture BuffersManager::texture = Texture(Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT));

sf::Image* BuffersManager::getBuffer(bool firstb) {
	return &Buffers[(int)firstb];
}

sf::Image* BuffersManager::getBackBuffer(bool firstb) {
	return &Buffers[(int)!firstb];
}

void BuffersManager::updateTexture() {
	texture.update(*getBuffer(firstBuffer));
}
void BuffersManager::swapBuffers() {
	firstBuffer = !firstBuffer;
	updateTexture();
}

void BuffersManager::clearBackBuffer(Color color) {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			BuffersManager::getBackBuffer()->setPixel(Vector2u(x, y), color);
		}
	}
}