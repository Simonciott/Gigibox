#pragma once

#include <SFML/Graphics.hpp>

using sf::Keyboard::Scan;

namespace Gigi::Input {
	struct Key {
		Scan mainKey;
		Scan alternativeKey;

		Key(Scan mk, Scan ak);

		bool isEitherDown();

		static Key Up;
		static Key Left;
		static Key Down;
		static Key Right;

		static Key Zed;
		static Key Ex;
		static Key Cee;
	};
}