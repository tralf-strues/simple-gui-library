/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file image_view.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../component.h"
#include "image.h"

namespace Sgl
{
    class ImageView final : public Component
    {
    public:
        ImageView(Image* image = nullptr, bool preserveRatio = false);
        virtual ~ImageView() override = default;

        virtual Component* getHitComponent(int32_t x, int32_t y) override;
        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) override;
        virtual void layout() override;
        virtual void prerender() override;
        virtual bool isResizable() const override;

        const Image* getImage() const;
        void setImage(const Image* image);

        // TODO:
        // bool getPreserveRatio() const;
        // void setPreserveRatio(bool preserveRatio);

        int32_t getFitWidth() const;
        void setFitWidth(int32_t width);

        int32_t getFitHeight() const;
        void setFitHeight(int32_t height);

        virtual int32_t computePrefWidth(int32_t height = -1) const override;
        virtual int32_t computePrefHeight(int32_t width = -1) const override;
        
        virtual int32_t computeMinWidth(int32_t height = -1) const override;
        virtual int32_t computeMinHeight(int32_t width = -1) const override;

        virtual int32_t computeMaxWidth(int32_t height = -1) const override;
        virtual int32_t computeMaxHeight(int32_t width = -1) const override;

    private:
        const Image* m_Image     = nullptr;

        int32_t      m_FitWidth  = USE_COMPUTED_SIZE;
        int32_t      m_FitHeight = USE_COMPUTED_SIZE;
    };
};