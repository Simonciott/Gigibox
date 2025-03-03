#include "Sprite.hpp"

using Gigi::Sprite;

using sf::Vector2u;

Sprite::Sprite(Image& img) {
	Sprite::texture = &img;
	for (int i = 0; i < img.paletteSize; i++) {
		uint8_t gray = (255 / img.paletteSize) * i;
		palette.push_back(Color(gray, gray, gray));
	}
}

Sprite::Sprite(Image& img, vector<Color> colors) {
	Sprite::texture = &img;
	for (int i = 0; i < img.paletteSize; i++) {
		palette.push_back(colors[i]);
	}
}

void Sprite::Draw() {
	if (!visible) return;

	for (int i = 0; i < texture->height; i++) {
		for (int j = 0; j < texture->width; j++) {
			if (x + j >= SCREEN_WIDTH || x + j < 0 || y + i >= SCREEN_HEIGHT || y + i < 0) continue;

			int pxindex = texture->pixels[texture->width * i + j];
			if (pxindex >= palette.size()) pxindex = 0;
			Color color = palette[pxindex];

			if (texture->alpha && color == palette[texture->alphaColor]) continue;

			BuffersManager::getBackBuffer()->setPixel(Vector2u(j + x, i + y), color);
		}
	}
}