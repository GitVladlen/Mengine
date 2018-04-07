#pragma once

#include "BasePrototypeGenerator.h"

#include "Kernel/Surface.h"

#include "Core/ConstString.h"
#include "Core/MemoryAllocator.h"

#include "Factory/FactoryPool.h"

#include "Logger/Logger.h"

namespace Mengine
{
	template<class Type, uint32_t Count>
	class SurfacePrototypeGenerator
		: public BasePrototypeGenerator
	{
    protected:
        bool _initialize() override
        {
            m_factory = new FactoryPool<Type, Count>();

			return true;
        }

	protected:
		PointerFactorable generate() override
		{
			Surface * surface = m_factory->createObject();

			if( surface == nullptr )
			{
				LOGGER_ERROR("NodePrototypeGenerator::generate can't generate %s %s"
					, m_category.c_str()
					, m_prototype.c_str()
					);

				return nullptr;
			}

			surface->setType( m_prototype );
			surface->setScriptWrapper( m_scriptWrapper );

			return surface;
		}

		uint32_t count() const override
		{
			uint32_t count = m_factory->getCountObject();

			return count;
		}

	protected:		
		FactoryPtr m_factory;
	};
}