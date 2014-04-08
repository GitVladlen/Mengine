#pragma once

#	include "Codec/ImageDecoder.h"

namespace Menge
{
	class ImageDecoderETZ1 
		: public ImageDecoder
	{
	public:
		ImageDecoderETZ1();
		~ImageDecoderETZ1();

	public:
		bool _initialize() override;

	public:
		size_t decode( void * _buffer, size_t _bufferSize ) override;

	private:
		uint32_t m_uncompress_size;		
		uint32_t m_compress_size;
	};
}
