#	include "PrefetcherManager.h"

#	include "Interface/ThreadSystemInterface.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_FACTORY( PrefetcherService, Menge::PrefetcherServiceInterface, Menge::PrefetcherManager );
//////////////////////////////////////////////////////////////////////////
namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	PrefetcherManager::PrefetcherManager()
		: m_serviceProvider(nullptr)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	PrefetcherManager::~PrefetcherManager()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void PrefetcherManager::setServiceProvider( ServiceProviderInterface * _serviceProvider )
	{
		m_serviceProvider = _serviceProvider;
	}
	//////////////////////////////////////////////////////////////////////////
	ServiceProviderInterface * PrefetcherManager::getServiceProvider() const
	{
		return m_serviceProvider;
	}
	//////////////////////////////////////////////////////////////////////////
	bool PrefetcherManager::initialize()
	{
		m_threadPool = THREAD_SERVICE(m_serviceProvider)
			->runTaskPool();

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void PrefetcherManager::finalize()
	{
		m_threadPool->cancel();
		m_threadPool = nullptr;

		for( TMapPrefetchImageDecoderReceiver::iterator
			it = m_prefetchImageDecoderReceiver.begin(),
			it_end = m_prefetchImageDecoderReceiver.end();
		it != it_end;
		++it )
		{
			PrefetchImageDecoderReceiver & receiver = m_prefetchImageDecoderReceiver.get_value( it );
			receiver.prefetcher->cancel();
		}

		m_prefetchImageDecoderReceiver.clear();

		for( TMapPrefetchDataReceiver::iterator
			it = m_prefetchDataReceiver.begin(),
			it_end = m_prefetchDataReceiver.end();
		it != it_end;
		++it )
		{
			PrefetchDataReceiver & receiver = m_prefetchDataReceiver.get_value( it );
			receiver.prefetcher->cancel();
		}

		m_prefetchDataReceiver.clear();
	}
	//////////////////////////////////////////////////////////////////////////
	bool PrefetcherManager::prefetchImageDecoder( const ConstString& _pakName, const FilePath & _fileName, const ConstString & _codec )
	{
		TMapPrefetchImageDecoderReceiver::iterator it_found = m_prefetchImageDecoderReceiver.find( _fileName );

		if( it_found == m_prefetchImageDecoderReceiver.end() )
		{
			PrefetchImageDecoderReceiver receiver;
			receiver.refcount = 1;
						
			ThreadTaskPrefetchImageDecoderPtr task = m_factoryThreadTaskPrefetchImageDecoder.createObjectT();
			task->setServiceProvider( m_serviceProvider );
			task->initialize( _pakName, _fileName, _codec );

			receiver.prefetcher = task;

			m_prefetchImageDecoderReceiver.insert( _fileName, receiver );

			m_threadPool->addTask( task );

			return true;
		}

		PrefetchImageDecoderReceiver & receiver = m_prefetchImageDecoderReceiver.get_value( it_found );

		++receiver.refcount;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void PrefetcherManager::unfetchImageDecoder( const FilePath& _fileName )
	{
		TMapPrefetchImageDecoderReceiver::iterator it_found = m_prefetchImageDecoderReceiver.find( _fileName );

		if( it_found != m_prefetchImageDecoderReceiver.end() )
		{
			return;
		}

		PrefetchImageDecoderReceiver & receiver = m_prefetchImageDecoderReceiver.get_value( it_found );

		if( --receiver.refcount == 0 )
		{
			return;
		}

		receiver.prefetcher->cancel();

		m_prefetchImageDecoderReceiver.erase( it_found );
	}
	//////////////////////////////////////////////////////////////////////////
	bool PrefetcherManager::getImageDecoder( const FilePath & _fileName, ImageDecoderInterfacePtr & _decoder ) const
	{
		TMapPrefetchImageDecoderReceiver::const_iterator it_found = m_prefetchImageDecoderReceiver.find( _fileName );

		if( it_found == m_prefetchImageDecoderReceiver.end() )
		{
			return false;
		}

		const PrefetchImageDecoderReceiver & receiver = m_prefetchImageDecoderReceiver.get_value( it_found );

		if( receiver.prefetcher->isComplete() == false )
		{
			return false;
		}

		ImageDecoderInterfacePtr prefetch_decoder = receiver.prefetcher->getDecoder();
		
		_decoder = prefetch_decoder;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool PrefetcherManager::prefetchData( const ConstString& _pakName, const FilePath & _fileName, const ConstString & _dataflowType )
	{
		TMapPrefetchDataReceiver::iterator it_found = m_prefetchDataReceiver.find( _fileName );

		if( it_found == m_prefetchDataReceiver.end() )
		{
			PrefetchDataReceiver receiver;
			receiver.refcount = 1;

			ThreadTaskPrefetchDataflowPtr task = m_factoryThreadTaskPrefetchDataflow.createObjectT();
			task->setServiceProvider( m_serviceProvider );
			task->initialize( _pakName, _fileName, _dataflowType );

			receiver.prefetcher = task;

			m_prefetchDataReceiver.insert( _fileName, receiver );

			m_threadPool->addTask( task );

			return true;
		}

		PrefetchDataReceiver & receiver = m_prefetchDataReceiver.get_value( it_found );

		++receiver.refcount;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void PrefetcherManager::unfetchData( const FilePath& _fileName )
	{
		TMapPrefetchDataReceiver::iterator it_found = m_prefetchDataReceiver.find( _fileName );

		if( it_found != m_prefetchDataReceiver.end() )
		{
			return;
		}

		PrefetchDataReceiver & receiver = m_prefetchDataReceiver.get_value( it_found );

		if( --receiver.refcount == 0 )
		{
			return;
		}

		receiver.prefetcher->cancel();

		m_prefetchDataReceiver.erase( it_found );
	}
	//////////////////////////////////////////////////////////////////////////			
	bool PrefetcherManager::getData( const FilePath & _fileName, DataInterfacePtr & _data ) const
	{
		TMapPrefetchDataReceiver::const_iterator it_found = m_prefetchDataReceiver.find( _fileName );

		if( it_found == m_prefetchDataReceiver.end() )
		{
			return false;
		}

		const PrefetchDataReceiver & receiver = m_prefetchDataReceiver.get_value( it_found );

		if( receiver.prefetcher->isComplete() == false )
		{
			return false;
		}

		_data = receiver.prefetcher->getData();

		return true;
	}
}