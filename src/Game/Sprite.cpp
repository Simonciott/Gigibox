#include "Sprite.hpp"

using Gigi::Sprite;

using sf::Vector2u;

Sprite::Sprite(Image& img) {
	Sprite::texture = &img;
}

void Sprite::Draw() {
	// clamping posizione per prevenire errori out of bounds
	if (*xHook + texture->width >= SCREEN_WIDTH)
		*xHook = SCREEN_WIDTH - texture->width;
	if (*yHook + texture->height >= SCREEN_HEIGHT)
		*yHook = SCREEN_HEIGHT - texture->height;
	if (*xHook < 0)
		*xHook = 0;
	if (*yHook < 0)
		*yHook = 0;

	if (!visibleHook) return;

	for (int i = 0; i < texture->height; i++) {
		for (int j = 0; j < texture->width; j++) {
			int pxindex = texture->pixels[texture->width * i + j];
			if (pxindex >= texture->palette.size()) pxindex = 0;
			Color color = texture->palette[pxindex];

			if (texture->alpha && color == texture->alphaColor) continue;

			BuffersManager::getBackBuffer()->setPixel(Vector2u(j + (unsigned int)xHook, i + (unsigned int)yHook), color);
		}
	}
}