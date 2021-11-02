//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file spaced_container.cpp
//! @date 2021-10-31
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <tgmath.h>
#include "containers/spaced_container.h"

void SpacedContainer::insertSpacerAfter(Component* component, uint32_t weight)
{
    assert(component);
    
    if (weight == 0) { return; }

    m_Spacers.emplace_back(component, weight);
}

void SpacedContainer::addSpacer(uint32_t weight)
{
    if (weight == 0) { return; }

    m_Spacers.emplace_back(m_Components.back(), weight);
}

void SpacedContainer::pushFrontSpacer(uint32_t weight)
{
    if (weight == 0) { return; }

    m_Spacers.emplace_back(nullptr, weight);
}

void SpacedContainer::layOutComponents()
{
    Container::layOutComponents();

    int32_t  totalComponentsWidth = getTotalWidth();
    int32_t  totalSpacersWidth    = getWidth() - totalComponentsWidth;
    uint32_t totalWeight          = getTotalWeight();

    // FIXME: Handle the situation somehow else.
    assert(totalComponentsWidth <= getWidth());
    
    int32_t curX = getSpacerWidth(nullptr, totalWeight, totalSpacersWidth);

    for (Component* component : m_Components)
    {
        component->setX(curX);
        curX += component->getWidth() + getSpacerWidth(component, totalWeight, totalSpacersWidth);

        // if (maxHeight < component->getY() + component->getHeight())
        // {
            // maxHeight = component->getY() + component->getHeight();
        // }
    }

    // setHeight(maxHeight);
    // setMinSize(getMinSize().x, maxHeight);
}

// Vec2<int32_t> SpacedContainer::getPrefferedLayoutSize()
// {
//     Vec2<int32_t> totalPrefferedLayoutSize{0, 0};
//     for (Component* component : m_Components)
//     {
//         if (component->isPrefSizeSet())
//         {
//             totalPrefferedLayoutSize.x += component->getPrefSize().x;
            
//             if (component->getPrefSize().y > totalPrefferedLayoutSize.y)
//             {
//                 totalPrefferedLayoutSize.y = component->getPrefSize().y;
//             }
//         }
//     }

//     return totalPrefferedLayoutSize;
// }

// Vec2<int32_t> SpacedContainer::getMinLayoutSize()
// {
//     Vec2<int32_t> totalMinLayoutSize{0, 0};
//     for (Component* component : m_Components)
//     {
//         if (component->isMinSizeSet())
//         {
//             totalMinLayoutSize.x += component->getMinSize().x;
            
//             if (component->getMinSize().y > totalMinLayoutSize.y)
//             {
//                 totalMinLayoutSize.y = component->getMinSize().y;
//             }
//         }
//     }

//     return totalMinLayoutSize;
// }

int32_t SpacedContainer::getSpacerWidth(Component* prev, int32_t totalWeight, int32_t totalSpacersWidth)
{
    int32_t spacerWidth = 0;
    Spacer curSpacer{};

    if (getSpacer(prev, &curSpacer))
    {
        spacerWidth = weightToWidth(curSpacer, totalWeight, totalSpacersWidth);
    }

    return spacerWidth;
}

bool SpacedContainer::getSpacer(Component* prev, Spacer* spacer)
{
    assert(spacer);

    for (Spacer curSpacer : m_Spacers)
    {
        if (curSpacer.prevComponent == prev)
        {
            *spacer = curSpacer;
            return true;
        }
    }

    return false;
}

int32_t SpacedContainer::weightToWidth(const Spacer& spacer, int32_t totalWeight, int32_t totalSpacersWidth)
{
    assert(totalWeight != 0);
    return std::roundf((static_cast<float>(spacer.weight) / static_cast<float>(totalWeight)) * 
                       totalSpacersWidth);
}

uint32_t SpacedContainer::getTotalWeight()
{
    uint32_t totalWeight = 0;

    for (Spacer spacer : m_Spacers)
    {
        totalWeight += spacer.weight;
    }

    return totalWeight;
}

int32_t SpacedContainer::getTotalWidth()
{
    int32_t totalWidth = 0;

    for (Component* component : m_Components)
    {
        totalWidth += component->getWidth();
    }

    return totalWidth;
}