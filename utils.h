#define NUMBER_OF_TAMPLES 16
#define HISTORY_BUFFER_SIZE 24

#include "String.h"
struct Tample
{
    int _note;        // MIDI CC number for playback (0-127)
    int _recordCC;    // MIDI CC number for recording (0-127)
    float _hue;       // Minimum hue (0.0 - 360.0)
    float _tolerance; // Maximum hue (0.0 - 360.0)
    float _saturation;
    float _saturationTolerance;
    float _brightness;          // 0-1
    float _brightnessTolerance; // 0-1
    String _name;

    Tample(){}
    Tample(bool invalid) : _name("invalid"), _hue(-1), _saturation(-1), _brightness(-1) {}

    // Constructor for initializing values
    Tample(String name, int note, float hue, float tolerance, float saturation, float saturationTolerance, float brightness, float brightnessTolerance)
        : _name(name), _note(note), _recordCC(note), _hue(hue), _tolerance(tolerance), _saturation(saturation), _saturationTolerance(saturationTolerance), _brightness(brightness), _brightnessTolerance(brightnessTolerance) {}
};

inline Tample *getTamples()
{
    static Tample midiHueArray[NUMBER_OF_TAMPLES]; // Static to retain data after the function call
    midiHueArray[0] = Tample("red", 1, 5.0f, 5.0f, 0.75, 0.1, 0.67, 0.1);
    midiHueArray[1] = Tample("purple", 2, 354.0f, 5.0f, 0.50, 0.1, 0.50, 0.1);
    midiHueArray[2] = Tample("pink", 3, 357.0f, 8.0f, 0.64, 0.1, 0.57, 0.1);
    midiHueArray[3] = Tample("orange", 4, 30.0f, 8.0f, 0.75, 0.1, 0.52, 0.1);
    midiHueArray[4] = Tample("yellow", 5, 40.0f, 5.0f, 0.71, 0.1, 0.46, 0.1);
    midiHueArray[5] = Tample("green", 6, 85.0f, 8.0f, 0.6, 0.1, 0.44, 0.1);
    midiHueArray[6] = Tample("lightgreen", 7, 62.0f, 8.0f, 0.4, 0.1, 0.4, 0.1);
    midiHueArray[7] = Tample("blue", 8, 210.0f, 8.0f, 0.37, 0.1, 0.39, 0.1);
    midiHueArray[8] = Tample("turquoise", 9, 135.0f, 8.0f, 0.3, 0.1, 0.42, 0.1);
    midiHueArray[9] = Tample("lightblue", 10, 88.0f, 5.0f, 0.33, 0.1, 0.41, 0.1);

    midiHueArray[10] = Tample("white", 11, 40, 5, 0.4, 0.05, 0.4, 0.1);
    midiHueArray[11] = Tample("lightgray", 12, 39.0f, 3.0f, 0.4, 0.05, 0.38, 0.1);
    midiHueArray[11] = Tample("mossgreen", 11, 80, 5, 0.38f, 0.1f, 0.41f, 0.1f);
    midiHueArray[12] = Tample("orangebrown", 12, 15.0f, 4.0f, 0.75, 0.1, 0.58, 0.1);
    midiHueArray[13] = Tample("olivegray", 14, 40.0f, 5.0f, 0.5, 0.05, 0.4, 0.05);
    midiHueArray[14] = Tample("beige", 15, 32.0f, 5.0f, 0.6, 0.1, 0.45, 0.1);
    midiHueArray[15] = Tample("brown", 16, 20.0f, 5.0f, 0.6, 0.1, 0.5, 0.1);

    return midiHueArray; // Return the array
}

struct HSV
{
    float h; // Hue: 0-360 degrees
    float s; // Saturation: 0-1
    float v; // Value (brightness): 0-1
};
HSV rgbToHsv(int r, int g, int b)
{
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;

    float maxVal = std::max({rf, gf, bf});
    float minVal = std::min({rf, gf, bf});
    float delta = maxVal - minVal;

    HSV hsv;

    if (delta == 0)
    {
        hsv.h = 0; // Undefined hue
    }
    else if (maxVal == rf)
    {
        hsv.h = 60.0f * (fmod(((gf - bf) / delta), 6));
    }
    else if (maxVal == gf)
    {
        hsv.h = 60.0f * (((bf - rf) / delta) + 2);
    }
    else
    {
        hsv.h = 60.0f * (((rf - gf) / delta) + 4);
    }
    if (hsv.h < 0)
    {
        hsv.h += 360.0f;
    }
    hsv.s = (maxVal == 0) ? 0 : (delta / maxVal);
    hsv.v = maxVal;
    return hsv;
}

struct historyEntry
{
    HSV _hsv;
    unsigned long _timestamp;
};