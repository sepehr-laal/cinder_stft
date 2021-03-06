#ifndef CISTFT_INCLUDE_APP_H_
#define CISTFT_INCLUDE_APP_H_

#include <cinder/app/AppNative.h>
#include <cinder/params/Params.h>

#include "app_globals.h"
#include "audio_nodes.h"
#include "work_manager.h"
#include "app_config.h"
#include "stft_renderer.h"
#include "monitor_renderer.h"
#include "grid_renderer.h"

namespace cistft
{

/*!
 * \class Application
 * \brief The main application class. Handles window initialization
 * window title, and other common juju.
 */
class Application final : public ci::app::AppNative
{
public:
	Application();

	//! tells Cinder how to prepare the window
	void			prepareSettings(Settings *settings) override final;
	//! sets up the application, this gets fired after Cinder is fully initialized.
	void			setup() override final;
	//! called at the beginning of every render loop, used to refresh data.
	void			update() override final;
	//! called at the end of every render loop, used to draw the data.
	void			draw() override final;
	//! draws the current refresh rate of the render loop.
	void			drawFps();
	//! gets fired on mouse click
	void			mouseDown(ci::app::MouseEvent event) override final;
	//! gets fired on keyboard click
	void			keyDown(ci::app::KeyEvent event) override final;
	//! sets up the GUI for the time before user hits Start
	void			setupPreLaunchGUI();
	//! sets up the GUI for the time after user hits Start
	void			setupPostLaunchGUI();

private:
	//! all user configurable parameters
	AppConfig		mAppConfig;
	//! the work manager that accepts background works from main thread
	work::Manager	mWorkManager;
	//! the globals class instance, used to store common data
	AppGlobals		mGlobals;
	//! audio I/O class instance
	AudioNodes		mAudioNodes;
	//! STFT renderer
	StftRenderer	mStftRenderer;
	//! simple audio monitor renderer
	MonitorRenderer	mMonitorRenderer;
	//! simple grid renderer
	GridRenderer	mGridRenderer;
	//! ciUI instance
	ci::params::InterfaceGlRef
					mGuiInstance;
};

} //!cistft

#endif //!CISTFT_INCLUDE_APP_H_