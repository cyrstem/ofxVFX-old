#include "ofxVFX.h"

ofxVFX::ofxVFX()
: mMode(ofxVFXMode::NONE)
{}

void ofxVFX::setup(const int w, const int h)
{
    mWidth = w; mHeight = h;
    setupFbos();
    setupShaders();
}

void ofxVFX::update(const float t)
{
    
}

void ofxVFX::begin()
{
    mBaseFbo.begin();
    ofClear(0, 255);
}

void ofxVFX::end()
{
    mBaseFbo.end();
    
    switch(mMode)
    {
        case ofxVFXMode::NONE:
            mEffectFbo.begin();
            ofSetColor(255);
            ofClear(0, 0);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mEffectFbo.end();
            break;
        case ofxVFXMode::BLOOM:
            // Brightness Threshold Pass
            mBrightnessThreshFbo.begin();
            ofClear(0, 0);
            mBrightnessThreshShader.begin();
            mBrightnessThreshShader.setUniform1f("uBrightnessThresh", 0.3);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mBrightnessThreshShader.end();
            mBrightnessThreshFbo.end();
            
            // Vertical Blur Pass
            mBlurFbo[0].begin();
            ofClear(0, 0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 0);
            mBlurShader.setUniform1f("uOffsetScale", 1.0);
            mBlurShader.setUniform1f("uAttenuation", 1.0);
            mBrightnessThreshFbo.draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[0].end();
            
            // Horizontal Blur Pass
            mBlurFbo[1].begin();
            ofClear(0, 0);
            mBlurShader.begin();
            mBlurShader.setUniform1i("uDirection", 1);
            mBlurShader.setUniform1f("uOffsetScale", 1.0);
            mBlurShader.setUniform1f("uAttenuation", 1.0);
            mBlurFbo[0].draw(0, 0, mWidth, mHeight);
            mBlurShader.end();
            mBlurFbo[1].end();
            
            // Composite Pass
            mCompositeFbo.begin();
            ofClear(0, 0);
            mCompositeShader.begin();
            mCompositeShader.setUniformTexture("uBlur", mBlurFbo[1].getTexture(), 1);
            mCompositeShader.setUniform1f("uGamma", 2.2);
            mBaseFbo.draw(0, 0, mWidth, mHeight);
            mCompositeShader.end();
            mCompositeFbo.end();
            
            // Final
            mEffectFbo.begin();
            ofSetColor(255);
            ofClear(0, 0);
            mCompositeFbo.draw(0, 0, mWidth, mHeight);
            mEffectFbo.end();
            break;
        default:
            break;
    }
}

void ofxVFX::draw()
{
    mEffectFbo.draw(0, 0, mWidth, mHeight);
}

void ofxVFX::setupFbos()
{
    mBaseFbo.allocate(mWidth, mHeight, GL_RGBA16F);
//    mBaseFbo.getTexture().getTextureData().bFlipTexture = true;
    mEffectFbo.allocate(mWidth, mHeight, GL_RGBA16F);
    
    // Bloom
    mBrightnessThreshFbo.allocate(mWidth, mHeight, GL_RGBA16F);
    for(int i=0; i<2; i++)
    {
        mBlurFbo[i].allocate(mWidth, mHeight, GL_RGBA16F);
    }
    mCompositeFbo.allocate(mWidth, mHeight, GL_RGBA16F);
}

void ofxVFX::setupShaders()
{
    // Bloom
    mBrightnessThreshShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Bloom/brightnessThresh.frag");
    mBlurShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Bloom/blur.frag");
    mCompositeShader.load("shaders/ofxVFX/pass.vert", "shaders/ofxVFX/Bloom/composite.frag");
}
