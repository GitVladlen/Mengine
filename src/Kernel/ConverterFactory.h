#pragma once

#include "Interface/ConverterServiceInterface.h"

#include "Kernel/Factorable.h"
#include "Kernel/ConstString.h"

#include "Kernel/FactoryPool.h"
#include "Kernel/AssertionFactory.h"
#include "Kernel/ConstStringHelper.h"

namespace Mengine
{
    template<class T>
    class ConverterFactory
        : public ConverterFactoryInterface
        , public Factorable
    {
    protected:
        bool initialize() override
        {
            m_factory = Helper::makeFactoryPool<T, 8>();

            return true;
        }

    protected:
        ConverterInterfacePtr createConverter( const Char * _doc ) override
        {
            IntrusivePtr<T> converter = m_factory->createObject( _doc );

            return converter;
        }

    protected:
        void destroy() override
        {
            MENGINE_ASSERTION_FACTORY_EMPTY( m_factory );

            m_factory = nullptr;

            delete this;
        }

    protected:
        FactoryPtr m_factory;
    };
    //////////////////////////////////////////////////////////////////////////
    namespace Helper
    {
        template<class T>
        MENGINE_INLINE ConverterFactoryInterfacePtr registerConverter( const Char * _type )
        {
            ConverterFactoryInterfacePtr converter = Helper::makeFactorableUnique<ConverterFactory<T> >();

            if( converter->initialize() == false )
            {
                return nullptr;
            }

            if( CONVERTER_SERVICE()
                ->registerConverter( Helper::stringizeString( _type ), converter ) == false )
            {
                return nullptr;
            }

            return converter;
        }

        MENGINE_INLINE void unregisterConverter( const Char * _type )
        {
            CONVERTER_SERVICE()
                ->unregisterConverter( Helper::stringizeString( _type ) );
        }
    }
}