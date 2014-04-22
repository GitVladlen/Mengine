#	include "ResourceEmitterContainer.h"
#	include "Kernel/ResourceImplement.h"

#	include "ResourceImageDefault.h"

#   include "Interface/ResourceInterface.h"
#   include "Interface/ParticleSystemInterface.h"
#   include "Interface/StringizeInterface.h"

#	include "Logger/Logger.h"

#	include "Consts.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	RESOURCE_IMPLEMENT( ResourceEmitterContainer );
	//////////////////////////////////////////////////////////////////////////
	ResourceEmitterContainer::ResourceEmitterContainer()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void ResourceEmitterContainer::setFilePath( const FilePath& _path )
	{
		m_fileName = _path;
	}
	//////////////////////////////////////////////////////////////////////////
	const FilePath& ResourceEmitterContainer::getFilePath() const
	{
		return m_fileName;
	}
	//////////////////////////////////////////////////////////////////////////
	void ResourceEmitterContainer::setFolderPath( const FilePath& _folder )
	{
		m_folder = _folder;
	}
	//////////////////////////////////////////////////////////////////////////
	const FilePath& ResourceEmitterContainer::getFolderPath() const
	{
		return m_folder;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ResourceEmitterContainer::_loader( const Metabuf::Metadata * _meta )
	{
        const Metacode::Meta_DataBlock::Meta_ResourceEmitterContainer * metadata
            = static_cast<const Metacode::Meta_DataBlock::Meta_ResourceEmitterContainer *>(_meta);

        metadata->swap_File_Path( m_fileName );
        metadata->swap_Folder_Path( m_folder );

        return true;
	}
    //////////////////////////////////////////////////////////////////////////
    bool ResourceEmitterContainer::_isValid() const
    {
        const ConstString & category = this->getCategory();

        ParticleEmitterContainerInterfacePtr container = PARTICLE_SERVICE(m_serviceProvider)
            ->createEmitterContainerFromFile( category, m_fileName );

        if( container == nullptr )
        {
            LOGGER_ERROR(m_serviceProvider)("ResourceEmitterContainer::_isValid %s can't create container file '%s'"
                , m_name.c_str()
                , m_fileName.c_str() 
                );

            return false;
        }

        const TVectorParticleEmitterAtlas & atlas = container->getAtlas();

        for( TVectorParticleEmitterAtlas::const_iterator
            it = atlas.begin(),
            it_end = atlas.end();
        it != it_end;
        ++it )
        {            
            const FilePath & filename = it->file;

            ConstString filepath = this->makeTexturePath_( filename );

            const ConstString & category = this->getCategory();

            InputStreamInterfacePtr stream = 
                FILE_SERVICE(m_serviceProvider)->openInputFile( category, filepath );

            if( stream == nullptr )
            {
                LOGGER_ERROR(m_serviceProvider)("ResourceEmitterContainer::_isValid %s can't create texture '%s'"
                    , m_name.c_str()
                    , filepath.c_str() 
                    );

                return false;
            }

            const ConstString & codecType = CODEC_SERVICE(m_serviceProvider)
                ->findCodecType( filepath );

            ImageDecoderInterfacePtr imageDecoder = CODEC_SERVICE(m_serviceProvider)
                ->createDecoderT<ImageDecoderInterfacePtr>( codecType );

            if( imageDecoder == nullptr )
            {
                LOGGER_ERROR(m_serviceProvider)("ResourceEmitterContainer::_isValid %s can't create decoder '%s'"
                    , m_name.c_str()
                    , codecType.c_str() 
                    );

                return false;
            }

			if( imageDecoder->initialize( stream ) == false )
			{
				LOGGER_ERROR(m_serviceProvider)("ResourceEmitterContainer::_isValid %s can't initialize decoder '%s'"
					, m_name.c_str()
					, codecType.c_str() 
					);

				return false;
			}
        }

        if( container->isValid() == false )
        {
            LOGGER_ERROR(m_serviceProvider)("ResourceEmitterContainer::_isValid %s can't valid container '%s'"
                , m_name.c_str()
                , m_fileName.c_str() 
                );

            return false;
        }

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool ResourceEmitterContainer::_compile()
	{
		const ConstString & category = this->getCategory();

		m_container = PARTICLE_SERVICE(m_serviceProvider)
			->createEmitterContainerFromFile( category, m_fileName );

		if( m_container == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("ResourceEmitterContainer::_compile %s can't create container file '%s'"
                , m_name.c_str()
				, m_fileName.c_str() 
				);

			return false;
		}
		
		const TVectorParticleEmitterAtlas & atlas = m_container->getAtlas();
		
		for( TVectorParticleEmitterAtlas::const_iterator
			it = atlas.begin(),
			it_end = atlas.end();
		it != it_end;
		++it )
		{
			const FilePath & filename = it->file;

            ConstString filepath = this->makeTexturePath_( filename );

            const ConstString & category = this->getCategory();

            const ConstString & codecType = CODEC_SERVICE(m_serviceProvider)
                ->findCodecType( filepath );

            RenderTextureInterfacePtr texture = RENDERTEXTURE_SERVICE(m_serviceProvider)
                ->loadTexture( category, filepath, codecType );

            if( texture == nullptr )
            {
                LOGGER_ERROR(m_serviceProvider)("ResourceEmitterContainer::_compile %s can't create texture '%s'"
                    , m_name.c_str()
                    , filepath.c_str() 
                    );

                return false;
            }

			m_atlasRenderTextures.push_back( texture );
		}

		return true;
	}
    //////////////////////////////////////////////////////////////////////////
    ConstString ResourceEmitterContainer::makeTexturePath_( const FilePath & _filepath ) const
    {        
		PathString cache_path;
        
		cache_path += m_folder;
		cache_path += MENGE_FOLDER_RESOURCE_DELIM;
		cache_path += _filepath;

        ConstString name = Helper::stringizeString( m_serviceProvider, cache_path );

        return name;
    }
	//////////////////////////////////////////////////////////////////////////
	void ResourceEmitterContainer::_release()
	{
		m_atlasRenderTextures.clear();

		m_container = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	const RenderTextureInterfacePtr & ResourceEmitterContainer::getAtlasTexture( size_t _atlasId )
	{
        const RenderTextureInterfacePtr & texture = m_atlasRenderTextures[_atlasId];

		return texture;
	}
	//////////////////////////////////////////////////////////////////////////
	size_t ResourceEmitterContainer::getAtlasTextureCount() const
	{
		size_t count = m_atlasRenderTextures.size();

		return count;
	}
	//////////////////////////////////////////////////////////////////////////
	const ParticleEmitterContainerInterfacePtr & ResourceEmitterContainer::getContainer() const
	{
		return m_container;
	}
	//////////////////////////////////////////////////////////////////////////
}
