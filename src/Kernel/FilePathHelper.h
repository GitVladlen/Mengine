#pragma once

#include "Kernel/FilePath.h"

namespace Mengine
{
    namespace Helper
    {
        FilePath emptyPath();
        FilePath concatenationFilePath( const FilePath & _left, const FilePath & _right );
        bool makeFullPath( const ConstString & _fileGroupName, const FilePath & _fileName, FilePath & _fullPath );
        FilePath getPathFolder( const FilePath & _fullpath );
        FilePath unicodeToFilePath( const WString & _unicode );
        void pathCorrectBackslash( WChar * _out );
        void pathCorrectBackslashTo( WChar * _out, const WChar * _in );
        void pathRemoveBackslash( WChar * _path );
        bool pathRemoveFileSpec( WChar * _in );
    }
}