#ifndef CIEQ_INCLUDE_GRID_RENDERER_H_
#define CIEQ_INCLUDE_GRID_RENDERER_H_

#include <cinder/Color.h>

namespace cieq {

class AppGlobals;

class GridRenderer
{
public:
	GridRenderer(AppGlobals&);
	void			draw();

	struct
	{
		// to be configured via GUI

		int			mStepX;
		int			mStepY;
		int			mLabelFrequency; // per step
		float		mLabelMargin;
		float		mMaxX;
		float		mMaxY;
		float		mMinX;
		float		mMinY;
		ci::Color	mLabelColor;
		ci::Color	mGridColor;
		bool		mVisible;

	} mConfiguration;

	void			resetConfiguration();

private:
	AppGlobals&		mGlobals;
};

} // !namespace cieq

#endif // !CIEQ_INCLUDE_GRID_RENDERER_H_