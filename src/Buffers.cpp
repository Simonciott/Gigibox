#include "Constants.hpp"

#include "Buffers.hpp"

using sf::Color;
using sf::Vector2u;

// i buffer per disegnare pixel per pixel
// i buffer vengono scambiati avanti e indietro. quello dietro viene processato mentre quello davanti viene mostrato a schermo
sf::Image Gigi::Buffers[2] = { sf::Image({ SCREEN_WIDTH, SCREEN_HEIGHT }, Color::Black), Image({ SCREEN_WIDTH, SCREEN_HEIGHT }, Color::Black) };

// texture che cambia fra un buffer e l'altro

bool Gigi::BuffersManager::firstBuffer;

Texture Gigi::BuffersManager::texture = Texture(Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT));

sf::Image* Gigi::BuffersManager::getBuffer(bool firstb) {
	return &Buffers[(int)firstb];
}

sf::Image* Gigi::BuffersManager::getBackBuffer(bool firstb) {
	return &Buffers[(int)!firstb];
}

void Gigi::BuffersManager::updateTexture() {
	texture.update(*getBuffer(firstBuffer));
}
void Gigi::BuffersManager::swapBuffers() {
	firstBuffer = !firstBuffer;
	updateTexture();
}

void Gigi::BuffersManager::clearBackBuffer(Color color) {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			BuffersManager::getBackBuffer()->setPixel(Vector2u(x, y), color);
		}
	}
}