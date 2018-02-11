#	include "ImageEncoderACF.h"
#   include "ImageVerifyACF.h"

#	include "Interface/FileSystemInterface.h"
#	include "Interface/ArchiveInterface.h"
#	include "Interface/StringizeInterface.h"

#	include "Core/Stream.h"

#	include "Logger/Logger.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	ImageEncoderACF::ImageEncoderACF()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	ImageEncoderACF::~ImageEncoderACF()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool ImageEncoderACF::_initialize()
	{
		m_archivator = ARCHIVE_SERVICE()
			->getArchivator( STRINGIZE_STRING_LOCAL( "lz4") );

		if( m_archivator == nullptr )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	size_t ImageEncoderACF::encode( const void * _buffer, size_t _size, const CodecDataInfo* _bufferDataInfo )
	{
		(void)_size;

		if( Helper::writeStreamMagicHeader( m_stream, GET_MAGIC_NUMBER(MAGIC_ACF), GET_MAGIC_VERSION(MAGIC_ACF) ) == false )
		{
			LOGGER_ERROR("ImageEncoderACF::encode invalid write magic header"
				);

			return 0;
		}

		const ImageCodecDataInfo * dataInfo = static_cast<const ImageCodecDataInfo *>(_bufferDataInfo);
				
		uint32_t width = dataInfo->width;
        m_stream->write( &width, sizeof(width) );

		uint32_t height = dataInfo->height;
        m_stream->write( &height, sizeof(height) );

		uint32_t mipmaps = dataInfo->mipmaps;
		m_stream->write( &mipmaps, sizeof(mipmaps) );

		const unsigned char * mipmap_buffer = reinterpret_cast<const unsigned char *>(_buffer);

		for( uint32_t i = 0; i != mipmaps; ++i )
		{   
			size_t mipmap_size = dataInfo->getMipMapSize( i );

			if( Helper::writeStreamArchiveBuffer( m_stream, m_archivator, false, mipmap_buffer, mipmap_size, EAC_BEST ) == false )
			{
				LOGGER_ERROR("ImageEncoderACF::encode invalid write buffer"
					);

				return 0;
			}

			mipmap_buffer += mipmap_size;
		}

		return 1;
	}
	//////////////////////////////////////////////////////////////////////////
}	// namespace Menge
