#	include "ToolUtils/ToolUtils.h"

#	include "pugixml.hpp"

#	include <vector>
#	include <string>
#	include <sstream>

#	include "jansson.h"

//////////////////////////////////////////////////////////////////////////
int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd )
{
	(void)hInstance;
	(void)hPrevInstance;
	(void)nShowCmd;

	uint32_t images_count = parse_kwds( lpCmdLine, L"--image_count", 0U );
	std::wstring in_path = parse_kwds( lpCmdLine, L"--in_path", std::wstring() );
	std::wstring texturepacker_path = parse_kwds( lpCmdLine, L"--texturepacker", std::wstring() );
	std::wstring atlas_path = parse_kwds( lpCmdLine, L"--atlas_path", std::wstring() );
	
	std::vector<std::wstring> images_path;

	FILE * f_in = _wfopen( in_path.c_str(), L"r" );

	if( f_in == NULL )
	{
		message_error( "invalid open in_path %ls\n"
			, in_path
		);

		return 0;
	}
	
	WCHAR image_path[MAX_PATH];
	while( fgetws( image_path, MAX_PATH, f_in ) )
	{		
		wchar_t * pos;
		if( (pos = wcschr( image_path, L'\n' )) != NULL )
		{
			*pos = '\0';
		}

		images_path.push_back( image_path );
	}

	if( texturepacker_path.empty() == true )
	{
		message_error( "not found 'texturepacker' param\n"
			);

		return 0;
	}

	if( images_path.empty() == true )
	{
		message_error( "not found 'images' param\n"
			);

		return 0;
	}

	//if( out_path.empty() == true )
	//{
	//	message_error( "not found 'images' param\n"
	//	);

	//	return 0;
	//}	

	std::wstring system_cmd;

	system_cmd += L" --multipack ";
	system_cmd += L" --enable-rotation ";
	system_cmd += L" --trim-mode Trim ";
	system_cmd += L" --size-constraints POT ";	

	WCHAR tempPath[MAX_PATH];
	GetTempPath( MAX_PATH, tempPath );

	WCHAR libmovieTempDir[MAX_PATH];
	PathCombine( libmovieTempDir, tempPath, L"libmovie\\TexturePacker\\" );

	WCHAR dataTempDir[MAX_PATH];
	PathCombine( dataTempDir, libmovieTempDir, L"data\\" );

	WCHAR dataFormat[MAX_PATH];
	PathCombine( dataFormat, dataTempDir, L"atlas_{n}.json" );

	WCHAR dataFormatQuote[MAX_PATH];
	ForcePathQuoteSpaces( dataFormatQuote, dataFormat );

	WCHAR sheetFormat[MAX_PATH];
	PathCombine( sheetFormat, atlas_path.c_str(), L"atlas_{n}.png" );

	WCHAR sheetFormatQuote[MAX_PATH];
	ForcePathQuoteSpaces( sheetFormatQuote, sheetFormat );

	system_cmd += L" --data ";
	system_cmd += dataFormatQuote;
	system_cmd += L" ";

	system_cmd += L" --sheet ";
	system_cmd += sheetFormatQuote;
	system_cmd += L" ";

	system_cmd += L" --format json-array ";	
	system_cmd += L" --texture-format png ";
	system_cmd += L" --max-width 4096 ";
	system_cmd += L" --max-height 4096 ";
	system_cmd += L" --max-size 4096 ";
	
	for( const std::wstring & image_path : images_path )
	{
		system_cmd += L" ";

		WCHAR ImagePathCanonicalizeQuote[MAX_PATH];
		ForcePathQuoteSpaces( ImagePathCanonicalizeQuote, image_path.c_str() );

		system_cmd += ImagePathCanonicalizeQuote;
	}

	ForceRemoveDirectory( libmovieTempDir );
	
	STARTUPINFO lpStartupInfo;
	ZeroMemory( &lpStartupInfo, sizeof( STARTUPINFOW ) );
	lpStartupInfo.cb = sizeof( lpStartupInfo );
	lpStartupInfo.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION lpProcessInformation;
	ZeroMemory( &lpProcessInformation, sizeof( PROCESS_INFORMATION ) );

	WCHAR lpCommandLine[32768];
	wcscpy_s( lpCommandLine, system_cmd.c_str() );

	WCHAR TexturePathCanonicalizeQuote[MAX_PATH];
	ForcePathQuoteSpaces( TexturePathCanonicalizeQuote, texturepacker_path );
	PathUnquoteSpaces( TexturePathCanonicalizeQuote );
	
	if( CreateProcess( TexturePathCanonicalizeQuote
		, lpCommandLine
		, NULL
		, NULL
		, FALSE
		, CREATE_NO_WINDOW
		, NULL
		, NULL
		, &lpStartupInfo
		, &lpProcessInformation ) == FALSE )
	{
		message_error( "invalid CreateProcess %ls %ls\n"
			, TexturePathCanonicalizeQuote
			, lpCommandLine
			);

		return 0;
	}

	CloseHandle( lpProcessInformation.hThread );

	WaitForSingleObject( lpProcessInformation.hProcess, INFINITE );

	DWORD exit_code;//��� ���������� ��������
	GetExitCodeProcess( lpProcessInformation.hProcess, &exit_code );

	CloseHandle( lpProcessInformation.hProcess );

	if( exit_code != 0 )
	{
		message_error( "invalid Process %ls exit_code %d\n"
			, TexturePathCanonicalizeQuote
			, exit_code
			);

		return 0;
	}

	WCHAR dataPath[MAX_PATH];
	PathCombine( dataPath, dataTempDir, L"*.json" );
	
	std::vector<std::wstring> files;
	SelectFile( dataPath, files );

	struct AtlasImageDesc
	{
		std::string name;
		std::string atlas;

		json_int_t aw;
		json_int_t ah;

		json_int_t ox;
		json_int_t oy;

		json_int_t ow;
		json_int_t oh;

		json_int_t fw;
		json_int_t fh;

		json_int_t x0;
		json_int_t y0;
		json_int_t x1;
		json_int_t y1;
		json_int_t x2;
		json_int_t y2;
		json_int_t x3;
		json_int_t y3;
	};

	std::vector<AtlasImageDesc> atlas_images;

	for( Files::const_iterator
		it = files.begin(),
		it_end = files.end();
		it != it_end;
		++it )
	{
		const std::wstring & path = *it;

		WCHAR sheetPath[MAX_PATH];
		PathCombine( sheetPath, dataTempDir, path.c_str() );

		FILE * f = _wfopen( sheetPath, L"rb" );

		if( f == NULL )
		{
			message_error( "invalid _wfopen %ls\n"
				, sheetPath
				);

			return 0;
		}

		fseek( f, 0, SEEK_END );
		int f_size = ftell( f );
		rewind( f );

		void * buff = malloc( f_size );
		fread( buff, f_size, 1, f );
		fclose( f );
		
		json_error_t jerror;
		json_t * root = json_loadb( (const char *)buff, f_size, 0, &jerror );

		json_t * meta = json_object_get( root, "meta" );

		json_t * meta_image = json_object_get( meta, "image" );		
		const char * atlasname = json_string_value( meta_image );

		json_t * meta_size = json_object_get( meta, "size" );

		json_t * meta_size_w = json_object_get( meta_size, "w" );
		json_int_t atlas_w = json_integer_value( meta_size_w );

		json_t * meta_size_h = json_object_get( meta_size, "h" );
		json_int_t atlas_h = json_integer_value( meta_size_h );

		json_t * frames = json_object_get( root, "frames" );

		size_t frames_size = json_array_size( frames );

		for( size_t i = 0; i != frames_size; ++i )
		{
			json_t * frame_data = json_array_get( frames, i );

			json_t * frame_data_filename = json_object_get( frame_data, "filename" );

			const char * filename = json_string_value( frame_data_filename );

			json_t * frame_data_sourceSize = json_object_get( frame_data, "sourceSize" );

			json_t * frame_data_sourceSize_w = json_object_get( frame_data_sourceSize, "w" );
			json_int_t fw = json_integer_value( frame_data_sourceSize_w );

			json_t * frame_data_sourceSize_h = json_object_get( frame_data_sourceSize, "h" );
			json_int_t fh = json_integer_value( frame_data_sourceSize_h );

			json_t * frame_data_spriteSourceSize = json_object_get( frame_data, "spriteSourceSize" );
			
			json_t * frame_data_spriteSourceSize_x = json_object_get( frame_data_spriteSourceSize, "x" );
			json_int_t ox = json_integer_value( frame_data_spriteSourceSize_x );

			json_t * frame_data_spriteSourceSize_y = json_object_get( frame_data_spriteSourceSize, "y" );
			json_int_t oy = json_integer_value( frame_data_spriteSourceSize_y );

			json_t * frame_data_frame = json_object_get( frame_data, "frame" );

			json_t * frame_data_frame_x = json_object_get( frame_data_frame, "x" );
			json_int_t x = json_integer_value( frame_data_frame_x );

			json_t * frame_data_frame_y = json_object_get( frame_data_frame, "y" );
			json_int_t y = json_integer_value( frame_data_frame_y );

			json_t * frame_data_frame_w = json_object_get( frame_data_frame, "w" );
			json_int_t w = json_integer_value( frame_data_frame_w );

			json_t * frame_data_frame_h = json_object_get( frame_data_frame, "h" );
			json_int_t h = json_integer_value( frame_data_frame_h );

			json_t * frame_data_frame_rotated = json_object_get( frame_data_frame, "rotated" );
			bool rotated = json_boolean_value( frame_data_frame_rotated );

			AtlasImageDesc image;
			image.name = filename;
			image.atlas = atlasname;

			image.aw = atlas_w;
			image.ah = atlas_h;

			image.ox = ox;
			image.oy = oy;

			image.ow = w;
			image.oh = h;

			image.fw = fw;
			image.fh = fh;

			if( rotated == true )
			{
				image.x0 = x + 0;
				image.y0 = y + h;
				image.x1 = x + 0;
				image.y1 = y + 0;
				image.x2 = x + w;
				image.y2 = y + 0;
				image.x3 = x + w;
				image.y3 = y + h;
			}
			else
			{
				image.x0 = x + 0;
				image.y0 = y + 0;
				image.x1 = x + w;
				image.y1 = y + 0;
				image.x2 = x + w;
				image.y2 = y + h;
				image.x3 = x + 0;
				image.y3 = y + h;
			}
			
			atlas_images.push_back( image );
		}
	}
		
	for( std::vector<AtlasImageDesc>::const_iterator
		it = atlas_images.begin(),
		it_end = atlas_images.end();
		it != it_end;
		++it )
	{
		const AtlasImageDesc & image = *it;

		///////////////n//a//aw///ah///ox///oy///ow///oh///x0///y0...
		printf( "image=%s;%s;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu;%llu\n"
			, image.name.c_str()
			, image.atlas.c_str()
			, image.aw
			, image.ah
			, image.ox
			, image.oy
			, image.ow
			, image.oh
			, image.fw
			, image.fh
			, image.x0
			, image.y0
			, image.x1
			, image.y1
			, image.x2
			, image.y2
			, image.x3
			, image.y3
		);
	}

	return 0;
}