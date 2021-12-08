/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file image_view.cpp
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/image_view.h"

namespace Sgl
{
    ImageView::ImageView(Image* image, bool preserveRatio)
        : m_Image(image) {}

    Component* ImageView::getHitComponent(int32_t x, int32_t y)
    {
        if (Sml::isPointInsideRectangle({x, y}, m_LayoutBounds))
        {
            return this;
        }

        return nullptr;
    }

    void ImageView::render(const Sml::Rectangle<int32_t>& targetRegion)
    {
        if (m_Image != nullptr)
        {
            Sml::Rectangle<int32_t> translatedRegion = targetRegion;
            translatedRegion.pos += {getLayoutX(), getLayoutY()};
            renderImage(m_Image, translatedRegion, getLayoutWidth(), getLayoutHeight());
        }
    }

    void ImageView::layout() {}
    void ImageView::prerender() {}

    bool ImageView::isResizable() const { return true; }

    const Image* ImageView::getImage() const { return m_Image; }
    void ImageView::setImage(const Image* image) { m_Image = image; }

    float ImageView::getRatio() const
    {
        return getImage()->getHeight() == 0 ? 0 : getImage()->getWidth() / getImage()->getHeight();
    }

    bool ImageView::getPreserveRatio() const { return m_PreserveRatio; }
    void ImageView::setPreserveRatio(bool preserveRatio) { m_PreserveRatio = preserveRatio; }

    int32_t ImageView::getFitWidth() const { return m_FitWidth; }
    void ImageView::setFitWidth(int32_t width) { m_FitWidth = width; }

    int32_t ImageView::getFitHeight() const { return m_FitHeight; }
    void ImageView::setFitHeight(int32_t height) { m_FitHeight = height; }

    int32_t getWidthFixedRatio(int32_t height, float ratio)
    {
        assert(fabs(ratio) > FLT_EPSILON);

        return height / ratio;
    }

    int32_t getHeightFixedRatio(int32_t width, float ratio)
    {
        return width * ratio;
    }

    int32_t ImageView::computePrefWidth(int32_t height) const
    {
        if (m_Image == nullptr) { return 0; }

        if (!getPreserveRatio() || height == -1)
        {
            return (m_FitWidth != USE_COMPUTED_SIZE) ? m_FitWidth : getImage()->getWidth();
        }
        
        return (m_FitWidth != USE_COMPUTED_SIZE) ? m_FitWidth : getWidthFixedRatio(height, getRatio());
    }

    int32_t ImageView::computePrefHeight(int32_t width) const
    {
        if (m_Image == nullptr) { return 0; }

        if (!getPreserveRatio() || width == -1)
        {
            return (m_FitHeight != USE_COMPUTED_SIZE) ? m_FitHeight : getImage()->getHeight();
        }
        
        return (m_FitHeight != USE_COMPUTED_SIZE) ? m_FitHeight : getHeightFixedRatio(width, getRatio());
    }
    
    int32_t ImageView::computeMinWidth(int32_t height) const { return 0; }
    int32_t ImageView::computeMinHeight(int32_t width) const { return 0; }

    int32_t ImageView::computeMaxWidth(int32_t height) const { return UNLIMITED_SIZE; }
    int32_t ImageView::computeMaxHeight(int32_t width) const { return UNLIMITED_SIZE; }
}