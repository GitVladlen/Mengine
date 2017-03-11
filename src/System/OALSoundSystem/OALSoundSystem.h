#	pragma once

#	include "Interface/SoundSystemInterface.h"

#	include "OALSoundBufferBase.h"
#	include "OALSoundBufferStream.h"
#	include "OALSoundSource.h"

#   include "Core/ServiceBase.h"

#   include "Factory/FactoryStore.h"

#	include "AL/al.h"
#	include "AL/alc.h"

namespace Menge
{
	class OALSoundSource;

	class ServiceProviderInterface;

	class OALSoundSystem
		: public ServiceBase<SoundSystemInterface>
	{
	public:
		OALSoundSystem();
		~OALSoundSystem();

	public:
		bool _initialize() override;
        void _finalize() override;

	public:
		void update() override;

	public:
		bool isSilent() const override;
		
	public:
		void onTurnSound( bool _turn ) override;

	public:
		SoundSourceInterfacePtr createSoundSource( bool _isHeadMode, const SoundBufferInterfacePtr & _sample ) override;

		SoundBufferInterfacePtr createSoundBuffer( const SoundDecoderInterfacePtr & _soundDecoder, bool _isStream ) override;
		//SoundBufferInterface* createSoundBufferFromMemory( void * _buffer, int _size, bool _newmem ) override;

	public:
		ALuint genSourceId();
		void releaseSourceId( ALuint _sourceId );

		ALuint genBufferId();
		void releaseBufferId( ALuint _bufferId );

	private:
		ALCcontext * m_context;
		ALCdevice * m_device;

        FactoryPtr m_poolOALSoundBuffer;
        FactoryPtr m_poolOALSoundBufferStream;
        FactoryPtr m_poolOALSoundSource;

		bool m_threadAvaliable;
    };
}	// namespace Menge
