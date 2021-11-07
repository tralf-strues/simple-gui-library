/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file control.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "../parent.h"
#include "../style/skin.h"

namespace Sgl
{
    class Control : public Parent, public Skinnable
    {
    public:
        Control() = default;
        virtual ~Control() override = default;

    protected:
        
    };
}