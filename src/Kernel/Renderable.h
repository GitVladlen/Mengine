#pragma once

#include "Kernel/Mixin.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class RenderInterface;
    //////////////////////////////////////////////////////////////////////////
    class Renderable
        : public Mixin
    {
    public:
        virtual RenderInterface * getRender()
        {
            return nullptr;
        }

        virtual const RenderInterface * getRenderConst() const
        {
            return nullptr;
        }
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Renderable> RenderablePtr;
    //////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////
#define DECLARE_RENDERABLE()\
public:\
    Mengine::RenderInterface * getRender() override{ return this; }\
    const Mengine::RenderInterface * getRenderConst() const override{ return this; }\
protected:
