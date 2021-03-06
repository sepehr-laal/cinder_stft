#ifndef CISTFT_INCLUDE_AUDIO_NODES_H_
#define CISTFT_INCLUDE_AUDIO_NODES_H_

#include <memory>

#include "work_manager.h"

namespace cinder {
namespace audio {
class InputDeviceNode;
class MonitorNode;
}} //!ci::audio

namespace cistft {
namespace audio {
class RecorderNode;
} //!cistft::audio
class AppGlobals;

/*!
 * \class AudioNodes
 * \brief holds and handles all Cinder audio nodes. Also takes care
 * of initialization of those nodes.
 * \note from my understanding, node in Cinder is a unit of audio
 * processing. For each operation (regardless of being input / output)
 * a node is required. I have three nodes here, one monitoring the raw
 * input and the other one performing FFT on it. The third is reading
 * the input.
 */
class AudioNodes
{
public:
	AudioNodes(AppGlobals&);

	// \brief initializes nodes and connect them together
	void												setupInput();
	void												setupRecorder();
	void												setupMonitor();
	// \brief enables reading from input
	void												enableInput();
	// \brief disables reading from input
	void												disableInput();
	// \brief toggles reading from input
	void												toggleInput();
	// \brief updates audio nodes class for one tick.
	void												update();
	// \brief returns true if everything is setup correctly for audio nodes
	bool												isInputReady() const;
	bool												isRecorderReady() const;
	bool												isMonitorReady() const;

	// \brief returns a pointer to the node which is recording audio
	cistft::audio::RecorderNode* const					getBufferRecorderNode();
	// \brief returns a pointer to the node which is having raw data in it
	cinder::audio::MonitorNode* const					getMonitorNode();

private:
	std::shared_ptr<cinder::audio::InputDeviceNode>		mInputDeviceNode;
	std::shared_ptr<cistft::audio::RecorderNode>		mBufferRecorderNode;
	std::shared_ptr<cinder::audio::MonitorNode>			mMonitorNode;

private:
	AppGlobals&											mGlobals;
	work::ClientRef										mStftClient;

private: //state
	bool												mIsInputReady;
	bool												mIsRecorderReady;
	bool												mIsMonitorReady;
	bool												mIsEnabled;
	std::size_t											mQueryPosition;
};

} //!cistft

#endif //!CISTFT_INCLUDE_AUDIO_NODES_H_