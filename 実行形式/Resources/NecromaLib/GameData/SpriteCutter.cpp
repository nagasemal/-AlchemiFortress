#include "pch.h"
#include "SpriteCutter.h"

int animationKey = 0;

RECT SpriteCutter(int wight, int height, int key[], int beside, int vertical, int interval)
{
	animationKey = (animationKey + 1) % (beside * interval);

	int pulsX = wight * key[animationKey / interval];

	RECT rect;
	rect.top	= 0 + (height * vertical);
	rect.bottom = height + (height * vertical);
	rect.left	= 0 + pulsX;
	rect.right	= wight + pulsX;

	return rect;
}

RECT SpriteCutter(int wight, int height, int beside, int vertical)
{
	RECT rect;
	rect.top	= 0 + (height * vertical);
	rect.bottom = height + (height * vertical);
	rect.left	= 0 + (wight * beside);
	rect.right	= wight + (wight * beside);

	return rect;
}
