//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file pane.h
//! @date 2021-10-19
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef PANE_H
#define PANE_H

#include "container.h"

//------------------------------------------------------------------------------
//! @brief Simplest <code>Container</code> that lays out components in the exact
//!        same way they are specified (min/max/pref sizes are ignored!).
//------------------------------------------------------------------------------
class Pane : protected Container
{
public:
    /* Component */
    virtual void draw() override;

    /* Container */
    virtual const Dimension& getPrefferedLayoutSize() override;
    virtual const Dimension& getMinLayoutSize() override;

private:
    /* Container */
    virtual void layOutComponents() override;
};

#endif