#pragma once

#include <SFML/Graphics.hpp>

using sf::Keyboard::Scan;

namespace Gigi::Input {
	struct Key {
		Scan mainKey;
		Scan alternativeKey;

		Key(Scan mk, Scan ak);

		bool isEitherDown();
	};

	Key Up = Key(Scan::W, Scan::Up);
	Key Left = Key(Scan::A, Scan::Left);
	Key Down = Key(Scan::S, Scan::Down);
	Key Right = Key(Scan::D, Scan::Right);

	Key Zed = Key(Scan::I, Scan::Z);
	Key Ex = Key(Scan::O, Scan::X);
	Key Cee = Key(Scan::P, Scan::C);
}