// /**
//  * @author Nikita Mochalov (github.com/tralf-strues)
//  * @file hbox.h
//  * @date 2021-11-09
//  * 
//  * @copyright Copyright (c) 2021
//  */

// #pragma once

// #include "container.h"

// namespace Sgl
// {
//     class HBox : public Container
//     {
//     public:
//         HBox() = default;
//         virtual ~HBox() override = default;

//         int32_t getMargin() const;
//         void setMargin(int32_t margin);

//         virtual int32_t computePrefWidth(int32_t height = -1) const override;
//         virtual int32_t computePrefHeight(int32_t width = -1) const override;
        
//         virtual int32_t computeMinWidth(int32_t height = -1) const override;
//         virtual int32_t computeMinHeight(int32_t width = -1) const override;     

//     private:
//         int32_t m_Margin = 0;

//         virtual void layoutChildren();
//     };
// }