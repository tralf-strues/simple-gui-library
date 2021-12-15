/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file slider.h
 * @date 2021-11-28
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "control.h"
#include "../containers/box_container.h"
#include "../shapes/text.h"

namespace Sgl
{
    class Slider : public Control
    {
    public:
        Slider(float rangeMin, float rangeMax);
        Slider(BaseSkin<Slider>* skin, float rangeMin, float rangeMax);

        Orientation getOrientation() const;
        void setOrientation(Orientation orientation);

        float getRangeMin() const;
        void setRangeMin(float min);

        float getRangeMax() const;
        void setRangeMax(float max);

        float getValue() const;
        void setValue(float value);

        void addOnPropertyChange(Sml::PropertyChangeListener<float>* listener);
        const std::list<Sml::PropertyChangeListener<float>*>& getOnPropertyChange(); // TODO: get rid of

    private:
        Orientation m_Orientation = Orientation::HORIZONTAL;

        float       m_RangeMin    = 0;
        float       m_RangeMax    = 0;
        float       m_Value       = 0;

        std::list<Sml::PropertyChangeListener<float>*> m_PropertyChangeListeners;
    };

    class SliderWithLabel : public HBox
    {
    public:
        static const int32_t DEFAULT_SPACING;

    public:
        SliderWithLabel(float rangeMin, float rangeMax, const char* format = "%lg");
        SliderWithLabel(BaseSkin<Slider>* skin, float rangeMin, float rangeMax, const char* format = "%lg");

        Slider* getSlider();
        Text* getLabel();

    private:
        Slider*     m_Slider = nullptr;
        Text*       m_Label  = nullptr;
        const char* m_Format = nullptr;

        void attach();
    };
}