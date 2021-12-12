/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file skin.h
 * @date 2021-10-29
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/graphics_wrapper/renderer.h"
#include "../../paint/insets.h"

namespace Sgl
{
    class Control;
    class Component;

    class Skin
    {
    public:
        virtual ~Skin() = default;

        /**
         * @brief Should be called by @ref Control when this skin is replaced.
         * 
         * Must do things like removing its Components from the Control's
         * children list, detaching its Listeners from the Control's dispatcher.
         */
        virtual void dispose() = 0;

        virtual void prerenderControl();

        virtual Component* getHitComponent(int32_t x, int32_t y);
        virtual const Control* getControl() const = 0;
        virtual Control* getModifiableControl() = 0;

        virtual int32_t computePrefHeight(int32_t width = -1) const;
        virtual int32_t computePrefWidth(int32_t height = -1) const;

        virtual int32_t computeMinHeight(int32_t width = -1) const;
        virtual int32_t computeMinWidth(int32_t height = -1) const;

        virtual int32_t computeMaxHeight(int32_t width = -1) const;
        virtual int32_t computeMaxWidth(int32_t height = -1) const;

        virtual void layoutChildren();
    };

    template<typename C>
    class BaseSkin : public Skin
    {
    public:
        virtual void attach(C* control) = 0;
    };

    class Skinnable
    {
    public:
        Skin* getSkin();
        void setSkin(Skin* skin);

    protected:
        Skin* m_Skin = nullptr;
    };
}