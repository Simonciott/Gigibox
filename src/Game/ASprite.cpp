#include "ASprite.hpp"
#include "ATexture.hpp"

#include <../Assembly/Assembly.hpp>

using Gigi::Assembly::Sprite, Gigi::Assembly::Registers;

using sf::Vector2u;

Sprite::Sprite(short* address, short imgaddress) {
	spriteIndex = address;
	*spriteIndex = imgaddress;
	*(spriteIndex + 1) = -1;
}

Sprite::Sprite(short* address, short imgaddress, short colorsaddress) {
	spriteIndex = address;
	*spriteIndex = imgaddress;
	*(spriteIndex + 1) = colorsaddress;
}

void Sprite::Draw(vector<Assembly::Image>& images) {
	if (!*(spriteIndex + 4)) return;

	Gigi::Assembly::Image& textureRef = images[*spriteIndex];
	short& x = spriteIndex[2];
	short& y = spriteIndex[3];

	short* paletteindex = Registers::getData(spriteIndex[1]);
	int numcolors = floor(*paletteindex * 2 / 3);
	vector<Color> colors;

	// ho fatto uno screen di paint.net per spiegare che fa sto pezzone colossale
	const int lefthalf16b = 0b1111111100000000;
	int whichHalf = 0;
	for (int i = 1; i <= *paletteindex; i++) {
		uint8_t rgb[3];
		for (int j = 0; j < 3; j++, whichHalf++) {
			if (whichHalf >= 2) { // io sono lunatico e pratico dove cazzo mi pare
				i++;
				whichHalf = 0;
			}
			rgb[j] = whichHalf % 2 == 0 ? (paletteindex[i + 1] | lefthalf16b ^ lefthalf16b) : paletteindex[i + 1] >> 8;
		}
		colors.push_back(Color(rgb[0], rgb[1], rgb[2]));
	}

	for (int i = 0; i < textureRef.index[2]; i++) {
		for (int j = 0; j < textureRef.index[1]; j++) {
			if (x + j >= SCREEN_WIDTH || x + j < 0 || y + i >= SCREEN_HEIGHT || y + i < 0) continue;

			int pxindex = textureRef.pixels[textureRef.index[1] * i + j];
			if (pxindex >= textureRef.index[3]) pxindex = 0;
			Color color = colors[pxindex];

			if (textureRef.index[4] && color == colors[textureRef.index[5]]) continue;

			BuffersManager::getBackBuffer()->setPixel(Vector2u(j + x, i + y), color);
		}
	}
}