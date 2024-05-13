/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelloSamplerAudioProcessorEditor::HelloSamplerAudioProcessorEditor (HelloSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
    addAndMakeVisible(mLoadButton);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    //  ATTACK SLIDER

    mAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    //mAttackSlider.addListener(this);
    //mAttackSlider.setRange(0.0f, 5.0f, 0.01f);
    mAttackSlider.setNumDecimalPlacesToDisplay(2);

    addAndMakeVisible(mAttackSlider);

    mAttackLabel.setFont(12.5f);
    mAttackLabel.setText("Attack", juce::dontSendNotification);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.setColour(juce::Label::textColourId, juce::Colours::red);
    mAttackLabel.attachToComponent(&mAttackSlider, false);

    //addAndMakeVisible(mAttackLabel);

    mAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", mAttackSlider);

    //  DECAY SLIDER

    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    //mDecaySlider.addListener(this);
    //mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
    mDecaySlider.setNumDecimalPlacesToDisplay(2);

    addAndMakeVisible(mDecaySlider);

    mDecayLabel.setFont(12.5f);
    mDecayLabel.setText("Decay", juce::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);

    //addAndMakeVisible(mDecayLabel);

    mDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", mDecaySlider);


    //  SUSTAIN SLIDER

    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    //mSustainSlider.addListener(this);
    //mSustainSlider.setRange(0.0f, 5.0f, 0.01f);
    mSustainSlider.setNumDecimalPlacesToDisplay(2);

    addAndMakeVisible(mSustainSlider);

    mSustainLabel.setFont(12.5f);
    mSustainLabel.setText("Sustain", juce::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);

    //addAndMakeVisible(mSustainLabel);

    mSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", mSustainSlider);

    //  RELEASE SLIDER

    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    // mReleaseSlider.addListener(this);
    //mReleaseSlider.setRange(0.0f, 5.0f, 0.01f);
    mReleaseSlider.setNumDecimalPlacesToDisplay(2);

    addAndMakeVisible(mReleaseSlider);

    mReleaseLabel.setFont(12.5f);
    mReleaseLabel.setText("Release", juce::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);

    //addAndMakeVisible(mReleaseLabel);

    mReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RELEASE", mReleaseSlider);

    setSize (600, 200);



}

HelloSamplerAudioProcessorEditor::~HelloSamplerAudioProcessorEditor()
{
}

//==============================================================================
void HelloSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
   
    g.setColour(juce::Colours::white);
    


    if (mShouldBePainting == true)
    {
        juce::Path p;
        mAudioPoints.clear();

        auto waveForm = audioProcessor.getWaveForm();
        auto ratio = waveForm.getNumSamples() / getWidth();
        auto buffer = waveForm.getReadPointer(0);

        for (int sample = 0; sample < waveForm.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }

        p.startNewSubPath(0, getHeight() / 2);

        //skala na y axis
        for (int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, 200, 0);

            p.lineTo(sample, point);
        }

        g.strokePath(p, juce::PathStrokeType(2));

        mShouldBePainting = false;
    }
    

            //use the ratio to take values from the audio buffer to put in vector to display
            //draw the waveform

   /* if (audioProcessor.getNumSamplerSounds() > 0)
    {
        g.fillAll(juce::Colours::red);
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawText("Sound loaded", getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 20, juce::Justification::centred);

    }
    else
    {
        g.drawText("Sound loaded", getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 20, juce::Justification::centred);

    }*/


}

void HelloSamplerAudioProcessorEditor::resized()
{
    const auto startX = 0.6f;
    const auto startY = 0.6f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.4f;

    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + (dialWidth * 3), startY, dialWidth, dialHeight);

    //mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

bool HelloSamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }

    return false;
}

void HelloSamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(file))
        {
            mShouldBePainting = true;
            audioProcessor.loadFile(file);
        }
    }

    repaint();
}

//void HelloSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
//{
//    if (slider == &mAttackSlider)
//    {
//        audioProcessor.getADSRParams().attack = mAttackSlider.getValue();
//    }
//    else if (slider == &mDecaySlider)
//    {
//        audioProcessor.getADSRParams().decay = mDecaySlider.getValue();
//    }
//    else if (slider == &mSustainSlider)
//    {
//        audioProcessor.getADSRParams().sustain = mSustainSlider.getValue();
//    }
//    else if (slider == &mReleaseSlider)
//    {
//        audioProcessor.getADSRParams().release = mReleaseSlider.getValue();
//    }
//
//    audioProcessor.updateADSR();
//}