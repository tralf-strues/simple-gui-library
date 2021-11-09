/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file control.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../parent.h"
#include "../style/skin.h"

namespace Sgl
{
    class Control : public Parent, public Skinnable
    {
    public:
        Control() = default;
        virtual ~Control() override;

        virtual Component* getHitComponent(int32_t x, int32_t y) override;

        virtual int32_t computePrefWidth(int32_t height = -1) const override;
        virtual int32_t computePrefHeight(int32_t width = -1) const override;
        
        virtual int32_t computeMinWidth(int32_t height = -1) const override;
        virtual int32_t computeMinHeight(int32_t width = -1) const override;

        virtual int32_t computeMaxWidth(int32_t height = -1) const override;
        virtual int32_t computeMaxHeight(int32_t width = -1) const override;

    protected:
        Skin* m_DefaultSkin = nullptr; // Supposed to be allocated dynamically.

        virtual void layoutChildren() override final;
        virtual void prerenderSelf() override final;
    };
}