#include "Input.hpp"

Gigi::Key::Key(Scan mk, Scan ak) {
	mainKey = mk;
	alternativeKey = ak;
}

bool Gigi::Key::isEitherDown() {
	return sf::Keyboard::isKeyPressed(mainKey) || sf::Keyboard::isKeyPressed(alternativeKey);
}