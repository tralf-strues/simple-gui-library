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
    class ImageView : public Component
    {
    public:
        ImageView(Image* image = nullptr);
        virtual ~ImageView() override = default;

        virtual Component* getHitComponent(int32_t x, int32_t y) override;
        virtual void render(Sml::Texture* target, const Sml::Rectangle<int32_t>& targetRegion) override;

        Image* getImage();
        void setImage(Image* image);

    private:
        Image* m_Image = nullptr;
    };
};