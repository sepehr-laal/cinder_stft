/*
 Copyright (c) 2014, The Cinder Project

 This code is intended to be used with the Cinder C++ library, http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "audio_draw.h"
#include "audio_nodes.h"

#include <cinder/audio/Utilities.h>
#include <cinder/audio/MonitorNode.h>
#include <cinder/gl/gl.h>

namespace cieq
{

Plot::Plot()
	: mDrawBounds(true)
	, mBoundsColor(0.5f, 0.5f, 0.5f, 1)
{}

void Plot::draw()
{
	drawLocal();

	if (mDrawBounds) {
		ci::gl::color(mBoundsColor);
		ci::gl::drawStrokedRect(mBounds);
	}
}

SpectrumPlot::SpectrumPlot(AudioNodes& nodes)
	: mScaleDecibels( true )
	, mAudioNodes(nodes)
{}

// original draw function is from Cinder examples _audio/common
void SpectrumPlot::drawLocal()
{
	auto& spectrum = mAudioNodes.getMonitorSpectralNode()->getMagSpectrum();
	
	if (spectrum.empty())
		return;

	ci::ColorA bottomColor(0, 0, 0.7f, 1);

	float width = mBounds.getWidth();
	float height = mBounds.getHeight();
	std::size_t numBins = spectrum.size();
	float padding = 0;
	float binWidth = ( width - padding * ( numBins - 1 ) ) / (float)numBins;

	std::size_t numVerts = spectrum.size() * 2 + 2;
	if( mVerts.size() < numVerts ) {
		mVerts.resize( numVerts );
		mColors.resize( numVerts );
	}

	std::size_t currVertex = 0;
	ci::Rectf bin(mBounds.x1, mBounds.y1, mBounds.x1 + binWidth, mBounds.y2);
	for( std::size_t i = 0; i < numBins; i++ ) {
		float m = spectrum[i];
		if( mScaleDecibels )
			m = ci::audio::linearToDecibel(m) / 100;

		bin.y1 = bin.y2 - m * height;

		mVerts[currVertex] = bin.getLowerLeft();
		mColors[currVertex] = bottomColor;
		mVerts[currVertex + 1] = bin.getUpperLeft();
		mColors[currVertex + 1] = ci::ColorA( 0, m, 0.7f, 1 );

		bin += ci::Vec2f(binWidth + padding, 0);
		currVertex += 2;
	}

	mVerts[currVertex] = bin.getLowerLeft();
	mColors[currVertex] = bottomColor;
	mVerts[currVertex + 1] = bin.getUpperLeft();
	mColors[currVertex + 1] = mColors[currVertex - 1];

	ci::gl::color(0, 0.9f, 0);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glVertexPointer( 2, GL_FLOAT, 0, mVerts.data() );
	glColorPointer( 4, GL_FLOAT, 0, mColors.data() );  // note: on OpenGL ES v1.1, the 'size' param to glColorPointer can only be 4
	glDrawArrays( GL_TRIANGLE_STRIP, 0, (GLsizei)mVerts.size() );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}

void SpectrumPlot::enableDecibelsScale(bool on /*= true*/)
{
	mScaleDecibels = on;
}


WaveformPlot::WaveformPlot(AudioNodes& nodes)
	: mGraphColor(0, 0.9f, 0, 1)
	, mAudioNodes(nodes)
{}

void WaveformPlot::setGraphColor(const ci::ColorA& color)
{
	mGraphColor = color;
}

// original draw function is from Cinder examples _audio/common
void WaveformPlot::drawLocal()
{
	auto& buffer = mAudioNodes.getMonitorNode()->getBuffer();

	ci::gl::color(mGraphColor);

	const float waveHeight = mBounds.getHeight() / (float)buffer.getNumChannels();
	const float xScale = mBounds.getWidth() / (float)buffer.getNumFrames();

	float yOffset = mBounds.y1;
	for (std::size_t ch = 0; ch < buffer.getNumChannels(); ch++) {
		ci::PolyLine2f waveform;
		const float *channel = buffer.getChannel(ch);
		float x = mBounds.x1;
		for (std::size_t i = 0; i < buffer.getNumFrames(); i++) {
			x += xScale;
			float y = (1 - (channel[i] * 0.5f + 0.5f)) * waveHeight + yOffset;
			waveform.push_back(ci::Vec2f(x, y));
		}

		if (!waveform.getPoints().empty())
			ci::gl::draw(waveform);

		yOffset += waveHeight;
	}
}

} //!namespace cieq