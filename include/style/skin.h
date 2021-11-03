//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file skin.h
//! @date 2021-10-29
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SKIN_H
#define SKIN_H

#include "sml/containers/dynamic_array.h"
#include "sml/graphics_wrapper/renderer.h"

template<typename C>
class Skin
{
public:
    Skin(C* component) : m_Component(component) { assert(component); };

    //--------------------------------------------------------------------------
    //! @brief Should be called by @ref Skinnable when this skin is replaced.
    //--------------------------------------------------------------------------
    virtual void dispose() {}

    virtual void apply() = 0;

    C* getSkinnable() { return m_Component; }

    virtual int32_t computePrefHeight() const { return 0; };
    virtual int32_t computePrefWidth()  const { return 0; };
    virtual void    layoutChildren()    const {};

protected:
    C* m_Component = nullptr;
};

template<typename C>
class Skinnable
{
public:
    const Skin<C>* getSkin() const
    {
        return m_Skin;
    }

    void setSkin(Skin<C>* skin)
    {
        if (m_Skin != nullptr)
        {
            m_Skin->dispose();
        }

        m_Skin = skin;
    }

    void applySkin()
    {
        if (m_Skin != nullptr)
        {
            m_Skin->apply();
        }
    }

protected:
    Skin<C>* m_Skin = nullptr;
};

// class IconSkin : public Skin
// {
// public:
//     IconSkin(Texture* icon) : m_Icon(icon) {}

//     virtual void apply(Renderer* renderer, Texture* target,
//                        const Rectangle<int32_t>* targetRegion) const override;

// private:
//     Texture* m_Icon = nullptr;
// };

// class BasicSkin : public Skin
// {
// public:
//     BasicSkin() = default;
//     BasicSkin(bool backgroundEnabled, Color background,
//               bool borderEnabled, Color borderColor, int32_t borderThickness);

//     virtual void apply(Renderer* renderer, Texture* target,
//                        const Rectangle<int32_t>* targetRegion) const override;

//     /* Getters */
//     bool isBackgroundEnabled() const;
//     Color getBackgroundColor() const;
    
//     bool isBorderEnabled() const;
//     Color getBorderColor() const;
//     int32_t getBorderThickness() const;

//     /* Setters */
//     void setBackgroundEnabled(bool enabled);
//     void setBackgroundColor(Color color);
    
//     void setBorderEnabled(bool enabled);
//     void setBorderColor(Color color);
//     void setBorderThickness(int32_t thickness);

// protected:
//     /* Background */
//     bool  m_BackgroundEnabled = false;
//     Color m_BackgroundColor   = COLOR_TRANSPARENT;

//     /* Border */
//     bool  m_BorderEnabled     = false;
//     Color m_BorderColor       = COLOR_BLACK;
//     int32_t m_BorderThickness = 1;

//     void renderBackground(Renderer* renderer, Texture* target, const Rectangle<int32_t>& region) const;
//     void renderBorder(Renderer* renderer, Texture* target, const Rectangle<int32_t>& region) const;
// };

// struct Patch
// {
//     enum PatchApplyType
//     {
//         NO_REPEAT,
//         FIXED_REPEAT_COUNT,
//         ADJUSTING
//     };

//     Vec2<int32_t>  absoluteSize     = {0, 0};

//     bool           useRelativeSize  = false;
//     Vec2<float>    relativeSize     = {0.0f, 0.0f};

//     PatchApplyType applyType        = NO_REPEAT;
//     float          repeatCount      = 0.0f;
    
//     Texture*       source           = nullptr;
//     Rectangle<int32_t> sourceRegion = {{0, 0}, 0, 0};
// };

// FIXME: Make support for vertical patch strip!
// class HorizontalPatchStrip : public Skin
// {
// public:
//     DynamicArray<Patch>* getPatches();

//     void appendPatch(const Patch& patch);
//     void apply(Texture* target, const Rectangle<int32_t>* targetRegion);

// protected:
//     DynamicArray<Patch> m_Patches;
// };

#endif // SKIN_H