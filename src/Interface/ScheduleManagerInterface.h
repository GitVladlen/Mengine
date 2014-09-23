#   pragma once

#   include "Interface/ServiceInterface.h"

#   include "Factory/FactorablePtr.h"

namespace Menge
{
    class ScheduleListener
		: public FactorablePtr
    {
    public:
        virtual void onScheduleComplete( uint32_t _id ) = 0;
        virtual void onScheduleStop( uint32_t _id ) = 0;
    };

	typedef stdex::intrusive_ptr<ScheduleListener> ScheduleListenerPtr;

    class ScheduleManagerInterface
        : public Factorable
    {
    public:
        virtual void initialize( ServiceProviderInterface * _serviceProvider ) = 0;

    public:
        virtual uint32_t schedule( float _timing, const ScheduleListenerPtr & _listener ) = 0;

        virtual bool remove( uint32_t _id ) = 0;
        virtual void removeAll() = 0;

        virtual bool freeze( uint32_t _id, bool _freeze ) = 0;
        virtual void freezeAll( bool _freeze ) = 0;
		virtual bool isFreezeAll() const = 0;
        virtual bool isFreeze( uint32_t _id ) const = 0;

        virtual float time( uint32_t _id ) const = 0;

    public:
        virtual void update( float _current, float _timing ) = 0;
    };
}