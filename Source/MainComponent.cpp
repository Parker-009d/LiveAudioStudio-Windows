#include "MainComponent.h"

namespace
{
    const juce::Colour background { 14, 18, 24 };
    const juce::Colour strip      { 27, 34, 43 };
    const juce::Colour panel      { 21, 27, 35 };
    const juce::Colour inset      { 12, 17, 23 };
    const juce::Colour line       { 54, 68, 80 };
    const juce::Colour text       { 238, 244, 248 };
    const juce::Colour muted      { 145, 160, 173 };
    const juce::Colour amber      { 42, 198, 180 };
    const juce::Colour green      { 58, 194, 119 };
    const juce::Colour red        { 226, 73, 69 };

    void labelStyle(juce::Label& label, const juce::String& value,
                    float size, juce::Colour colour,
                    juce::Justification align = juce::Justification::centredLeft)
    {
        label.setText(value, juce::dontSendNotification);
        label.setFont(juce::Font(size, juce::Font::bold));
        label.setColour(juce::Label::textColourId, colour);
        label.setJustificationType(align);
        label.setInterceptsMouseClicks(false, false);
    }

    void knobStyle(juce::Slider& slider, const juce::String& suffix = {})
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 62, 18);
        slider.setTextValueSuffix(suffix);
        slider.setColour(juce::Slider::rotarySliderFillColourId, amber);
        slider.setColour(juce::Slider::rotarySliderOutlineColourId, line);
        slider.setColour(juce::Slider::thumbColourId, text);
        slider.setColour(juce::Slider::textBoxTextColourId, text);
        slider.setColour(juce::Slider::textBoxBackgroundColourId, inset);
        slider.setColour(juce::Slider::textBoxOutlineColourId, line);
    }

    void faderStyle(juce::Slider& slider)
    {
        slider.setSliderStyle(juce::Slider::LinearVertical);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 20);
        slider.setColour(juce::Slider::trackColourId, juce::Colour(110, 123, 137));
        slider.setColour(juce::Slider::thumbColourId, juce::Colour(225, 230, 235));
        slider.setColour(juce::Slider::backgroundColourId, inset);
        slider.setColour(juce::Slider::textBoxTextColourId, text);
        slider.setColour(juce::Slider::textBoxBackgroundColourId, inset);
        slider.setColour(juce::Slider::textBoxOutlineColourId, line);
    }

    void buttonStyle(juce::TextButton& button)
    {
        button.setClickingTogglesState(true);
        button.setColour(juce::TextButton::buttonColourId, juce::Colour(48, 58, 69));
        button.setColour(juce::TextButton::buttonOnColourId, juce::Colour(145, 57, 54));
        button.setColour(juce::TextButton::textColourOffId, text);
        button.setColour(juce::TextButton::textColourOnId, text);
    }

    void drawMeter(juce::Graphics& g, juce::Rectangle<int> r, float level, bool clipping)
    {
        g.setColour(inset);
        g.fillRoundedRectangle(r.toFloat(), 3.0f);

        constexpr int segments = 24;
        const int padX = 2;
        const int lit = juce::jlimit(0, segments,
            static_cast<int>(std::round(juce::jlimit(0.0f, 1.0f, level) * segments)));

        for (int i = 0; i < segments; ++i)
        {
            const int yIndex = segments - 1 - i;
            const int segmentTop = r.getY() + (yIndex * r.getHeight()) / segments;
            const int segmentBottom = r.getY() + ((yIndex + 1) * r.getHeight()) / segments;

            juce::Colour activeColour;
            juce::Colour inactiveColour;

            if (i >= 19)
            {
                activeColour = red;
                inactiveColour = juce::Colour(75, 46, 47);
            }
            else if (i >= 15)
            {
                activeColour = amber;
                inactiveColour = juce::Colour(78, 69, 46);
            }
            else
            {
                activeColour = green;
                inactiveColour = juce::Colour(40, 73, 59);
            }
