#include "stdafx.h"
#include "TrackerCore.h"

#define GETRED(x) (x>>16)&0xFF
#define GETGREEN(x) (x>>8)&0xFF
#define GETBLUE(x) (x)&0xFF

TrackerCore::TrackerCore()
{
	colorMask = NULL;
	trackingColorOne.hueRangeHigh = trackingColorOne.hueRangeLow = 0;
	trackingColorOne.satRangeHigh = trackingColorOne.satRangeLow = 0;
	trackingColorOne.lumRangeHigh = trackingColorOne.lumRangeLow = 0;
	// Generate a default color mask
	this->generateColorMask();
	return;
}

//TrackerCore::TrackerCore(int hueRangeHigh, int hueRangeLow, int satRangeHigh,
//						 int satRangeLow, int lumRangeHigh, int lumRangeLow) 
//						 : trackingColorOne.hueRangeHigh(hueRangeHigh), trackingColorOne.hueRangeLow(hueRangeLow),
//						 trackingColorOne.satRangeHigh(satRangeHigh), trackingColorOne.satRangeLow(satRangeLow),
//						 trackingColorOne.lumRangeHigh(lumRangeHigh), trackingColorOne.lumRangeLow(lumRangeLow)
//{
//	colorMask = NULL;
//	// Generate a color mask for the new ranges
//	this->generateColorMask();
//	return;
//}

TrackerCore::~TrackerCore()
{ 
	if( colorMask == NULL )
		delete colorMask;
	return;
}

// Pre-calculates a lookup table
// We can then use RGB values as an index into
// the array containing whether the color is in range
void TrackerCore::generateColorMask(void)
{
	if( colorMask == NULL ) // colorMask not allocated yet
	{
		try
		{
			colorMask = new int[NUM_COLOR_VALUES * NUM_COLOR_VALUES * NUM_COLOR_VALUES];
		}
		catch( std::bad_alloc& ba )
		{
			std::cerr << "Failed to allocate memory for color mask: " << ba.what() << std::endl;
			return;
		}
	}
	// For every RGB value we find which are in the selected range
	for (int i = 0; i < NUM_COLOR_VALUES * NUM_COLOR_VALUES * NUM_COLOR_VALUES; i++)
		colorMask[i] = colorTest(GETRED(i), GETGREEN(i), GETBLUE(i));
	return;
}

void TrackerCore::findTarget( void* imageData, int pitch, int size )
{
	int runningXTotal = 0;
	int runningYTotal = 0;
	int totalPoints = 0;

	if( colorMask == NULL )
	{
		std::cerr << "findTarget called without a colorMask" << std::endl;
		return;
	}

	// size and pitch are in bytes not pixels, so we change them to pixels with 4 bpp
	pitch /= sizeof(UINT32);
	size /= sizeof(UINT32);

	// For every pixel we check its value in the lookup table
	// if it is there we sum the pixels position
	for( int i = 0; i < size; i++ )
	{
		if( colorMask[ ((UINT32*)imageData)[i] ] )
		{
			runningXTotal += i % pitch;
			runningYTotal += i / pitch;
			totalPoints++;
			((UINT8*)imageData)[(i*4)+2] = 0xFF;
		}
	}

	// then find the average of the tagets pixels, this
	// could be done better but it needs to be fast and
	// this blob detector works ok for us
	if( totalPoints )
	{
		this->centerOne.x = runningXTotal / totalPoints;
		this->centerOne.x = runningYTotal / totalPoints;
	}

	return;
}

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// #FIXME speedup and case for low larger than high range value
// Return which range the RGB value is in.
int TrackerCore::colorTest(int red, int green, int blue)
{
	float r = (float)red / 255;
	float g = (float)green / 255;
	float b = (float)blue / 255;

	float rgb_max = MAX(r, MAX(g, b));
    float rgb_min = MIN(r, MIN(g, b));
    float delta = rgb_max - rgb_min;
    float s = delta / (rgb_max + 1e-20f);
    float v = rgb_max;

    float hue;
    if (r == rgb_max)
        hue = (g - b) / (delta + 1e-20f);
    else if (g == rgb_max)
        hue = 2 + (b - r) / (delta + 1e-20f);
    else
        hue = 4 + (r - g) / (delta + 1e-20f);
    if (hue < 0)
        hue += 6.f;
    float h = hue * (1.f / 6.f);

	if( s > 0.4 )
		if( v > 0.5 )
			if( h * 360 > 25 && h * 360 < 45 )
				return 1;
	return 0;
}