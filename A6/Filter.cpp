#include "stdafx.h"
#include "Filter.h"


Filter::Filter ()
{
}


Filter::~Filter ()
{
}
Vec3f Filter::getColor (int i, int j, Film *film) {
	Vec3f color;
	float sumWeight = 0.f;
	int supportRadius = getSupportRadius ();
	for (int xOffset = -supportRadius; xOffset <= supportRadius; ++xOffset) {
		for (int yOffset = -supportRadius; yOffset <= supportRadius; ++yOffset) {
			int x = xOffset + i, y = yOffset + j;
			if (x < 0 || x >= film->getWidth () || y < 0 || y >= film->getHeight ())
				continue; 
			for (int n = 0; n < film->getNumSamples (); ++n) {
				Sample sample = film->getSample (x, y, n);
				Vec2f samplePos = sample.getPosition ();
				Vec2f pixelPos = Vec2f (samplePos.x () + xOffset - 0.5f, samplePos.y () + yOffset - 0.5f);
				float weight = getWeight (pixelPos.x (), pixelPos.y ());
				sumWeight += weight;
				color += sample.getColor () * weight;
			}
		}
	}
	float normalizeFactor = 1.f / sumWeight;
	return color * normalizeFactor;
}
