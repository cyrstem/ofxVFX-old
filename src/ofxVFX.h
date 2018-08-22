#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"

enum class ofxVFXMode
{
    NONE,
    BLOOM,
    CRT,
    SOBEL,
    SYMMETRY,
    STREAK,
    NOISEWARP,
    CA, // (Chromatic Aberration)
    MAX,
};

class ofxVFX
{
public:
    ofxVFX();
    void setup(const int w, const int h);
    void update(const float t);
    void bang();
    void begin();
    void end();
    void draw();

    // Setter
    inline void setGlobalColor(const ofFloatColor& color) { mGlobalColor = color; }
    inline void setVFXMode(const ofxVFXMode mode) { mMode = mode; }
    inline void setBloomAttenuation(const float attenuation) { mAttenuation = attenuation; }
    inline void setBloomOffsetScale(const float offsetScale) { mOffsetScale = offsetScale; }
    
    static const int NUM_VFX_MODES = static_cast<int>(ofxVFXMode::MAX);
    
private:
    void setupFbos();
    void setupShaders();
    
    ofxVFXMode mMode;

    int mWidth, mHeight;
    ofFbo mBaseFbo, mEffectFbo;
    
    float mTime;
    float mVal1, mVal2, mVal3;
    ofFloatColor mGlobalColor;
    
    // Bloom
    ofFbo mBrightnessThreshFbo, mBlurFbo[2], mCompositeFbo;
    ofxAutoReloadedShader mBrightnessThreshShader, mBlurShader, mCompositeShader;
    float mAttenuation, mOffsetScale;
    
    // CRT
    ofxAutoReloadedShader mCRTShader;
    
    // Sobel
    ofxAutoReloadedShader mSobelShader;
    
    // Symmetry
    ofxAutoReloadedShader mSymmetryShader;
    
    // Streak
    ofxAutoReloadedShader mStreakShader;
    
    // NoiseWarp
    ofxAutoReloadedShader mNoiseWarpShader;
    
    // CA (Chromatic Aberration)
    ofxAutoReloadedShader mCAShader;
};
