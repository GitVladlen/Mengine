#	pragma once

#	include "Interface/FileSystemInterface.h"
#	include "Interface/WindowsLayerInterface.h"

namespace Menge
{
	class Win32InputStream
		: public FileInputStreamInterface
	{
	public:
		Win32InputStream();
		~Win32InputStream();

    public:
        void setServiceProvider( ServiceProviderInterface * _serviceProvider );

	public:
		bool open( const FilePath & _folder, const FilePath & _fileName, size_t _offset, size_t _size ) override;
		
	public:
		size_t read( void* _buf, size_t _count ) override;
		bool seek( size_t _pos ) override;
		size_t tell() const override;
		size_t size() const override;		

    public:
        bool time( uint64_t & _time ) const override;

    protected:
		bool openFile_( const FilePath & _folder, const FilePath & _fileName, WChar * _filePath );
        void close_();

    protected:
        ServiceProviderInterface * m_serviceProvider;

		HANDLE m_hFile;

		size_t m_size;
		size_t m_offset;		
		size_t m_pos;
	};
}	// namespace Menge