#include "grid_renderer.h"
#include "app_globals.h"
#include "audio_nodes.h"

#include <cinder/Color.h>
#include <cinder/app/App.h>
#include <cinder/params/Params.h>

namespace cistft {

GridRenderer::GridRenderer(AppGlobals& g)
	: mGlobals(g)
	, mLabelFrequency(4)
	, mGridColor(ci::Color(0.2f, 0.2f, 0.2f))
	, mLabelColor(ci::Color::white())
	, mLabelMargin(55.0f)
	, mMaxX(1.0f)
	, mMaxY(1.0f)
	, mMinX(0.0f)
	, mMinY(0.0f)
	, mStepX(50)
	, mStepY(50)
	, mVisible(false)
	, mHorizontalUnit("")
	, mHorizontalUnitTextToDraw("")
	, mVerticalUnit("")
	, mVerticalUnitTextToDraw("")
{}

void GridRenderer::draw()
{
	static ci::Font _axis_font("Arial", 20);

	if (!mVisible || !mGlobals.getAudioNodes().isRecorderReady()) return;

	ci::gl::SaveColorState _save_color;
	ci::gl::color(mGridColor);

	const auto _w_float = static_cast<float>(ci::app::getWindowWidth());
	const auto _h_float = static_cast<float>(ci::app::getWindowHeight());

	int count = 0;
	// draw horizontal axis
	for (int i = mStepY; i <= ci::app::getWindowHeight(); i += mStepY)
	{
		const auto _y = static_cast<float>(i);
		ci::gl::drawLine(ci::Vec2f(0.0f, _y), ci::Vec2f(_w_float, _y));

		if (count % mLabelFrequency == 0) // every 4 steps, draw string of where we're standing
		{
			const auto _val = ((i / _h_float) * (mMaxY - mMinY)) + mMinY;
			ci::gl::drawStringCentered(std::to_string(_val) + mHorizontalUnitTextToDraw, ci::Vec2f(mLabelMargin * ci::app::getWindowAspectRatio(), _y), mLabelColor, _axis_font);
		}

		count++;
	}

	count = 0;
	// draw vertical axis
	for (int i = mStepX; i <= ci::app::getWindowWidth(); i += mStepX)
	{
		const auto _x = static_cast<float>(i);
		ci::gl::drawLine(ci::Vec2f(_x, 0.0f), ci::Vec2f(_x, _h_float));

		if (count % mLabelFrequency == 0) // every 4 steps, draw string of where we're standing
		{
			ci::gl::pushMatrices();
			ci::gl::rotate(-90.0f);
			const auto _val = ((i / _w_float) * (mMaxX - mMinX)) + mMinX;
			ci::gl::drawStringCentered(std::to_string(_val) + mVerticalUnitTextToDraw, ci::Vec2f((mLabelMargin - ci::app::getWindowHeight()), _x), mLabelColor, _axis_font);
			ci::gl::popMatrices();
		}

		count++;
	}

}

namespace {
const static std::string GUI_SEPARATOR("_G");
const static std::string PRE_LAUNCH_TEXT("Grids will be configurable after you hit START.");
}

void GridRenderer::setupPreLaunchGUI(cinder::params::InterfaceGl* const gui)
{
	gui->addSeparator(GUI_SEPARATOR);
	gui->addText(PRE_LAUNCH_TEXT);
}

void GridRenderer::setupPostLaunchGUI(cinder::params::InterfaceGl* const gui)
{
	gui->removeParam(GUI_SEPARATOR);
	gui->removeParam(PRE_LAUNCH_TEXT);

	gui->addText("Grid options:");
	gui->addParam("Draw grids?", &mVisible);
	gui->addParam("Grid Color", &mGridColor);
	gui->addParam("Step X", &mStepX).min(10).max(100);
	gui->addParam("Step Y", &mStepY).min(10).max(100);
	gui->addParam("Label Frequency", &mLabelFrequency).min(1).max(10);
	gui->addParam("Label Margin", &mLabelMargin);
	gui->addParam("Label Color", &mLabelColor);
}

void GridRenderer::setHorizontalBoundary(float min /*= 0.0f*/, float max /*= 1.0f*/)
{
	mMinX = min;
	mMaxX = max;
}

void GridRenderer::setVerticalBoundary(float min /*= 0.0f*/, float max /*= 1.0f*/)
{
	// vertical axis draws upside down!
	mMinY = max;
	mMaxY = min;
}

void GridRenderer::setHorizontalUnit(const std::string& str)
{
	mHorizontalUnit = str;
	mHorizontalUnitTextToDraw = mHorizontalUnit.empty() ? "" : (" (" + mHorizontalUnit + ")");
}

void GridRenderer::setVerticalUnit(const std::string& str)
{
	mVerticalUnit = str;
	mVerticalUnitTextToDraw = mVerticalUnit.empty() ? "" : (" (" + mVerticalUnit + ")");
}

}