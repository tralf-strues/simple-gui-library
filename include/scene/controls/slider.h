/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file slider.h
 * @date 2021-11-28
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "control.h"

namespace Sgl
{
    class Slider : public Control
    {
    public:
        Slider(float rangeMin, float rangeMax);
        Slider(BaseSkin<Slider>* skin, float rangeMin, float rangeMax);

        float getRangeMin() const;
        void setRangeMin(float min);

        float getRangeMax() const;
        void setRangeMax(float max);

        float getValue() const;
        void setValue(float value);

        void setOnPropertyChange(Sml::PropertyChangeListener<float>* listener);
        Sml::PropertyChangeListener<float>* getOnPropertyChange(); // TODO: get rid of

    private:
        float m_RangeMin = 0;
        float m_RangeMax = 0;
        float m_Value    = 0;

        Sml::PropertyChangeListener<float>* m_PropertyChangeListener = nullptr;
    };
}