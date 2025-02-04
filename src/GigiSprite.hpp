#pragma once

#include "GigiTexture.hpp"
#include "GigiBuffers.hpp"
#include "GigiConstants.hpp"

#include <SFML/Graphics.hpp>

struct Gigi_Sprite {

	int x = 0;
	int y = 0;

	// gigibox exclusive hookup variables
	short* xHook = nullptr;
	short* yHook = nullptr;
	bool* visibleHook = nullptr;

	bool visible = true;

	Gigi_Image* texture;

	Gigi_Sprite(Gigi_Image& img);

	void Draw(int x, int y);
};