/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file utils.h
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

/**
 * type      = {Pref, Min, Max}
 * dimension = {Width, Height}
 * axis      = {X, Y}
 * children  = children container
 */
#define COMPONENT_COMPUTE_DIMENSION(type, dimension, dependency, axis, children, enabledValue, addition)  \
    if (enabledValue != Sgl::Component::USE_COMPUTED_SIZE) { return enabledValue; } \
                                                                                    \
    int32_t min##axis = INT32_MAX;                                                  \
    int32_t max##axis = 0;                                                          \
    for (const Component* child : children)                                         \
    {                                                                               \
        int32_t child##axis      = child->getLayout##axis();                        \
        int32_t child##dimension = child->compute##type##dimension(dependency);     \
                                                                                    \
        if (child##dimension == Sgl::Component::UNLIMITED_SIZE)                     \
        {                                                                           \
            return Sgl::Component::UNLIMITED_SIZE;                                  \
        }                                                                           \
                                                                                    \
        min##axis = std::min(min##axis, child##axis);                               \
        max##axis = std::max(max##axis, child##axis + child##dimension);            \
    }                                                                               \
                                                                                    \
    if (min##axis != INT32_MAX)                                                     \
    {                                                                               \
        return max##axis - min##axis + addition;                                    \
    }                                                                               \
                                                                                    \
    return addition;