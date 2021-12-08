/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file fill.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include <algorithm>
#include "sml/graphics_wrapper/primitives.h"
#include "paint/fill.h"

using namespace Sgl;

//------------------------------------------------------------------------------
// ColorFill
//------------------------------------------------------------------------------
ColorFill::ColorFill(Sml::Color color)
    : m_Color(color) {}

void ColorFill::fillLine(const Sml::Vec2i& start, const Sml::Vec2i& end,
                            const Sml::Rectangle<int32_t>& targetRegion) const
{
    Sml::Renderer::getInstance().setColor(m_Color);

    Sml::Vec2i translatedStart = start + targetRegion.pos;
    Sml::Vec2i translatedEnd   = end + targetRegion.pos;

    Sml::renderLine(translatedStart, translatedEnd);
}

void ColorFill::fillArea(const Sml::Rectangle<int32_t>& area,
                            const Sml::Rectangle<int32_t>& targetRegion) const
{
    Sml::Renderer::getInstance().setColor(m_Color);

    Sml::Rectangle<int32_t> translatedArea = area;
    translatedArea.pos += targetRegion.pos;

    Sml::renderFilledRect(translatedArea);
}

void ColorFill::fillPoint(const Sml::Vec2i& point,
                            const Sml::Rectangle<int32_t>& targetRegion) const
{
    Sml::Renderer::getInstance().setColor(m_Color);

    Sml::Vec2i translatedPoint = point + targetRegion.pos;
    Sml::renderPoint(translatedPoint);
}

//------------------------------------------------------------------------------
// LinearGradientFill->Stop
//------------------------------------------------------------------------------
LinearGradientFill::Stop::Stop(float offset, Sml::Color color)
    : m_Offset(offset), m_Color(Sml::colorToNormalized(color)) {}

float LinearGradientFill::Stop::getOffset() const { return m_Offset; }
const Sml::Vec4f& LinearGradientFill::Stop::getColorVector() const { return m_Color; }
Sml::Color LinearGradientFill::Stop::getColor() const { return Sml::colorFromNormalized(m_Color); }

bool LinearGradientFill::Stop::operator<(const LinearGradientFill::Stop& other) const
{
    return getOffset() < other.getOffset();
}

//------------------------------------------------------------------------------
// LinearGradientFill
//------------------------------------------------------------------------------
LinearGradientFill::LinearGradientFill(const std::initializer_list<Stop>& stops)
{
    m_Stops.insert(m_Stops.end(), stops);
    std::sort(m_Stops.begin(), m_Stops.end());
}

void LinearGradientFill::addStop(const Stop& stop)
{
    m_Stops.push_back(stop);
    std::sort(m_Stops.begin(), m_Stops.end());
}

void LinearGradientFill::setVertical()
{
    m_IsVertical = true;
}

void LinearGradientFill::setHorizontal()
{
    m_IsVertical = false;
}

void LinearGradientFill::fillLine(const Sml::Vec2i& start,
                                  const Sml::Vec2i& end,
                                  const Sml::Rectangle<int32_t>& targetRegion) const
{
    if (m_Stops.size() < 2 || start == end) { return; }

    int32_t x0 = (int32_t) start.x;
	int32_t y0 = (int32_t) start.y;
	int32_t x1 = (int32_t) end.x;
	int32_t y1 = (int32_t) end.y;

	int32_t dx = std::abs(x1 - x0);
	int32_t dy = std::abs(y1 - y0);
	int32_t sx = (x0 < x1) ? 1 : -1;
	int32_t sy = (y0 < y1) ? 1 : -1;
	int32_t err = dx - dy;

    float length = Sml::length(end - start);

    int32_t prevStop = -1;
    int32_t nextStop = 0;
    int32_t stops    = m_Stops.size();

	while (true)
    {
		float offset = Sml::length(Sml::Vec2i(x0, y0) - start) / length;

        if (nextStop != stops && offset >= m_Stops[nextStop].getOffset() && (nextStop + 1) != stops)
        {
            ++prevStop;
            ++nextStop;
        }

        if (prevStop == -1 && nextStop != stops)
        {
            continue;
        }

        if (prevStop == stops || nextStop == stops)
        {
            break;
        }

        float alpha = (offset - m_Stops[prevStop].getOffset()) / (m_Stops[nextStop].getOffset() - m_Stops[prevStop].getOffset());
        Sml::Vec4f color = (1 - alpha) * m_Stops[prevStop].getColorVector() + alpha * m_Stops[nextStop].getColorVector();
        
        Sml::Renderer::getInstance().setColor(Sml::colorFromNormalized(color));
        Sml::renderPoint(Sml::Vec2i(x0, y0) + targetRegion.pos);

		if ((x0 == x1) && (y0 == y1))
        {
			break;
        }

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}

		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void LinearGradientFill::fillArea(const Sml::Rectangle<int32_t>& area,
                                  const Sml::Rectangle<int32_t>& targetRegion) const
{
    if (m_Stops.size() < 2) { return; }

    if (!m_IsVertical)
    {
        for (int32_t row = 0; row < area.height; ++row)
        {
            fillLine(Sml::Vec2i(area.pos.x, area.pos.y + row),
                     Sml::Vec2i(area.pos.x + area.width - 1, area.pos.y + row),
                     targetRegion);
        }
    }
    else
    {
        for (int32_t column = 0; column < area.width; ++column)
        {
            fillLine(Sml::Vec2i(area.pos.x + column, area.pos.y),
                     Sml::Vec2i(area.pos.x + column, area.pos.y + area.height - 1),
                     targetRegion);
        }
    }
}

void LinearGradientFill::fillPoint(const Sml::Vec2i& point,
                                   const Sml::Rectangle<int32_t>& targetRegion) const
{
    if (m_Stops.size() < 2) { return; }
}