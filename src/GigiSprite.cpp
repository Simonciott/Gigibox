#include "GigiSprite.hpp"

using sf::Vector2u;

Gigi_Sprite::Gigi_Sprite(Gigi_Image& img) {
	Gigi_Sprite::texture = &img;
}

void Gigi_Sprite::Draw(int x, int y) {
	// hooks
	if (*xHook + texture->width >= SCREEN_WIDTH)
		*xHook = SCREEN_WIDTH - texture->width;
	if (*yHook + texture->height >= SCREEN_HEIGHT)
		*yHook = SCREEN_HEIGHT - texture->height;
	if (*xHook < 0)
		*xHook = 0;
	if (*yHook < 0)
		*yHook = 0;

	x = *xHook;
	y = *yHook;
	visible = *visibleHook;

	if (!visible) return;

	if (!x) Gigi_Sprite::x = x;
	if (!y) Gigi_Sprite::y = y;

	for (int i = 0; i < texture->height; i++) {
		for (int j = 0; j < texture->width; j++) {
			//cout << "palette " << texture->pixels[texture->width * (i - 1) + j] << endl;
			int pxindex = texture->pixels[texture->width * i + j];
			if (pxindex >= texture->palette.size()) pxindex = 0;
			Color color = texture->palette[pxindex];

			if (texture->alpha && color == texture->alphaColor) continue;

			Gigi_BackBufferTexture::getBackBuffer()->setPixel(Vector2u(j + x, i + y), color);
		}
	}
}