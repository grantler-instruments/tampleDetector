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
    midiHueArray[0] = Tample("red", 1, 5.0f, 8.0f, 0.75, 0.1, 0.67, 0.1);
    midiHueArray[1] = Tample("purple", 2, 350.0f, 5.0f, 0.35, 0.1, 0.44, 0.1);
    midiHueArray[2] = Tample("pink", 3, 358.0f, 8.0f, 0.48, 0.1, 0.5, 0.1);
    midiHueArray[3] = Tample("orange", 4, 42.0f, 8.0f, 0.75, 0.1, 0.48, 0.1);
    midiHueArray[4] = Tample("yellow", 5, 64.0f, 8.0f, 0.63, 0.1, 0.42, 0.1);
    midiHueArray[5] = Tample("green", 6, 100.0f, 8.0f, 0.6, 0.1, 0.48, 0.1);
    midiHueArray[6] = Tample("lightgreen", 7, 92.0f, 8.0f, 0.42, 0.1, 0.42, 0.1);
    midiHueArray[7] = Tample("blue", 8, 210.0f, 8.0f, 0.53, 0.1, 0.44, 0.1);
    midiHueArray[8] = Tample("turquoise", 9, 155.0f, 8.0f, 0.45, 0.1, 0.42, 0.1);
    midiHueArray[9] = Tample("lightblue", 10, 125.0f, 8.0f, 0.33, 0.1, 0.41, 0.1);
    midiHueArray[10] = Tample("white", 11, 82.0f, 2.0f, 0.32, 0.01, 0.38, 0.1);      // TODO
    midiHueArray[11] = Tample("lightgray", 12, 76.0f, 3.0f, 0.28, 0.05, 0.38, 0.1);
    midiHueArray[12] = Tample("darkgray", 13, 85.0f, 5.0f, 0.25, 0.1, 0.38, 0.1); //TODO: this color cannot be recognized
    midiHueArray[13] = Tample("olivegray", 14, 68.0f, 6.0f, 0.42, 0.1, 0.38, 0.1);
    midiHueArray[14] = Tample("beige", 15, 54.0f, 8.0f, 0.48, 0.1, 0.38, 0.1);
    midiHueArray[15] = Tample("brown", 16, 30.0f, 8.0f, 0.55, 0.1, 0.46, 0.1);

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