#include "DataflowResourcePrefetcher.h"

#include "Interface/PrefetcherServiceInterface.h"
#include "Interface/VocabularyServiceInterface.h"
#include "Interface/ContentInterface.h"

#include "Kernel/AssertionMemoryPanic.h"
#include "Kernel/ConstStringHelper.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    DataflowResourcePrefetcher::DataflowResourcePrefetcher()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    DataflowResourcePrefetcher::~DataflowResourcePrefetcher()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool DataflowResourcePrefetcher::_prefetch( const ResourcePtr & _resource, const PrefetcherObserverInterfacePtr & _observer ) const
    {
        ContentInterface * content = _resource->getContent();

        MENGINE_ASSERTION_MEMORY_PANIC( content, false, "resource '%s' is not contentable"
            , _resource->getType().c_str()
        );

        const FileGroupInterfacePtr & fileGroup = content->getFileGroup();
        const FilePath & filePath = content->getFilePath();
        const DataflowInterfacePtr & dataflow = content->getDataflow();

        if( PREFETCHER_SERVICE()
            ->prefetchData( fileGroup, filePath, dataflow, _observer ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////        
    bool DataflowResourcePrefetcher::_unfetch( const ResourcePtr & _resource ) const
    {
        ContentInterface * content = _resource->getContent();

        MENGINE_ASSERTION_MEMORY_PANIC( content, false, "resource '%s' is not contentable"
            , _resource->getType().c_str()
        );

        const FileGroupInterfacePtr & fileGroup = content->getFileGroup();
        const FilePath & filePath = content->getFilePath();

        bool successful = PREFETCHER_SERVICE()
            ->unfetch( fileGroup, filePath );

        return successful;
    }
}