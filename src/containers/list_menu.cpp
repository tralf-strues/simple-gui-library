//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file list_menu.cpp
//! @date 2021-10-31
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "containers/list_menu.h"

ListMenu::Orientation ListMenu::getOrientation() const
{
    return m_Orientation;
}

void ListMenu::setOrientation(Orientation orientation)
{
    m_Orientation = orientation;
}

void ListMenu::setIndent(int32_t indent)
{
    m_Indent = indent;
}

int32_t ListMenu::getIndent() const
{
    return m_Indent;
}

void ListMenu::setSpacing(int32_t spacing)
{
    m_Spacing = spacing;
}

int32_t ListMenu::getSpacing() const
{
    return m_Spacing;
}

void ListMenu::setDefaultStyle()
{
    Container::setDefaultStyle();
    setSkin(&DEFAULT_SKIN_LIST_MENU);
}

// TODO: Get rid of copy-paste code!
void ListMenu::layOutComponents()
{
    Container::layOutComponents();

    if (m_Orientation == Orientation::VERTICAL)
    {
        int32_t curY     = getSpacing();
        int32_t maxWidth = 0;

        for (Component* component : m_Components)
        {
            if (component->getWidth() > maxWidth)
            {
                maxWidth = component->getWidth();
            }
        }

        if (maxWidth + 2 * getIndent() < getWidth())
        {
            maxWidth = getWidth() - 2 * getIndent();
        }

        for (Component* component : m_Components)
        {
            component->setX(getIndent());
            component->setY(curY);
            component->setWidth(maxWidth);

            // if (component->isPrefSizeSet())
            // {
                // component->setSize(maxWidth, component->getPrefSize().y);
            // }

            curY += component->getHeight() + getSpacing();
        }

        setSize(maxWidth + 2 * getIndent(), curY);
        // setPrefSize(maxWidth + 2 * getIndent(), curY);
    }
    else if (m_Orientation == Orientation::HORIZONTAL)
    {
        int32_t curX      = getSpacing();
        int32_t maxHeight = 0;

        for (Component* component : m_Components)
        {
            if (component->getHeight() > maxHeight)
            {
                maxHeight = component->getHeight();
            }
        }

        if (maxHeight + 2 * getIndent() < getHeight())
        {
            maxHeight = getHeight() - 2 * getIndent();
        }

        for (Component* component : m_Components)
        {
            component->setX(curX);
            component->setY(getIndent());
            component->setHeight(maxHeight);

            // if (component->isPrefSizeSet())
            // {
                // component->setSize(component->getPrefSize().x, maxHeight);
            // }

            curX += component->getWidth() + getSpacing();
        }

        // setPrefSize(curX, maxHeight + 2 * getIndent());
        setSize(curX, maxHeight + 2 * getIndent());
    }

    // setSize(getPrefSize()); // FIXME:
}