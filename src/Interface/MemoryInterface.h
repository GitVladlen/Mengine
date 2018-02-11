#	pragma once

#	include "Interface/ServiceInterface.h"
#	include "Interface/StreamInterface.h"

#   include "Factory/FactorablePtr.h"

#	include "Core/Pointer.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	class MemoryGetterBufferInterface
		: public ServantInterface
	{ 
	public:
		virtual Pointer getMemory() const = 0;
		virtual size_t getSize() const = 0;
		virtual bool empty() const = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<MemoryGetterBufferInterface> MemoryGetterBufferInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
	class MemoryInterface
		: public MemoryGetterBufferInterface
	{
	public:
		virtual void setMemory( const void * _ptr, size_t _size, const char * _file, uint32_t _line ) = 0;
		virtual Pointer newMemory( size_t _size, const char * _file, uint32_t _line ) = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<MemoryInterface> MemoryInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
	class MemoryGetterStreamInterface
		: public InputStreamInterface
	{
	public:
		virtual Pointer getMemory() const = 0;
		virtual size_t getSize() const = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	class MemoryCacheInputInterface
		: public MemoryGetterStreamInterface
	{
	public:
		virtual Pointer cacheMemory( size_t _size, const char * _file, uint32_t _line ) = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<MemoryCacheInputInterface> MemoryCacheInputInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
	class MemoryProxyInputInterface
		: public MemoryGetterStreamInterface
	{
	public:
		virtual Pointer setMemory( void * _memory, size_t _offset, size_t _size ) = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<MemoryProxyInputInterface> MemoryProxyInputInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
	class MemoryInputInterface
		: public MemoryGetterStreamInterface
	{
	public:		
		virtual Pointer newMemory( size_t _size ) = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<MemoryInputInterface> MemoryInputInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
	class MemoryServiceInterface
		: public ServiceInterface
	{
		SERVICE_DECLARE( "MemoryService" )

	public:
        virtual MemoryInterfacePtr createMemory() = 0;
		virtual MemoryInterfacePtr createMemoryCacheBuffer() = 0;

    public:
		virtual MemoryCacheInputInterfacePtr createMemoryCacheInput() = 0;
		virtual MemoryProxyInputInterfacePtr createMemoryProxyInput() = 0;
		virtual MemoryInputInterfacePtr createMemoryInput() = 0;		

	public:
		virtual void clearCacheBuffers() = 0;
	};
	//////////////////////////////////////////////////////////////////////////
#   define MEMORY_SERVICE()\
	((Menge::MemoryServiceInterface *)SERVICE_GET(Menge::MemoryServiceInterface))
}
