/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file layered_container.cpp
 * @date 2021-12-12
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/containers/layered_container.h"

using namespace Sgl;

class FocusReceivedFilter : public FocusListener<LayeredContainer>
{
public:
    FocusReceivedFilter(LayeredContainer* component) : FocusListener<LayeredContainer>(component) {}

    virtual void onFocusReceived(FocusReceivedEvent* event) override
    {
        if (!getComponent()->isAutoAdjust())
        {
            return;
        }

        for (auto child : getComponent()->getChildren())
        {
            if (child->containsComponent(event->getNewFocus()))
            {
                getComponent()->moveToFront(child);
                break;
            }
        }
    }
};

LayeredContainer::LayeredContainer()
{
    getEventDispatcher()->attachFilter(FocusReceivedFilter::EVENT_TYPES, new FocusReceivedFilter(this));
}

bool LayeredContainer::isAutoAdjust() const { return m_AutoAdjust; }
void LayeredContainer::setAutoAdjust(bool autoAdjust) { m_AutoAdjust = autoAdjust; }

void LayeredContainer::moveToFront(Component* child)
{
    assert(child);

    for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
    {
        if (*it == child)
        {
            m_Children.erase(it);
            m_Children.push_back(child);
            break;
        }
    }
}

void LayeredContainer::moveToBack(Component* child)
{
    assert(child);

    for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
    {
        if (*it == child)
        {
            m_Children.erase(it);
            m_Children.push_front(child);
            break;
        }
    }
}

Component* LayeredContainer::getFront()
{
    return m_Children.empty() ? nullptr : m_Children.back();
}

Component* LayeredContainer::getBack()
{
    return m_Children.empty() ? nullptr : m_Children.front();
}