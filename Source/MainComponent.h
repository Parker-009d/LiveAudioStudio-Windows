
#pragma once

#include <JuceHeader.h>
#include <atomic>

class MixerLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics&, int, int, int, int,
                          float, float, float, juce::Slider&) override;
    void drawLinearSlider(juce::Graphics&, int, int, int, int, float,
                          float, float, juce::Slider::SliderStyle,
                          juce::Slider&) override;
};

class MainComponent : public juce::AudioAppComponent,
                      public juce::AudioIODeviceCallback,
                      private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void audioDeviceIOCallbackWithContext(
        const float* const* inputChannelData,
        int numInputChannels,
        float* const* outputChannelData,
        int numOutputChannels,
        int numSamples,
        const juce::AudioIODeviceCallbackContext& context) override;
    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void showPanel(const juce::String& name);
    void updatePanelVisibility();

    MixerLookAndFeel mixerLookAndFeel;
    juce::Label titleLabel, statusLabel;
    juce::TextButton settingsButton { "SETTINGS" };

    juce::Label vocalLabel, keyboardLabel, masterLabel;
    juce::Label vocalSubtitle, keyboardSubtitle, masterSubtitle;

    juce::Label vocalGainLabel, vocalPanLabel, vocalFXLabel;
    juce::Label keyboardGainLabel, keyboardPanLabel, keyboardFXLabel;

    juce::Slider vocalFader, keyboardFader, masterFader;
    juce::Slider vocalGain, vocalPan, vocalFX;
    juce::Slider keyboardGain, keyboardPan, keyboardFX;

    juce::TextButton vocalSoloButton { "SOLO" };
    juce::TextButton vocalMuteButton { "MUTE" };
    juce::TextButton keyboardSoloButton { "SOLO" };
    juce::TextButton keyboardMuteButton { "MUTE" };
    juce::TextButton masterMuteButton { "MUTE" };

    juce::TextButton vocalGearButton { "⚙" };
    juce::TextButton keyboardGearButton { "⚙" };
    juce::TextButton masterGearButton { "⚙" };

    juce::TextButton effectsTabButton { "EFFECTS" };
    juce::TextButton parametricEQTabButton { "PARAMETRIC EQ" };
    juce::TextButton graphicEQTabButton { "GRAPHIC EQ" };
    juce::TextButton compressorTabButton { "COMPRESSOR" };
    juce::TextButton noiseGateTabButton { "NOISE GATE" };
    juce::TextButton deEsserTabButton { "DE-ESSER" };
    juce::TextButton closePanelButton { "HIDE PANEL" };

    juce::Label panelTitleLabel, panelStatusLabel;

    juce::Slider effectAmountSlider, frequencySlider, eqGainSlider;
    juce::Slider thresholdSlider, ratioSlider, attackSlider, releaseSlider;

    // Real stereo parametric EQ state; filters will be used by the audio thread.
    juce::dsp::IIR::Filter<float> eqLeftFilter, eqRightFilter;
    juce::Reverb vocalReverbProcessor;
    std::atomic<float> vocalFXAmount { 20.0f };
    juce::AudioBuffer<float> vocalReverbBuffer;
    float lastAppliedVocalFXAmount = -1.0f;
    std::atomic<float> eqFrequencyHz { 1000.0f };
    std::atomic<float> eqGainDb { 0.0f };
    double audioSampleRate = 44100.0;
    float lastAppliedEqFrequencyHz = -1.0f;
    float lastAppliedEqGainDb = 999.0f;

    juce::Label effectAmountLabel, frequencyLabel, eqGainLabel;
    juce::Label thresholdLabel, ratioLabel, attackLabel, releaseLabel;

    std::atomic<bool> vocalMuted { false };
    std::atomic<bool> keyboardMuted { false };
    std::atomic<bool> masterMuted { false };
    std::atomic<bool> vocalSolo { false };
    std::atomic<bool> keyboardSolo { false };
    std::atomic<float> vocalFaderDb { 0.0f };
    std::atomic<float> keyboardFaderDb { 0.0f };
    std::atomic<float> masterFaderDb { 0.0f };
    std::atomic<float> vocalGainDb { 0.0f };
    std::atomic<float> keyboardGainDb { 0.0f };
    std::atomic<float> vocalPanValue { 0.0f };
    std::atomic<float> keyboardPanValue { 0.0f };
    bool panelVisible = false;

    juce::String activePanel = "EFFECTS";

    std::atomic<float> vocalPeak { 0.0f };
    std::atomic<float> keyboardPeak { 0.0f };
    std::atomic<float> masterPeak { 0.0f };

    // UI-only smoothed meter levels.
    float vocalDisplayPeak = 0.0f;
    float keyboardDisplayPeak = 0.0f;
    float masterDisplayPeak = 0.0f;

    std::atomic<bool> vocalClip { false };
    std::atomic<bool> keyboardClip { false };
    std::atomic<bool> masterClip { false };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
