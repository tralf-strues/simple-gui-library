/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file core.h
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <stdio.h>
#include "sml/graphics_wrapper/renderer.h"

#define LOG_INFO(format, ...) fprintf(stdout, "[INFO ] " format "\n", ##__VA_ARGS__)
#define LOG_ERROR(format, ...) fprintf(stderr, "[ERROR] " format "\n", ##__VA_ARGS__)

namespace Sgl
{
    extern Sml::Renderer* g_ContextRenderer;

    inline Sml::Renderer* getContextRenderer()
    {
        assert(Sgl::g_ContextRenderer);
        return Sgl::g_ContextRenderer;
    }

    inline void setContextRenderer(Sml::Renderer* renderer)
    {
        Sgl::g_ContextRenderer = renderer;
    }
}
