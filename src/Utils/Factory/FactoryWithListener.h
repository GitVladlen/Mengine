#	pragma once

#	include "Factory.h"

namespace Menge
{
    //////////////////////////////////////////////////////////////////////////
	class FactoryDestroyListenerInterface
		: public FactorablePtr
	{
	public:
		virtual void onFactoryDestroyObject( Factorable * _object ) = 0;
	};
    //////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<FactoryDestroyListenerInterface> FactoryDestroyListenerInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
	class FactoryWithListener
		: public Factory
	{
	public:
        FactoryWithListener(const char * _name);
		virtual ~FactoryWithListener();

	public:
		void setDestroyListener( const FactoryDestroyListenerInterfacePtr & _listener );
		
	public:
        void destroyObject( Factorable * _object ) override;
       
    
	protected:		
		FactoryDestroyListenerInterfacePtr m_destroyListener;
	};
    //////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<FactoryWithListener> FactoryWithListenerPtr;
    //////////////////////////////////////////////////////////////////////////
}
