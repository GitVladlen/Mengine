#   pragma once

#   include "Interface/ServiceInterface.h"

#	include "Config/String.h"
#   include "Core/FilePath.h"

namespace Menge
{
    class MarmaladeLayerInterface
        : public ServiceInterface
    {
    public:
        SERVICE_DECLARE("MarmaladeLayerService")

    public:
        virtual bool concatenateFilePath( const FilePath & _folder, const FilePath & _fileName, Char * _filePath, size_t _capacity ) = 0;
    };

#   define MARMALADELAYER_SERVICE( serviceProvider )\
    ((Menge::MarmaladeLayerInterface *)SERVICE_GET(serviceProvider, Menge::MarmaladeLayerInterface))
}