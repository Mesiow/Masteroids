#include "Util.h"
#include "../Net/Constants.h"

namespace Util {

	void wrapCoords(float ix, float iy, float& ox, float& oy)
	{
		ox = ix;
		oy = iy;
		if (ix < 0.0f) ox = ix + (float)SCREEN_WIDTH;
		if (ix >= (float)SCREEN_WIDTH) ox = ix - (float)SCREEN_WIDTH;
		if (iy < 0.0f) oy = iy + (float)SCREEN_HEIGHT;
		if (iy >= (float)SCREEN_HEIGHT) oy = iy - (float)SCREEN_HEIGHT;
	}
}
