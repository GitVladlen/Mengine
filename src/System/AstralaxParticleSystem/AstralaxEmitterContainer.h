#	pragma once

#	include "Interface/ParticleSystemInterface.h"

#	include "AstralaxEmitter.h"

#   include "Factory/FactoryPool.h"

#   ifndef MENGINE_UNSUPPORT_PRAGMA_WARNING
#	pragma warning(push, 0) 
#	endif 

#	include "magic.h"

#   ifndef MENGINE_UNSUPPORT_PRAGMA_WARNING
#	pragma warning(pop) 
#	endif

#	include <map>

namespace Menge
{
	typedef std::vector<HM_EMITTER> TVectorEmitters;

	class AstralaxEmitterContainer
		: public ParticleEmitterContainerInterface
	{
	public:
		AstralaxEmitterContainer();
		~AstralaxEmitterContainer();

    public:
        bool initialize( ServiceProviderInterface * _serviceProvider ) override;

	public:
		void addEmitterIds( const ConstString & _name, HM_EMITTER _id, MAGIC_POSITION _pos, const TVectorEmitters & _emitters );
		HM_EMITTER getEmitterId( const ConstString & _name ) const;
		void releaseEmitterId( const ConstString & _name, HM_EMITTER _id );

	public:
		void addAtlas( const ParticleEmitterAtlas & _atlas );
		void visitContainer( ParticleEmitterContainerVisitor * visitor ) override;
		const TVectorParticleEmitterAtlas & getAtlas() const override;
		ParticleEmitterInterface * createEmitter( const ConstString & _name ) override;
		void releaseEmitter( ParticleEmitterInterface * _emitter );
		
	private:
        ServiceProviderInterface * m_serviceProvider;

        struct EmitterPool
        {
            HM_EMITTER id;
            MAGIC_POSITION pos;

            mutable TVectorEmitters emitters;
            bool dublicate;
        };

        typedef FactoryPool<AstralaxEmitter, 16> TFactoryPoolAstralaxEmitter;
        TFactoryPoolAstralaxEmitter m_factoryPoolAstralaxEmitter;

		typedef std::map<ConstString, EmitterPool> TMapEmitters;		
		TMapEmitters m_emitters;

		TVectorParticleEmitterAtlas m_atlas;
	};
}