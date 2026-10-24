
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

            g.setColour(i < lit ? activeColour : inactiveColour);
            g.fillRect(r.getX() + padX, segmentTop,
                       r.getWidth() - padX * 2, segmentBottom - segmentTop);
        }

        g.setColour(clipping ? red : juce::Colour(78, 48, 49));
        g.fillEllipse(static_cast<float>(r.getRight() - 10),
                      static_cast<float>(r.getY() - 15), 8.0f, 8.0f);

        g.setColour(muted);
        g.setFont(juce::Font(8.0f, juce::Font::bold));
        g.drawText("CLIP", r.getRight() - 25, r.getY() - 28, 32, 10,
                   juce::Justification::centred);
    }
}

void MixerLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    juce::ignoreUnused(slider);
    auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height).reduced(9.0f);
    auto diameter = juce::jmin(bounds.getWidth(), bounds.getHeight());
    auto knob = bounds.withSizeKeepingCentre(diameter, diameter);
    auto radius = diameter * 0.5f;
    auto centre = knob.getCentre();
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    g.setColour(juce::Colour(8, 12, 17));
    g.fillEllipse(knob.expanded(2.0f));
    g.setColour(juce::Colour(67, 82, 94));
    g.drawEllipse(knob, 1.4f);
    g.setColour(juce::Colour(31, 40, 49));
    g.fillEllipse(knob.reduced(3.0f));

    juce::Path arc;
    arc.addCentredArc(centre.x, centre.y, radius - 5.0f, radius - 5.0f, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(juce::Colour(65, 79, 91));
    g.strokePath(arc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path activeArc;
    activeArc.addCentredArc(centre.x, centre.y, radius - 5.0f, radius - 5.0f, 0.0f, rotaryStartAngle, angle, true);
    g.setColour(juce::Colour(42, 198, 180));
    g.strokePath(activeArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto pointerLength = radius * 0.56f;
    juce::Path pointer;
    pointer.addRoundedRectangle(-1.8f, -pointerLength, 3.6f, pointerLength * 0.56f, 1.5f);
    g.setColour(juce::Colour(235, 244, 248));
    g.fillPath(pointer, juce::AffineTransform::rotation(angle).translated(centre.x, centre.y));
    g.setColour(juce::Colour(12, 17, 23));
    g.fillEllipse(centre.x - 4.0f, centre.y - 4.0f, 8.0f, 8.0f);
}

void MixerLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider)
{
    if (style != juce::Slider::LinearVertical)
    {
        juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        return;
    }

    juce::ignoreUnused(minSliderPos, maxSliderPos, slider);
    auto area = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);
    auto centreX = area.getCentreX();
    auto slotWidth = 8.0f;
    auto slot = juce::Rectangle<float>(centreX - slotWidth * 0.5f, area.getY() + 8.0f, slotWidth, juce::jmax(20.0f, area.getHeight() - 16.0f));

    g.setColour(juce::Colour(7, 11, 15));
    g.fillRoundedRectangle(slot, 4.0f);
    g.setColour(juce::Colour(58, 72, 83));
    g.drawRoundedRectangle(slot, 4.0f, 1.0f);

    for (int i = 0; i <= 10; ++i)
    {
        auto tickY = slot.getY() + slot.getHeight() * (float)i / 10.0f;
        g.setColour(i == 5 ? juce::Colour(145, 160, 173) : juce::Colour(61, 75, 86));
        g.drawLine(centreX + 8.0f, tickY, centreX + (i == 5 ? 17.0f : 13.0f), tickY, 1.0f);
    }

    auto thumbY = juce::jlimit(slot.getY(), slot.getBottom(), sliderPos);
    auto cap = juce::Rectangle<float>(centreX - 17.0f, thumbY - 8.0f, 34.0f, 16.0f);
    g.setColour(juce::Colour(8, 12, 17));
    g.fillRoundedRectangle(cap.translated(0.0f, 2.0f), 3.0f);
    g.setColour(juce::Colour(104, 121, 133));
    g.fillRoundedRectangle(cap, 3.0f);
    g.setColour(juce::Colour(219, 229, 235));
    g.drawLine(cap.getX() + 4.0f, thumbY - 4.0f, cap.getRight() - 4.0f, thumbY - 4.0f, 1.0f);
    g.setColour(juce::Colour(36, 49, 59));
    g.drawLine(cap.getX() + 4.0f, thumbY + 4.0f, cap.getRight() - 4.0f, thumbY + 4.0f, 1.0f);
}

MainComponent::MainComponent()
{
    setSize(1280, 760);
    setAudioChannels(2, 2);


    labelStyle(titleLabel, "LIVE AUDIO STUDIO", 19.0f, text);
    labelStyle(statusLabel, "LIVE MIXER  /  TWO INPUTS  /  MAIN OUTPUT", 10.0f, muted);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(statusLabel);

    buttonStyle(settingsButton);
    settingsButton.setClickingTogglesState(false);
    addAndMakeVisible(settingsButton);
    settingsButton.onClick = [this]
    {
        juce::DialogWindow::LaunchOptions options;
        options.content.setOwned(new juce::AudioDeviceSelectorComponent(
            deviceManager, 0, 2, 0, 2, false, false, true, false));
        options.dialogTitle = "Audio Settings";
        options.dialogBackgroundColour = juce::Colour(35, 42, 51);
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = true;
        options.launchAsync();
    };


    labelStyle(vocalLabel, "VOCAL", 16.0f, text);
    labelStyle(keyboardLabel, "KEYBOARD", 16.0f, text);
    labelStyle(masterLabel, "MASTER", 16.0f, amber);

    labelStyle(vocalSubtitle, "SHURE SM7B  /  INPUT 1", 9.0f, muted);
    labelStyle(keyboardSubtitle, "NORD STAGE  /  INPUT 2", 9.0f, muted);
    labelStyle(masterSubtitle, "MAIN OUTPUT", 9.0f, muted);

    for (auto* c : { &vocalLabel, &keyboardLabel, &masterLabel,
                     &vocalSubtitle, &keyboardSubtitle, &masterSubtitle })
        addAndMakeVisible(*c);

    labelStyle(vocalGainLabel, "GAIN", 9.0f, muted, juce::Justification::centred);
    labelStyle(vocalPanLabel, "PAN", 9.0f, muted, juce::Justification::centred);
    labelStyle(vocalFXLabel, "FX SEND", 9.0f, muted, juce::Justification::centred);
    labelStyle(keyboardGainLabel, "GAIN", 9.0f, muted, juce::Justification::centred);
    labelStyle(keyboardPanLabel, "PAN", 9.0f, muted, juce::Justification::centred);
    labelStyle(keyboardFXLabel, "FX SEND", 9.0f, muted, juce::Justification::centred);

    for (auto* c : { &vocalGainLabel, &vocalPanLabel, &vocalFXLabel,
                     &keyboardGainLabel, &keyboardPanLabel, &keyboardFXLabel })
        addAndMakeVisible(*c);

    knobStyle(vocalGain, " dB");
    knobStyle(vocalPan);
    knobStyle(vocalFX, "%");
    knobStyle(keyboardGain, " dB");
    knobStyle(keyboardPan);
    knobStyle(keyboardFX, "%");

    faderStyle(vocalFader);
    faderStyle(keyboardFader);
    faderStyle(masterFader);

    for (auto* knob : { &vocalGain, &vocalPan, &vocalFX, &keyboardGain, &keyboardPan, &keyboardFX })
        knob->setLookAndFeel(&mixerLookAndFeel);

    for (auto* fader : { &vocalFader, &keyboardFader, &masterFader })
        fader->setLookAndFeel(&mixerLookAndFeel);

    vocalGain.setRange(-12.0, 12.0, 0.1);
    vocalGain.setValue(0.0);
    vocalPan.setRange(-1.0, 1.0, 0.01);
    vocalPan.setValue(0.0);
    vocalFX.setRange(0.0, 100.0, 1.0);
    vocalFX.setValue(20.0);

    keyboardGain.setRange(-12.0, 12.0, 0.1);
    keyboardGain.setValue(0.0);
    keyboardPan.setRange(-1.0, 1.0, 0.01);
    keyboardPan.setValue(0.0);
    keyboardFX.setRange(0.0, 100.0, 1.0);
    keyboardFX.setValue(15.0);

    for (auto* f : { &vocalFader, &keyboardFader, &masterFader })
    {
        f->setRange(-60.0, 10.0, 0.1);
        f->setValue(0.0);
        addAndMakeVisible(*f);
    }

    vocalFaderDb.store((float) vocalFader.getValue());
    keyboardFaderDb.store((float) keyboardFader.getValue());
    masterFaderDb.store((float) masterFader.getValue());
    vocalGainDb.store((float) vocalGain.getValue());
    keyboardGainDb.store((float) keyboardGain.getValue());
    vocalPanValue.store((float) vocalPan.getValue());
    keyboardPanValue.store((float) keyboardPan.getValue());

    vocalFader.onValueChange = [this] { vocalFaderDb.store((float) vocalFader.getValue()); };
    keyboardFader.onValueChange = [this] { keyboardFaderDb.store((float) keyboardFader.getValue()); };
    masterFader.onValueChange = [this] { masterFaderDb.store((float) masterFader.getValue()); };
    vocalGain.onValueChange = [this] { vocalGainDb.store((float) vocalGain.getValue()); };
    keyboardGain.onValueChange = [this] { keyboardGainDb.store((float) keyboardGain.getValue()); };
    vocalPan.onValueChange = [this] { vocalPanValue.store((float) vocalPan.getValue()); };
    keyboardPan.onValueChange = [this] { keyboardPanValue.store((float) keyboardPan.getValue()); };
    vocalFX.onValueChange = [this] { vocalFXAmount.store((float) vocalFX.getValue()); };

    for (auto* s : { &vocalGain, &vocalPan, &vocalFX,
                     &keyboardGain, &keyboardPan, &keyboardFX })
        addAndMakeVisible(*s);

    for (auto* b : { &vocalSoloButton, &vocalMuteButton,
                     &keyboardSoloButton, &keyboardMuteButton, &masterMuteButton })
    {
        buttonStyle(*b);
        addAndMakeVisible(*b);
    }

    vocalMuteButton.onClick = [this]
    {
        vocalMuted = vocalMuteButton.getToggleState();
        repaint();
    };
    keyboardMuteButton.onClick = [this]
    {
        keyboardMuted = keyboardMuteButton.getToggleState();
        repaint();
    };
    masterMuteButton.onClick = [this]
    {
        masterMuted = masterMuteButton.getToggleState();
        repaint();
    };
    vocalSoloButton.onClick = [this]
    {
        vocalSolo = vocalSoloButton.getToggleState();
        repaint();
    };
    keyboardSoloButton.onClick = [this]
    {
        keyboardSolo = keyboardSoloButton.getToggleState();
        repaint();
    };

    auto setupTab = [this](juce::TextButton& b, const juce::String& name)
    {
        buttonStyle(b);
        addAndMakeVisible(b);
        b.onClick = [this, name] { showPanel(name); };
    };

    setupTab(effectsTabButton, "EFFECTS");
    setupTab(parametricEQTabButton, "PARAMETRIC EQ");
    setupTab(graphicEQTabButton, "GRAPHIC EQ");
    setupTab(compressorTabButton, "COMPRESSOR");
    setupTab(noiseGateTabButton, "NOISE GATE");
    setupTab(deEsserTabButton, "DE-ESSER");

    auto setupGear = [this](juce::TextButton& b, const juce::String& panel)
    {
        buttonStyle(b);
        b.setTooltip("Open " + panel + " controls");
        addAndMakeVisible(b);
        b.onClick = [this, panel] { showPanel(panel); };
    };

    setupGear(vocalGearButton, "EFFECTS");
    setupGear(keyboardGearButton, "PARAMETRIC EQ");
    setupGear(masterGearButton, "EFFECTS");

    buttonStyle(closePanelButton);
    closePanelButton.setButtonText("HIDE PANEL");
    addAndMakeVisible(closePanelButton);
    closePanelButton.onClick = [this]
    {
        panelVisible = false;
        updatePanelVisibility();
        resized();
        repaint();
    };

    labelStyle(panelTitleLabel, "EFFECTS", 13.0f, text);
    labelStyle(panelStatusLabel,
               "Processor controls are visual placeholders until connected to the audio engine.",
               10.0f, muted);
    addAndMakeVisible(panelTitleLabel);
    addAndMakeVisible(panelStatusLabel);

    auto setupPanelControl = [this](juce::Slider& s, juce::Label& l,
                                    const juce::String& name,
                                    double low, double high, double initial,
                                    const juce::String& suffix)
    {
        labelStyle(l, name, 9.0f, muted, juce::Justification::centred);
        addAndMakeVisible(l);
        knobStyle(s, suffix);
        s.setRange(low, high, 0.1);
        s.setValue(initial);
        addAndMakeVisible(s);
    };

    setupPanelControl(effectAmountSlider, effectAmountLabel, "AMOUNT", 0, 100, 25, "%");
    setupPanelControl(frequencySlider, frequencyLabel, "FREQUENCY", 20, 20000, 1000, " Hz");
    setupPanelControl(eqGainSlider, eqGainLabel, "GAIN", -18, 18, 0, " dB");

    frequencySlider.onValueChange = [this]
    {
        eqFrequencyHz.store((float) frequencySlider.getValue());
    };

    eqGainSlider.onValueChange = [this]
    {
        eqGainDb.store((float) eqGainSlider.getValue());
    };
    setupPanelControl(thresholdSlider, thresholdLabel, "THRESHOLD", -60, 0, -18, " dB");
    setupPanelControl(ratioSlider, ratioLabel, "RATIO", 1, 20, 4, ":1");
    setupPanelControl(attackSlider, attackLabel, "ATTACK", 0.1, 100, 10, " ms");
    setupPanelControl(releaseSlider, releaseLabel, "RELEASE", 5, 1000, 120, " ms");

    startTimerHz(30);
    showPanel("EFFECTS");
}

MainComponent::~MainComponent()
{
    for (auto* control : { &vocalGain, &vocalPan, &vocalFX, &keyboardGain, &keyboardPan, &keyboardFX, &vocalFader, &keyboardFader, &masterFader })
        control->setLookAndFeel(nullptr);
    shutdownAudio();
}

void MainComponent::showPanel(const juce::String& name)
{
    activePanel = name;
    panelVisible = true;
    panelTitleLabel.setText(name, juce::dontSendNotification);
    panelStatusLabel.setText(
        name + "  /  UI ONLY — AUDIO PROCESSING NOT CONNECTED",
        juce::dontSendNotification);
    updatePanelVisibility();
    resized();
    repaint();
}

void MainComponent::updatePanelVisibility()
{
    panelTitleLabel.setVisible(panelVisible);
    panelStatusLabel.setVisible(panelVisible);
    closePanelButton.setVisible(panelVisible);

    const bool effect = panelVisible && activePanel == "EFFECTS";
    const bool eq = panelVisible &&
        (activePanel == "PARAMETRIC EQ" || activePanel == "GRAPHIC EQ");
    const bool dynamics = panelVisible &&
        (activePanel == "COMPRESSOR" || activePanel == "NOISE GATE"
         || activePanel == "DE-ESSER");

    effectAmountLabel.setVisible(effect);
    effectAmountSlider.setVisible(effect);

    frequencyLabel.setVisible(eq);
    frequencySlider.setVisible(eq);
    eqGainLabel.setVisible(eq);
    eqGainSlider.setVisible(eq);

    thresholdLabel.setVisible(dynamics);
    thresholdSlider.setVisible(dynamics);
    ratioLabel.setVisible(dynamics);
    ratioSlider.setVisible(dynamics);
    attackLabel.setVisible(dynamics);
    attackSlider.setVisible(dynamics);
    releaseLabel.setVisible(dynamics);
    releaseSlider.setVisible(dynamics);
}

void MainComponent::prepareToPlay(int, double) {}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& info)
{
    info.clearActiveBufferRegion();
}

void MainComponent::audioDeviceIOCallbackWithContext(
    const float* const* inputChannelData,
    int numInputChannels,
    float* const* outputChannelData,
    int numOutputChannels,
    int numSamples,
    const juce::AudioIODeviceCallbackContext& context)
{
    juce::ignoreUnused(context);

    if (outputChannelData == nullptr || numOutputChannels <= 0 || numSamples <= 0)
        return;

    for (int channel = 0; channel < numOutputChannels; ++channel)
        if (outputChannelData[channel] != nullptr)
            juce::FloatVectorOperations::clear(outputChannelData[channel], numSamples);

    auto inputSample = [inputChannelData, numInputChannels](int channel, int sample)
    {
        if (inputChannelData == nullptr || channel < 0 || channel >= numInputChannels
            || inputChannelData[channel] == nullptr)
            return 0.0f;

        return inputChannelData[channel][sample];
    };

    auto measurePeak = [&inputSample, numSamples](int channel)
    {
        float peak = 0.0f;
        for (int i = 0; i < numSamples; ++i)
            peak = juce::jmax(peak, std::abs(inputSample(channel, i)));
        return peak;
    };

    const float vocal = measurePeak(0);
    const float keyboard = measurePeak(1);

    vocalPeak.store(vocal);
    keyboardPeak.store(keyboard);

    if (vocal >= 0.999f) vocalClip.store(true);
    if (keyboard >= 0.999f) keyboardClip.store(true);

    const bool anySolo = vocalSolo.load() || keyboardSolo.load();
    const bool useVocal = !vocalMuted.load() && (!anySolo || vocalSolo.load());
    const bool useKeyboard = !keyboardMuted.load() && (!anySolo || keyboardSolo.load());
    const bool useMaster = !masterMuted.load();

    const float vocalGain = juce::Decibels::decibelsToGain(vocalGainDb.load());
    const float keyboardGain = juce::Decibels::decibelsToGain(keyboardGainDb.load());
    const float masterGain = useMaster
        ? juce::Decibels::decibelsToGain(masterFaderDb.load()) : 0.0f;

    const float vocalFaderGain = useVocal
        ? juce::Decibels::decibelsToGain(vocalFaderDb.load()) : 0.0f;
    const float keyboardFaderGain = useKeyboard
        ? juce::Decibels::decibelsToGain(keyboardFaderDb.load()) : 0.0f;

    const float vocalAngle = (juce::jlimit(-1.0f, 1.0f, vocalPanValue.load()) + 1.0f)
        * juce::MathConstants<float>::pi * 0.25f;
    const float keyboardAngle = (juce::jlimit(-1.0f, 1.0f, keyboardPanValue.load()) + 1.0f)
        * juce::MathConstants<float>::pi * 0.25f;

    const float vocalLeft = std::cos(vocalAngle) * vocalGain * vocalFaderGain;
    const float vocalRight = std::sin(vocalAngle) * vocalGain * vocalFaderGain;
    const float keyboardLeft = std::cos(keyboardAngle) * keyboardGain * keyboardFaderGain;
    const float keyboardRight = std::sin(keyboardAngle) * keyboardGain * keyboardFaderGain;

    const float requestedFrequency = eqFrequencyHz.load();
    const float requestedGainDb = eqGainDb.load();
    const float maxFrequency = (float) (audioSampleRate * 0.45);
    const float safeFrequency = juce::jlimit(
        20.0f, juce::jmax(20.0f, maxFrequency), requestedFrequency);

    if (std::abs(safeFrequency - lastAppliedEqFrequencyHz) > 0.1f
        || std::abs(requestedGainDb - lastAppliedEqGainDb) > 0.01f)
    {
        auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            audioSampleRate,
            safeFrequency,
            1.0f,
            juce::Decibels::decibelsToGain(requestedGainDb));

        eqLeftFilter.coefficients = coefficients;
        eqRightFilter.coefficients = coefficients;
        lastAppliedEqFrequencyHz = safeFrequency;
        lastAppliedEqGainDb = requestedGainDb;
    }

    const float requestedFXAmount =
        juce::jlimit(0.0f, 100.0f, vocalFXAmount.load());

    if (std::abs(requestedFXAmount - lastAppliedVocalFXAmount) > 0.1f)
    {
        juce::Reverb::Parameters reverbParameters;
        reverbParameters.roomSize = 0.5f;
        reverbParameters.damping = 0.4f;
        reverbParameters.wetLevel = (requestedFXAmount / 100.0f) * 0.35f;
        reverbParameters.dryLevel = 1.0f;
        reverbParameters.width = 1.0f;
        reverbParameters.freezeMode = 0.0f;

        vocalReverbProcessor.setParameters(reverbParameters);
        lastAppliedVocalFXAmount = requestedFXAmount;
    }

    const int reverbBufferSize = vocalReverbBuffer.getNumSamples();
    if (reverbBufferSize <= 0)
        return;

    float outputPeak = 0.0f;

    // Process vocal reverb in bounded chunks using the prepared buffer.
    for (int blockStart = 0; blockStart < numSamples;
         blockStart += reverbBufferSize)
    {
        const int blockSamples =
            juce::jmin(reverbBufferSize, numSamples - blockStart);

        auto* vocalLeftBuffer = vocalReverbBuffer.getWritePointer(0);
        auto* vocalRightBuffer = vocalReverbBuffer.getWritePointer(1);

        for (int i = 0; i < blockSamples; ++i)
        {
            const float vocalSample = inputSample(0, blockStart + i);
            vocalLeftBuffer[i] = vocalSample * vocalLeft;
            vocalRightBuffer[i] = vocalSample * vocalRight;
        }

        vocalReverbProcessor.processStereo(
            vocalLeftBuffer, vocalRightBuffer, blockSamples);

        for (int i = 0; i < blockSamples; ++i)
        {
            const int sampleIndex = blockStart + i;
            const float keyboardSample = inputSample(1, sampleIndex);

            const float mixedLeft =
                (vocalLeftBuffer[i] + keyboardSample * keyboardLeft) * masterGain;
            const float mixedRight =
                (vocalRightBuffer[i] + keyboardSample * keyboardRight) * masterGain;

            const float left = eqLeftFilter.processSample(mixedLeft);
            const float right = eqRightFilter.processSample(mixedRight);

            outputPeak = juce::jmax(outputPeak, std::abs(left), std::abs(right));

            if (outputChannelData[0] != nullptr)
                outputChannelData[0][sampleIndex] = numOutputChannels == 1
                    ? juce::jlimit(-1.0f, 1.0f, (left + right) * 0.5f)
                    : juce::jlimit(-1.0f, 1.0f, left);

            if (numOutputChannels > 1 && outputChannelData[1] != nullptr)
                outputChannelData[1][sampleIndex] =
                    juce::jlimit(-1.0f, 1.0f, right);
        }
    }

    masterPeak.store(outputPeak);
    if (outputPeak >= 0.999f)
        masterClip.store(true);
}

void MainComponent::audioDeviceAboutToStart(juce::AudioIODevice* device)
{
    if (device != nullptr && device->getCurrentSampleRate() > 0.0)
        audioSampleRate = device->getCurrentSampleRate();

    eqLeftFilter.reset();
    eqRightFilter.reset();

    const int bufferSize = device != nullptr
        ? juce::jmax(1, device->getCurrentBufferSizeSamples())
        : 512;
    vocalReverbBuffer.setSize(2, bufferSize, false, true, true);
    vocalReverbBuffer.clear();
    vocalReverbProcessor.reset();
    lastAppliedVocalFXAmount = -1.0f;

    lastAppliedEqFrequencyHz = -1.0f;
    lastAppliedEqGainDb = 999.0f;
}

void MainComponent::audioDeviceStopped() {}

void MainComponent::releaseResources() {}

void MainComponent::timerCallback()
{
    auto smoothMeter = [](float current, float target)
    {
        const float coefficient = target > current ? 0.25f : 0.08f;
        return current + coefficient * (target - current);
    };

    vocalDisplayPeak = smoothMeter(vocalDisplayPeak, vocalPeak.load());
    keyboardDisplayPeak = smoothMeter(keyboardDisplayPeak, keyboardPeak.load());
    masterDisplayPeak = smoothMeter(masterDisplayPeak, masterPeak.load());

    repaint();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(background);

    g.setColour(panel);
    g.fillRect(0, 0, getWidth(), 66);
    g.setColour(line);
    g.drawHorizontalLine(65, 0.0f, static_cast<float>(getWidth()));

    const int margin = 18;
    const int gap = 10;
    const int top = 76;
    const int bottom = panelVisible ? getHeight() - 294 : getHeight() - 24;
    const int usable = getWidth() - margin * 2 - gap * 2;
    const int w1 = usable * 34 / 100;
    const int w2 = usable * 34 / 100;
    const int w3 = usable - w1 - w2;

    int x = margin;
    for (int i = 0; i < 3; ++i)
    {
        const int w = i == 0 ? w1 : (i == 1 ? w2 : w3);
        juce::Rectangle<float> r(static_cast<float>(x), static_cast<float>(top),
                                 static_cast<float>(w), static_cast<float>(bottom - top));
        g.setColour(strip);
        g.fillRoundedRectangle(r, 5.0f);
        g.setColour(i == 2 ? amber : line);
        g.drawRoundedRectangle(r.reduced(0.5f), 5.0f, 1.0f);
        g.setColour(i == 2 ? amber : line);
        g.fillRect(x, top, w, 3);
        x += w + gap;
    }

    const int meterY = top + 156;
    drawMeter(g, { margin + 18, meterY, 20, 190 }, vocalDisplayPeak, vocalClip.load());
    drawMeter(g, { margin + w1 + gap + 18, meterY, 20, 190 },
              keyboardDisplayPeak, keyboardClip.load());
    drawMeter(g, { margin + w1 + gap + w2 + gap + 18, meterY, 20, 190 },
              masterDisplayPeak, masterClip.load());

    g.setColour(muted);
    g.setFont(juce::Font(9.0f));
    g.drawText("INPUT PEAK", margin + 4, top + 129, 70, 13, juce::Justification::centredLeft);
    g.drawText("INPUT PEAK", margin + w1 + gap + 4, top + 129, 70, 13, juce::Justification::centredLeft);
    g.drawText("OUTPUT PEAK", margin + w1 + gap + w2 + gap + 4,
               top + 129, 85, 13, juce::Justification::centredLeft);

    if (panelVisible)
    {
        const int y = getHeight() - 276;
        g.setColour(panel);
        g.fillRoundedRectangle(22.0f, static_cast<float>(y),
                               static_cast<float>(getWidth() - 44), 250.0f, 5.0f);
        g.setColour(line);
        g.drawRoundedRectangle(22.0f, static_cast<float>(y),
                               static_cast<float>(getWidth() - 44), 250.0f, 5.0f, 1.0f);
    }
}

void MainComponent::resized()
{
    const int margin = 18;
    const int gap = 10;
    const int top = 76;
    const int bottom = panelVisible ? getHeight() - 294 : getHeight() - 24;
    const int height = juce::jmax(350, bottom - top);
    const int usable = getWidth() - margin * 2 - gap * 2;
    const int w1 = usable * 34 / 100;
    const int w2 = usable * 34 / 100;
    const int w3 = usable - w1 - w2;

    titleLabel.setBounds(24, 10, 360, 28);
    statusLabel.setBounds(25, 39, 520, 16);
    settingsButton.setBounds(getWidth() - 142, 14, 118, 30);

    auto channel = [&](int x, int w, juce::Label& name, juce::Label& subtitle,
                       juce::Slider& gain, juce::Slider& pan, juce::Slider& fx,
                       juce::Label& gainText, juce::Label& panText, juce::Label& fxText,
                       juce::Slider& fader, juce::TextButton& solo,
                       juce::TextButton& mute, bool isMaster)
    {
        name.setBounds(x + 18, top + 13, w - 66, 24);
        subtitle.setBounds(x + 18, top + 39, w - 36, 16);

        juce::TextButton* gear = &vocalGearButton;
        if (&name == &keyboardLabel) gear = &keyboardGearButton;
        else if (isMaster) gear = &masterGearButton;
        gear->setBounds(x + w - 44, top + 12, 28, 28);

        if (!isMaster)
        {
            const int ky = top + 68;
            const int kw = 76;
            const int kx = x + 18;
            gainText.setBounds(kx, ky, kw, 14);
            gain.setBounds(kx, ky + 12, kw, 76);
            panText.setBounds(kx + kw + 8, ky, kw, 14);
            pan.setBounds(kx + kw + 8, ky + 12, kw, 76);
            fxText.setBounds(kx + (kw + 8) * 2, ky, kw, 14);
            fx.setBounds(kx + (kw + 8) * 2, ky + 12, kw, 76);

            solo.setBounds(x + w - 92, top + 91, 76, 26);
            mute.setBounds(x + w - 92, top + 121, 76, 26);
            fader.setBounds(x + w / 2 - 44, top + 203, 88, height - 220);
        }
        else
        {
            mute.setBounds(x + w - 92, top + 106, 76, 26);
            fader.setBounds(x + w / 2 - 44, top + 203, 88, height - 220);
        }
    };

    channel(margin, w1, vocalLabel, vocalSubtitle, vocalGain, vocalPan, vocalFX,
            vocalGainLabel, vocalPanLabel, vocalFXLabel, vocalFader,
            vocalSoloButton, vocalMuteButton, false);

    channel(margin + w1 + gap, w2, keyboardLabel, keyboardSubtitle,
            keyboardGain, keyboardPan, keyboardFX, keyboardGainLabel,
            keyboardPanLabel, keyboardFXLabel, keyboardFader,
            keyboardSoloButton, keyboardMuteButton, false);

    channel(margin + w1 + gap + w2 + gap, w3, masterLabel, masterSubtitle,
            vocalGain, vocalPan, vocalFX, vocalGainLabel, vocalPanLabel,
            vocalFXLabel, masterFader, vocalSoloButton, masterMuteButton, true);

    const int py = getHeight() - 276;
    panelTitleLabel.setBounds(40, py + 12, 250, 24);
    closePanelButton.setBounds(getWidth() - 150, py + 10, 110, 28);
    panelStatusLabel.setBounds(40, py + 39, getWidth() - 80, 18);

    const int cy = py + 70;
    int cx = 42;
    auto control = [&](juce::Label& l, juce::Slider& s)
    {
        l.setBounds(cx, cy, 92, 15);
        s.setBounds(cx, cy + 12, 92, 108);
        cx += 112;
    };

    control(effectAmountLabel, effectAmountSlider);
    control(frequencyLabel, frequencySlider);
    control(eqGainLabel, eqGainSlider);
    control(thresholdLabel, thresholdSlider);
    control(ratioLabel, ratioSlider);
    control(attackLabel, attackSlider);
    control(releaseLabel, releaseSlider);

    const int ty = getHeight() - 34;
    effectsTabButton.setBounds(22, ty, 90, 25);
    parametricEQTabButton.setBounds(117, ty, 130, 25);
    graphicEQTabButton.setBounds(252, ty, 112, 25);
    compressorTabButton.setBounds(369, ty, 110, 25);
    noiseGateTabButton.setBounds(484, ty, 100, 25);
    deEsserTabButton.setBounds(589, ty, 92, 25);

    updatePanelVisibility();
}
