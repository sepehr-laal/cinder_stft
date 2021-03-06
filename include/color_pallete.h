#ifndef CISTFT_INCLUDE_COLOR_PALETTE_H_
#define CISTFT_INCLUDE_COLOR_PALETTE_H_

#include <array>

#include <cinder/Color.h>

namespace cistft {
namespace palette {

struct MatlabJet			{ static const std::array<const ci::Color, 64> palette; };
struct MatlabHot			{ static const std::array<const ci::Color, 64> palette; };
struct MPLSummer			{ static const std::array<const ci::Color, 128> palette; };
struct MPLPaired			{ static const std::array<const ci::Color, 128> palette; };
struct MPLOcean				{ static const std::array<const ci::Color, 128> palette; };
struct MPLWinter			{ static const std::array<const ci::Color, 128> palette; };
struct OceanLakeLandSnow	{ static const std::array<const ci::Color, 254> palette; };
struct SVGBhw322			{ static const std::array<const ci::Color, 220> palette; };
struct MPLGnuplot			{ static const std::array<const ci::Color, 128> palette; };
struct MPLFlag				{ static const std::array<const ci::Color, 128> palette; };
struct NCVManga				{ static const std::array<const ci::Color, 256> palette; };
struct MPLPrism				{ static const std::array<const ci::Color, 128> palette; };
struct SVGLindaa07			{ static const std::array<const ci::Color, 220> palette; };
struct SVGGallet13			{ static const std::array<const ci::Color, 220> palette; };

template<typename T>
inline static const ci::Color& getColor(float value, float vmin, float vmax)
{
	float vd = vmax - vmin;

	// saturate if out of bounds
	if (value < vmin) value = vmin;
	if (value > vmax) value = vmax;

	return T::palette[static_cast<int>(((value - vmin) / vd) * (std::tuple_size<decltype(T::palette)>::value - 1))];
}

}} // !namespace cistft::palette

#endif // !CISTFT_INCLUDE_COLOR_PALETTE_H_