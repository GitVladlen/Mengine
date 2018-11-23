#pragma once

#include "Interface/PluginInterface.h"

#include "Kernel/Factorable.h"

#include "Environment/Windows/WindowsIncluder.h"

namespace Mengine
{
    class Win32DynamicLibrary
        : public DynamicLibraryInterface
        , public Factorable
    {
    public:
        Win32DynamicLibrary();
        ~Win32DynamicLibrary() override;

    public:
        void setName( const WString & _name );
        const WString & getName() const;

    public:
        bool load() override;

    public:
        TDynamicLibraryFunction getSymbol( const Char * _name ) const override;

    protected:
        WString m_name;
        HINSTANCE m_hInstance;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Win32DynamicLibrary> Win32DynamicLibraryPtr;
    //////////////////////////////////////////////////////////////////////////
};