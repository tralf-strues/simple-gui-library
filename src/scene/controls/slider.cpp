/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file slider.cpp
 * @date 2021-11-28
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/controls/slider.h"
#include "scene/style/default_skins.h"

namespace Sgl
{
    Slider::Slider(float rangeMin, float rangeMax) : m_RangeMin(rangeMin), m_RangeMax(rangeMax), m_Value(rangeMin)
    {
        assert(rangeMin < rangeMax);

        m_DefaultSkin = new DefaultSkins::SliderSkin(this);
        setSkin(m_DefaultSkin);
    }

    Slider::Slider(BaseSkin<Slider>* skin, float rangeMin, float rangeMax)
        : m_RangeMin(rangeMin), m_RangeMax(rangeMax), m_Value(rangeMin)
    {
        assert(skin);
        skin->attach(this);
        setSkin(skin);
    }

    float Slider::getRangeMin() const { return m_RangeMin; }
    float Slider::getRangeMax() const { return m_RangeMax; }
    float Slider::getValue()    const { return m_Value;    }

    void Slider::setRangeMin(float min)
    {
        assert(min < m_RangeMax);
        m_RangeMin = min;

        if (m_Value < m_RangeMin)
        {
            setValue(m_RangeMin);
        }
    }

    void Slider::setRangeMax(float max)
    {
        assert(m_RangeMin < max);
        m_RangeMax = max;

        if (m_Value > m_RangeMax)
        {
            setValue(m_RangeMax);
        }
    }

    void Slider::setValue(float value)
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

    void Slider::setOnPropertyChange(Sml::PropertyChangeListener<float>* listener)
    {
        m_PropertyChangeListener = listener;
    }

    Sml::PropertyChangeListener<float>* Slider::getOnPropertyChange()
    {
        return m_PropertyChangeListener;
    }
}