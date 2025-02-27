#include "Sprite.hpp"

using Gigi::Sprite;

using sf::Vector2u;

Sprite::Sprite(Image& img, vector<Color> colors) {
	Sprite::texture = &img;
	for (int i = 0; i < img.paletteSize; i++) {
		palette.push_back(colors[i]);
	}
}

void Sprite::Draw() {
	if (!visibleHook) return;

	for (int i = 0; i < texture->height; i++) {
		for (int j = 0; j < texture->width; j++) {
			if (*xHook + j >= SCREEN_WIDTH || *xHook + j < 0 || *yHook + i >= SCREEN_HEIGHT || *yHook + i < 0) continue;

			int pxindex = texture->pixels[texture->width * i + j];
			if (pxindex >= palette.size()) pxindex = 0;
			Color color = palette[pxindex];

			if (texture->alpha && color == palette[texture->alphaColor]) continue;

			BuffersManager::getBackBuffer()->setPixel(Vector2u(j + (unsigned int)xHook, i + (unsigned int)yHook), color);
			// ^-- non so come stia funzionando con i pointer non dereferenziati ma meglio non toccare
			// if it ain't broke don't fix it
		}
	}
}