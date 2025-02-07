#pragma once

#include "Texture.hpp"
#include "Buffers.hpp"
#include "Constants.hpp"

#include <SFML/Graphics.hpp>

namespace Gigi {
	struct Sprite {

		int x = 0;
		int y = 0;

		// gigibox exclusive hookup variables
		short* xHook = nullptr;
		short* yHook = nullptr;
		bool* visibleHook = nullptr;

		bool visible = true;

		Image* texture;

		Sprite(Image& img);

		void Draw(int x, int y);
	};
}