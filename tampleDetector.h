#pragma once
#include "./utils.h"

class TampleDetector
{
public:
  historyEntry _history[24];
  int historyIndex = 0;
  
  void update()
  {
    // TODO: check if new tample is detected
  }
  
  void addEntryToHistory(int red, int green, int blue)
  {
    historyEntry entry;
    entry._hsv = rgbToHsv(red, green, blue);
    entry._timestamp = millis();
    _history[historyIndex] = entry;
    historyIndex++;
    if (historyIndex >= HISTORY_BUFFER_SIZE)
    {
      historyIndex = 0;
    }
  }
  
  // Enhanced method that checks hue, saturation, and brightness
  bool isColorMatch(const Tample& tample, int requiredCount = HISTORY_BUFFER_SIZE / 3)
  {
    int consecutiveCount = 0;
    // Start from the most recent entry and go backwards
    for (int i = historyIndex - 1, count = 0; count < HISTORY_BUFFER_SIZE; count++)
    {
      // Handle circular buffer wrap-around
      int idx = (i - count) % HISTORY_BUFFER_SIZE;
      if (idx < 0)
        idx += HISTORY_BUFFER_SIZE;
      
      // Check if the hue is within the tolerance range
      int hueDiff = abs(_history[idx]._hsv.h - tample._hue);
      bool hueMatch = (hueDiff <= tample._tolerance || hueDiff >= (360 - tample._tolerance));
      
      // Check if the saturation is within the tolerance range
      float saturationDiff = abs(_history[idx]._hsv.s - tample._saturation);
      bool saturationMatch = (saturationDiff <= tample._saturationTolerance);
      
      // Check if the brightness is within the tolerance range
      float brightnessDiff = abs(_history[idx]._hsv.v - tample._brightness);
      bool brightnessMatch = (brightnessDiff <= tample._brightnessTolerance);
      
      if (hueMatch && saturationMatch && brightnessMatch)
      {
        consecutiveCount++;
        if (consecutiveCount >= requiredCount)
        {
          return true;
        }
      }
      else
      {
        consecutiveCount = 0; // Reset if sequence is broken
      }
    }
    return false;
  }
  
  // Legacy method for backwards compatibility (hue only)
  bool isHue(int hue, int requiredCount = HISTORY_BUFFER_SIZE / 3, int tolerance = 5)
  {
    int consecutiveCount = 0;
    // Start from the most recent entry and go backwards
    for (int i = historyIndex - 1, count = 0; count < HISTORY_BUFFER_SIZE; count++)
    {
      // Handle circular buffer wrap-around
      int idx = (i - count) % HISTORY_BUFFER_SIZE;
      if (idx < 0)
        idx += HISTORY_BUFFER_SIZE;
      // Check if the hue is within the tolerance range
      int diff = abs(_history[idx]._hsv.h - hue);
      if (diff <= tolerance || diff >= (360 - tolerance))
      { // Handle hue wrap-around (0 and 360 are the same)
        consecutiveCount++;
        if (consecutiveCount >= requiredCount)
        {
          return true;
        }
      }
      else
      {
        consecutiveCount = 0; // Reset if sequence is broken
      }
    }
    return false;
  }
  
  // Updated getTample method that uses brightness
  Tample getTample()
  {
    auto tokens = getTamples();
    for (auto i = 0; i < NUMBER_OF_TAMPLES; i++)
    {
      if (isColorMatch(tokens[i], 8))
      {
        return tokens[i];
      }
    }
    return Tample(false);
  }
  
  // Alternative getTample that allows you to specify which method to use
  Tample getTokenWithOptions(bool useFullColorMatching = true, int requiredCount = 8)
  {
    auto tokens = getTamples();
    for (auto i = 0; i < NUMBER_OF_TAMPLES; i++)
    {
      if (useFullColorMatching)
      {
        if (isColorMatch(tokens[i], requiredCount))
        {
          return tokens[i];
        }
      }
      else
      {
        if (isHue(tokens[i]._hue, requiredCount, tokens[i]._tolerance))
        {
          return tokens[i];
        }
      }
    }
    return Tample(false);
  }
  
  HSV getCurrentHistoryEntry()
  {
    return _history[historyIndex]._hsv;  // Fixed syntax
  }
  
  void printHistory()
  {
    String message;
    for (auto i = 0; i < HISTORY_BUFFER_SIZE; i++)
    {
      message += _history[i]._hsv.h;  // Fixed syntax
      if (i != HISTORY_BUFFER_SIZE - 1)
      {
        message += ", ";
      }
    }
    Serial.println(message);
  }
  
  void printCurrentHistoryEntry()
  {
    Serial.print("hue: ");
    Serial.print(_history[historyIndex]._hsv.h);
    Serial.print(", saturation: ");
    Serial.print(_history[historyIndex]._hsv.s);
    Serial.print(", value: ");
    Serial.println(_history[historyIndex]._hsv.v);
  }
  
  // Enhanced print method that shows color matching details
  void printColorMatchDetails()
  {
    auto tample = getTample();
    if (tample._note == -1)
    {
      Serial.println("No color match found");
      return;
    }
    
    Serial.print("Detected color: ");
    Serial.print(tample._name);
    Serial.print(" (note: ");
    Serial.print(tample._note);
    Serial.print(", target hue: ");
    Serial.print(tample._hue);
    Serial.print("±");
    Serial.print(tample._tolerance);
    Serial.print(", target saturation: ");
    Serial.print(tample._saturation);
    Serial.print("±");
    Serial.print(tample._saturationTolerance);
    Serial.print(", target brightness: ");
    Serial.print(tample._brightness);
    Serial.print("±");
    Serial.print(tample._brightnessTolerance);
    Serial.println(")");
    
    // Show current values
    HSV current = getCurrentHistoryEntry();
    Serial.print("Current values - hue: ");
    Serial.print(current.h);
    Serial.print(", saturation: ");
    Serial.print(current.s);
    Serial.print(", brightness: ");
    Serial.println(current.v);
  }
  
  void printToken()
  {
    auto tample = getTample();
    if (tample._note == -1)
    {
      Serial.println(tample._name);
      return;
    }
    Serial.print("note: ");
    Serial.print(tample._note);
    Serial.print(", name: ");
    Serial.println(tample._name);
  }
};