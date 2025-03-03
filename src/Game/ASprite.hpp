#pragma once

#include "Buffers.hpp"
#include "Constants.hpp"
#include "ATexture.hpp"


#include <SFML/Graphics.hpp>

namespace Gigi::Assembly {
	struct Sprite {

		short* spriteIndex;

		Sprite(short* address, short imgaddress);
		Sprite(short* address, short imgaddress, short paletteaddress);

		void Draw(vector<Assembly::Image>& images);
	};
}