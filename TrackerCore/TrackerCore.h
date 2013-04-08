#ifdef TRACKERCORE_EXPORTS
#define TRACKERCORE_API __declspec(dllexport)
#else
#define TRACKERCORE_API __declspec(dllimport)
#endif

#define NUM_COLOR_VALUES 256

typedef struct
{
	// Hue ranges from 0 to 360
	int hueRangeHigh;
	int hueRangeLow;
	// Saturation ranges from 0 to 100
	int satRangeHigh;
	int satRangeLow;
	// luminosity ranges from 0 to 100
	int lumRangeHigh;
	int lumRangeLow;
} HSVColorRange;

typedef struct
{
	int x, y;
} Coordinate;

class TRACKERCORE_API TrackerCore
{
public:
	// The two colors we will be tracking
	HSVColorRange trackingColorOne;
	HSVColorRange trackingColorTwo;

	// Pointer to the lookup table
	int *colorMask;

	// Center of targets
	Coordinate centerOne;
	Coordinate centerTwo;

	TrackerCore(void);
	TrackerCore(int hueRangeHigh, int hueRangeLow, int satRangeHigh,
				int satRangeLow, int lumRangeHigh, int lumRangeLow);
	~TrackerCore(void);
	
	void generateColorMask(void);
	// For speed and simplicity we assume 4 byte pixels in ARGB format
	void findTarget( void* imageData, int pitch, int size );

private:
	int colorTest(int red, int green, int blue);
};