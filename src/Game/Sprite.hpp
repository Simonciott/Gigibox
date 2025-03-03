#pragma once

#include "Texture.hpp"
#include "Buffers.hpp"
#include "Constants.hpp"

#include <SFML/Graphics.hpp>

namespace Gigi {
	struct Sprite {

		// x e y rimossi esclusivamente su assembly gigibox
		// altri progetti che potrebbero usare il motore grafico gigibox includeranno x e y (e rimuoveranno le variabili hook)

		// gigibox exclusive hookup variables
		// non c'ho voglia di cambiare il nome frate
		short x = 0;
		short y = 0;
		bool visible = true;

		// stessa cosa la prorietà visible

		Image* texture;
		vector<Color> palette;

		Sprite(Image& img);
		Sprite(Image& img, vector<Color> colors);

		virtual void Draw();
	};
}