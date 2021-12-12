/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scroll_bar.cpp
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/controls/scroll_bar.h"
#include "scene/style/default_skins.h"

using namespace Sgl;

ScrollBar::ScrollBar(Orientation orientation, float rangeMin, float rangeMax)
    : m_Orientation(orientation), m_RangeMin(rangeMin), m_RangeMax(rangeMax)
{
    m_DefaultSkin = new DefaultSkins::ScrollBarSkin(this);
    setSkin(m_DefaultSkin);
}

Orientation ScrollBar::getOrientation() const { return m_Orientation; }

float ScrollBar::getRangeMin() const { return m_RangeMin; }

void ScrollBar::setRangeMin(float min)
{
    assert(min < m_RangeMax);
    m_RangeMin = min;

    if (m_Value < m_RangeMin)
    {
        setValue(m_RangeMin);
    }
}

float ScrollBar::getRangeMax() const { return m_RangeMax; }

void ScrollBar::setRangeMax(float max)
{
    assert(m_RangeMin < max);
    m_RangeMax = max;

    if (m_Value > m_RangeMax)
    {
        setValue(m_RangeMax);
    }
}

float ScrollBar::getValue() const { return m_Value; }

void ScrollBar::setValue(float value)
{
    float oldValue = m_Value;

    m_Value = value;
    m_Value = std::max(m_Value, m_RangeMin);
    m_Value = std::min(m_Value, m_RangeMax);

    if (getOnPropertyChange() != nullptr && m_Value != oldValue)
    {
        getOnPropertyChange()->onPropertyChange(new Sml::PropertyChangeEvent<float>(oldValue, m_Value));
    }
}

float ScrollBar::getIncrement() const { return m_Increment; }
void ScrollBar::setIncrement(float increment) { m_Increment = increment; }

float ScrollBar::getVisibleRange() const { return m_VisibleRange; }
void ScrollBar::setVisibleRange(float visibleRange) { m_VisibleRange = visibleRange; }

void ScrollBar::setOnPropertyChange(Sml::PropertyChangeListener<float>* listener)
{
    m_PropertyChangeListener = listener;
}

Sml::PropertyChangeListener<float>* ScrollBar::getOnPropertyChange() { return m_PropertyChangeListener; }