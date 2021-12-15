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

    Orientation Slider::getOrientation() const { return m_Orientation; }
    void Slider::setOrientation(Orientation orientation) { m_Orientation = orientation; }

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

        if (m_Value == oldValue)
        {
            return;
        }

        for (auto listener : m_PropertyChangeListeners)
        {
            listener->onPropertyChange(new Sml::PropertyChangeEvent<float>(oldValue, m_Value));
        }
    }

    void Slider::addOnPropertyChange(Sml::PropertyChangeListener<float>* listener)
    {
        m_PropertyChangeListeners.push_back(listener);
    }

    const std::list<Sml::PropertyChangeListener<float>*>& Slider::getOnPropertyChange()
    {
        return m_PropertyChangeListeners;
    }

    const int32_t SliderWithLabel::DEFAULT_SPACING = 5;

    SliderWithLabel::SliderWithLabel(float rangeMin, float rangeMax, const char* format)
        : m_Slider(new Slider(rangeMin, rangeMax)), m_Label(new Text("0")), m_Format(format) { attach(); }

    SliderWithLabel::SliderWithLabel(BaseSkin<Slider>* skin, float rangeMin, float rangeMax, const char* format)
        : m_Slider(new Slider(skin, rangeMin, rangeMax)), m_Label(new Text("0")), m_Format(format) { attach(); }

    Slider* SliderWithLabel::getSlider() { return m_Slider; }
    Text* SliderWithLabel::getLabel() { return m_Label; }

    void SliderWithLabel::attach()
    {
        setBackground(nullptr);

        addChildren(m_Slider, m_Label);
        setSpacing(DEFAULT_SPACING);
        setGrowPriority(m_Slider, GrowPriority::ALWAYS);

        class SliderHandler : public Sml::PropertyChangeListener<float>
        {
        public:
            SliderHandler(Text* label, const char* format) : m_Label(label), m_Format(format) {}

            virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
            {
                snprintf(m_Str, sizeof(m_Str), m_Format, event->getNewValue());
                m_Label->setString(m_Str);
            }

        private:
            Text*       m_Label;
            const char* m_Format;
            char        m_Str[128];
        };

        m_Slider->addOnPropertyChange(new SliderHandler(m_Label, m_Format));
    }
}