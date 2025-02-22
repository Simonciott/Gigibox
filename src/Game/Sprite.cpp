#include "Sprite.hpp"

using Gigi::Sprite;

using sf::Vector2u;

Sprite::Sprite(Image& img) {
	Sprite::texture = &img;
}

void Sprite::Draw() {
	// clamping posizione per prevenire errori out of bounds

	if (!visibleHook) return;

	for (int i = 0; i < texture->height; i++) {
		for (int j = 0; j < texture->width; j++) {
			if (*xHook + j >= SCREEN_WIDTH || *xHook + j < 0 || *yHook + i >= SCREEN_HEIGHT || *yHook + i < 0) continue;

			int pxindex = texture->pixels[texture->width * i + j];
			if (pxindex >= texture->palette.size()) pxindex = 0;
			Color color = texture->palette[pxindex];

			if (texture->alpha && color == texture->alphaColor) continue;

			BuffersManager::getBackBuffer()->setPixel(Vector2u(j + (unsigned int)xHook, i + (unsigned int)yHook), color);
		}
	}
}