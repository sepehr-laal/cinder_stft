#ifndef CISTFT_INCLUDE_STFT_CLIENT_STORAGE_H_
#define CISTFT_INCLUDE_STFT_CLIENT_STORAGE_H_

#include <cinder/audio/dsp/Dsp.h>
#include <cinder/audio/dsp/Fft.h>
#include <cinder/audio/Buffer.h>

#include "stft_client.h"

namespace cistft {

class AppGlobals;

namespace stft {

struct ClientStorage
{
	ClientStorage(const Client::Format& fmt, AppGlobals* const globals);

	std::unique_ptr<ci::audio::dsp::Fft>	mFft;
	ci::audio::Buffer						mCopiedBuffer;		// not windowed samples before transform
	ci::audio::Buffer						mFftBuffer;			// windowed samples before transform
	ci::audio::BufferSpectral				mBufferSpectral;	// transformed samples
	std::vector<float>						mMagSpectrum;		// computed magnitude spectrum from frequency-domain samples
	ci::audio::AlignedArrayPtr				mWindowingTable;
	std::size_t								mFftSize;
	std::size_t								mChannelSize;
	std::size_t								mWindowSize;
	ci::audio::dsp::WindowType				mWindowType;
	float									mSmoothingFactor;
	float									mChannelScale;		// one over channel size
	float									mMagnitudeScale;	// one over FFT size
};

}} // !namespace cistft

#endif // !CISTFT_INCLUDE_STFT_CLIENT_STORAGE_H_