#   pragma once

#	include "Config/Typedef.h"

#   include "Factory/FactorablePtr.h"

namespace Menge
{
    //////////////////////////////////////////////////////////////////////////
    class ServantInterface
        : public FactorablePtr
    {
    public:
        ServantInterface()
        {
        };

        virtual ~ServantInterface()
        {
        };

    public:
        virtual void setServiceProvider( ServiceProviderInterface * _serviceProvider ) = 0;
        virtual ServiceProviderInterface * getServiceProvider() const = 0;
    };
}