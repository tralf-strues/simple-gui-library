/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file layered_container.h
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "container.h"

namespace Sgl
{
    class LayeredContainer : public Container
    {
    public:
        LayeredContainer();

        bool isAutoAdjust() const;
        void setAutoAdjust(bool autoAdjust);

        void moveToFront(Component* child);
        void moveToBack(Component* child);

        Component* getFront();
        Component* getBack();

    private:
        bool m_AutoAdjust = false;
    };
}