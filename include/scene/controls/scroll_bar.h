/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scroll_bar.h
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "control.h"

namespace Sgl
{
    class ScrollBar : public Control
    {
    public:
        ScrollBar(Orientation orientation, float rangeMin, float rangeMax);

        Orientation getOrientation() const;

        float getRangeMin() const;
        void setRangeMin(float min);

        float getRangeMax() const;
        void setRangeMax(float max);

        float getValue() const;
        void setValue(float value);

        float getIncrement() const;
        void setIncrement(float increment);

        float getVisibleRange() const;
        void setVisibleRange(float visibleRange);

        void setOnPropertyChange(Sml::PropertyChangeListener<float>* listener);
        Sml::PropertyChangeListener<float>* getOnPropertyChange(); // TODO: get rid of

    private:
        const Orientation m_Orientation;
        
        float       m_RangeMin     = 0;
        float       m_RangeMax     = 0;
        float       m_Value        = 0;

        float       m_Increment    = 0;
        float       m_VisibleRange = 0;

        Sml::PropertyChangeListener<float>* m_PropertyChangeListener = nullptr;
    };
};