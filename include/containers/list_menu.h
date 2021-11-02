//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file list_menu.h
//! @date 2021-10-31
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef LIST_MENU_H
#define LIST_MENU_H

#include "../container.h"
#include "style/default_skins.h"

class ListMenu : public Container
{
public:
    enum class Orientation
    {
        VERTICAL,
        HORIZONTAL
    };

    ListMenu() {}
    ListMenu(Renderer* renderer) : Container(renderer) { setDefaultStyle(); }
    virtual ~ListMenu() override = default;

    Orientation getOrientation() const;
    void setOrientation(Orientation orientation);

    void setIndent(int32_t indent);  
    int32_t getIndent() const;

    void setSpacing(int32_t spacing);
    int32_t getSpacing() const;

private:
    Orientation m_Orientation = Orientation::VERTICAL;
    int32_t     m_Indent      = DEFAULT_LIST_MENU_INDENT;
    int32_t     m_Spacing     = DEFAULT_LIST_MENU_SPACING;

    virtual void setDefaultStyle() override;
    virtual void layOutComponents() override;
};

#endif // LIST_MENU_H