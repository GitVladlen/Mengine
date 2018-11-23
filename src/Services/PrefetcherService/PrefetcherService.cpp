#include "PrefetcherService.h"

#include "Interface/ThreadServiceInterface.h"
#include "Interface/StringizeServiceInterface.h"
#include "Interface/ConfigServiceInterface.h"
#include "Interface/DataServiceInterface.h"

#include "Kernel/FactoryPool.h"
#include "Kernel/AssertionFactory.h"

#include "Kernel/Logger.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_FACTORY( PrefetcherService, Mengine::PrefetcherService );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    PrefetcherService::PrefetcherService()
        : m_avaliable( true )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    PrefetcherService::~PrefetcherService()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::_initializeService()
    {
        bool avaliable = CONFIG_VALUE( "Engine", "PrefetcherAvaliable", true );

        if( avaliable == false )
        {
            m_avaliable = false;

            return true;
        }

        THREAD_SERVICE()
            ->createThread( STRINGIZE_STRING_LOCAL( "ThreadPrefetcherManager" ), -1, __FILE__, __LINE__ );

        m_threadQueue = THREAD_SERVICE()
            ->runTaskQueue( STRINGIZE_STRING_LOCAL( "ThreadPrefetcherManager" ), MENGINE_PREFETCHER_THREAD_COUNT, MENGINE_PREFETCHER_PACKET_SIZE );

        m_factoryThreadTaskPrefetchImageDecoder = new FactoryPool<ThreadTaskPrefetchImageDecoder, 16>();
        m_factoryThreadTaskPrefetchSoundDecoder = new FactoryPool<ThreadTaskPrefetchSoundDecoder, 16>();
        m_factoryThreadTaskPrefetchDataflow = new FactoryPool<ThreadTaskPrefetchDataflow, 16>();

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void PrefetcherService::_finalizeService()
    {
        if( m_avaliable == false )
        {
            return;
        }

        if( m_threadQueue != nullptr )
        {
            THREAD_SERVICE()
                ->cancelTaskQueue( m_threadQueue );

            m_threadQueue = nullptr;
        }

        m_prefetchReceiver.clear();

        MENGINE_ASSERTION_FACTORY_EMPTY( m_factoryThreadTaskPrefetchImageDecoder );
        MENGINE_ASSERTION_FACTORY_EMPTY( m_factoryThreadTaskPrefetchSoundDecoder );
        MENGINE_ASSERTION_FACTORY_EMPTY( m_factoryThreadTaskPrefetchDataflow );

        m_factoryThreadTaskPrefetchImageDecoder = nullptr;
        m_factoryThreadTaskPrefetchSoundDecoder = nullptr;
        m_factoryThreadTaskPrefetchDataflow = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    void PrefetcherService::update()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::prefetchImageDecoder( const FileGroupInterfacePtr& _fileGroup, const FilePath & _filePath, const ConstString & _codecType, const PrefetcherObserverInterfacePtr & _observer )
    {
        if( m_avaliable == false )
        {
            _observer->onPrefetchIgnored();

            return true;
        }

        if( m_threadQueue == nullptr )
        {
            _observer->onPrefetchIgnored();

            return true;
        }

        PrefetchReceiver * receiver;
        if( this->hasPrefetch_( _fileGroup, _filePath, &receiver ) == true )
        {
            ++receiver->refcount;

            _observer->onPrefetchAlreadyExist();

            return true;
        }

        PrefetchReceiver new_receiver;

        new_receiver.refcount = 1;

        ThreadTaskPrefetchImageDecoderPtr task = m_factoryThreadTaskPrefetchImageDecoder->createObject();

        task->initialize( _fileGroup, _filePath, _observer );
        task->setImageCodec( _codecType );

        new_receiver.prefetcher = task;

        m_prefetchReceiver.emplace( std::make_pair( _fileGroup->getName(), _filePath ), new_receiver );

        m_threadQueue->addTask( task );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::getImageDecoder( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath, ImageDecoderInterfacePtr & _decoder )
    {
        if( m_avaliable == false )
        {
            return false;
        }

        PrefetchReceiver * receiver;
        if( this->popPrefetch_( _fileGroup, _filePath, &receiver ) == false )
        {
            return false;
        }

        ThreadTaskPrefetchImageDecoderPtr prefetcherImageDecoder = stdex::intrusive_static_cast<ThreadTaskPrefetchImageDecoderPtr>(receiver->prefetcher);

        const ImageDecoderInterfacePtr & prefetch_decoder = prefetcherImageDecoder->getDecoder();

        if( prefetch_decoder == nullptr )
        {
            return false;
        }

        _decoder = prefetch_decoder;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::prefetchSoundDecoder( const FileGroupInterfacePtr& _fileGroup, const FilePath & _filePath, const ConstString & _codecType, const PrefetcherObserverInterfacePtr & _observer )
    {
        if( m_avaliable == false )
        {
            _observer->onPrefetchIgnored();

            return true;
        }

        if( m_threadQueue == nullptr )
        {
            _observer->onPrefetchIgnored();

            return true;
        }

        PrefetchReceiver * receiver;
        if( this->hasPrefetch_( _fileGroup, _filePath, &receiver ) == true )
        {
            ++receiver->refcount;

            _observer->onPrefetchAlreadyExist();

            return true;
        }

        PrefetchReceiver new_receiver;

        new_receiver.refcount = 1;

        ThreadTaskPrefetchSoundDecoderPtr task = m_factoryThreadTaskPrefetchSoundDecoder->createObject();

        task->initialize( _fileGroup, _filePath, _observer );
        task->setSoundCodec( _codecType );

        new_receiver.prefetcher = task;

        m_prefetchReceiver.emplace( std::make_pair( _fileGroup->getName(), _filePath ), new_receiver );

        m_threadQueue->addTask( task );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::getSoundDecoder( const FileGroupInterfacePtr& _fileGroup, const FilePath & _filePath, SoundDecoderInterfacePtr & _decoder )
    {
        if( m_avaliable == false )
        {
            return false;
        }

        PrefetchReceiver * receiver;
        if( this->popPrefetch_( _fileGroup, _filePath, &receiver ) == false )
        {
            return false;
        }

        ThreadTaskPrefetchSoundDecoderPtr prefetcherSoundDecoder = stdex::intrusive_static_cast<ThreadTaskPrefetchSoundDecoderPtr>(receiver->prefetcher);

        const SoundDecoderInterfacePtr & prefetch_decoder = prefetcherSoundDecoder->getDecoder();

        if( prefetch_decoder == nullptr )
        {
            return false;
        }

        _decoder = prefetch_decoder;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::prefetchData( const FileGroupInterfacePtr& _fileGroup, const FilePath & _filePath, const ConstString & _dataflowType, const PrefetcherObserverInterfacePtr & _observer )
    {
        if( m_avaliable == false )
        {
            _observer->onPrefetchIgnored();

            return true;
        }

        if( m_threadQueue == nullptr )
        {
            _observer->onPrefetchIgnored();

            return true;
        }

        PrefetchReceiver * receiver;
        if( this->hasPrefetch_( _fileGroup, _filePath, &receiver ) == true )
        {
            ++receiver->refcount;

            _observer->onPrefetchAlreadyExist();

            return true;
        }

        PrefetchReceiver new_receiver;

        new_receiver.refcount = 1;

        ThreadTaskPrefetchDataflowPtr task = m_factoryThreadTaskPrefetchDataflow->createObject();

        task->initialize( _fileGroup, _filePath, _observer );

        const DataflowInterfacePtr & dataflow = DATA_SERVICE()
            ->getDataflow( _dataflowType );

        if( dataflow == nullptr )
        {
            LOGGER_ERROR( "PrefetcherService::prefetchData: '%s':'%s' invalide get dataflow '%s'"
                , _fileGroup->getName().c_str()
                , _filePath.c_str()
                , _dataflowType.c_str()
            );

            return false;
        }

        task->setDataflow( dataflow );

        new_receiver.prefetcher = task;

        m_prefetchReceiver.emplace( std::make_pair( _fileGroup->getName(), _filePath ), new_receiver );

        m_threadQueue->addTask( task );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////			
    bool PrefetcherService::getData( const FileGroupInterfacePtr& _fileGroup, const FilePath & _filePath, DataInterfacePtr & _data )
    {
        if( m_avaliable == false )
        {
            return false;
        }

        PrefetchReceiver * receiver;
        if( this->popPrefetch_( _fileGroup, _filePath, &receiver ) == false )
        {
            return false;
        }

        ThreadTaskPrefetchDataflowPtr prefetcherDataflow = stdex::intrusive_static_cast<ThreadTaskPrefetchDataflowPtr>(receiver->prefetcher);

        const DataInterfacePtr & prefetch_data = prefetcherDataflow->getData();

        if( prefetch_data == nullptr )
        {
            return false;
        }

        _data = prefetch_data;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::unfetch( const FileGroupInterfacePtr& _fileGroup, const FilePath& _filePath )
    {
        MapPrefetchReceiver::iterator it_found = m_prefetchReceiver.find( std::make_pair( _fileGroup->getName(), _filePath ) );

        if( it_found == m_prefetchReceiver.end() )
        {
            return false;
        }

        PrefetchReceiver & receiver = it_found->second;

        if( --receiver.refcount > 0 )
        {
            return true;
        }

        receiver.prefetcher->cancel();

        m_prefetchReceiver.erase( it_found );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void PrefetcherService::visitPrefetches( const VisitorPtr & _visitor ) const
    {
        for( MapPrefetchReceiver::const_iterator
            it = m_prefetchReceiver.begin(),
            it_end = m_prefetchReceiver.end();
            it != it_end;
            ++it )
        {
            const PrefetchReceiver & receiver = it->second;

            const ThreadTaskPrefetchPtr & prefetcher = receiver.prefetcher;

            prefetcher->visit( _visitor );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::hasPrefetch_( const FileGroupInterfacePtr& _fileGroup, const FilePath & _filePath, PrefetchReceiver ** _receiver ) const
    {
        MapPrefetchReceiver::const_iterator it_found = m_prefetchReceiver.find( std::make_pair( _fileGroup->getName(), _filePath ) );

        if( it_found == m_prefetchReceiver.end() )
        {
            return false;
        }

        const PrefetchReceiver & receiver = it_found->second;

        *_receiver = const_cast<PrefetchReceiver *>(&receiver);

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool PrefetcherService::popPrefetch_( const FileGroupInterfacePtr& _fileGroup, const FilePath & _filePath, PrefetchReceiver ** _receiver )
    {
        MapPrefetchReceiver::iterator it_found = m_prefetchReceiver.find( std::make_pair( _fileGroup->getName(), _filePath ) );

        if( it_found == m_prefetchReceiver.end() )
        {
            return false;
        }

        PrefetchReceiver & receiver = it_found->second;

        if( receiver.prefetcher->isComplete() == false ||
            receiver.prefetcher->isSuccessful() == false )
        {
            --receiver.refcount;

            receiver.prefetcher->cancel();

            m_prefetchReceiver.erase( it_found );

            return false;
        }

        *_receiver = const_cast<PrefetchReceiver *>(&receiver);

        return true;
    }
}