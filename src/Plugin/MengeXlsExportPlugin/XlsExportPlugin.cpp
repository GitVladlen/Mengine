#	include "XlsExportPlugin.h"

#	include "Interface/LogSystemInterface.h"
#	include "Logger/Logger.h"

#	include "Interface/ApplicationInterface.h"
#	include "Interface/UnicodeInterface.h"

#   include "XlsScriptLogger.h"

#   include "WindowsLayer/WindowsIncluder.h"

#	include <pybind\pybind.hpp>

//////////////////////////////////////////////////////////////////////////
bool dllCreatePlugin( Menge::PluginInterface ** _plugin )
{
	*_plugin = new Menge::XlsExportPlugin();
	return true;
}

//PyAPI_DATA(int) Py_OptimizeFlag;
//extern int Py_NoSiteFlag;
//extern int Py_IgnoreEnvironmentFlag;
//extern int Py_VerboseFlag;
//extern int Py_NoUserSiteDirectory;

extern "C"
{
	extern PyObject* PyInit_pyexpat(void);
}
//////////////////////////////////////////////////////////////////////////
namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	XlsExportPlugin::XlsExportPlugin()
        : m_serviceProvider(NULL)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool XlsExportPlugin::initialize( ServiceProviderInterface * _serviceProvider )
	{
        m_serviceProvider = _serviceProvider;

		const ConstString & projectCodename = APPLICATION_SERVICE(m_serviceProvider)
            ->getProjectCodename();

        if( projectCodename.empty() == true )
        {
            return false;
        }
	
		//Py_IgnoreEnvironmentFlag = 1;
		//Py_VerboseFlag = 1;
		//Py_NoUserSiteDirectory = 1;
		//Py_NoSiteFlag = 1;

        String currentPath = PLATFORM_SERVICE(m_serviceProvider)
			->getCurrentPath();

        {
            String exportPath = currentPath + "Python3Lib\\";

            char utf8_shortpath[MENGINE_MAX_PATH * 2];

            size_t utf8_shortpath_len = PLATFORM_SERVICE(m_serviceProvider)
				->getShortPathName( exportPath, utf8_shortpath, MENGINE_MAX_PATH * 2 );

            wchar_t unicode_shortpath[MENGINE_MAX_PATH];

            size_t unicode_shortpath_len;

            if( UNICODE_SERVICE(m_serviceProvider)
                ->utf8ToUnicode( utf8_shortpath, utf8_shortpath_len, unicode_shortpath, MAX_PATH, &unicode_shortpath_len ) == false )
            {
                LOGGER_ERROR(m_serviceProvider)("ScriptEngine.updateModulePath_: invalid filepath '%s'"
                    , exportPath.c_str()
                    );

                return false;
            }

            pybind::set_path( unicode_shortpath );
        }        

		pybind::initialize( false, false );

        PyObject * xls_module = pybind::module_init( "Xls" );

        pybind::set_currentmodule( xls_module );

        XlsScriptLogger * logger = new XlsScriptLogger(m_serviceProvider);

        PyObject * pyLogger = logger->embedding();
        pybind::setStdOutHandle( pyLogger );

        XlsScriptLoggerError * errorLogger = new XlsScriptLoggerError(m_serviceProvider);

        PyObject * pyErrorLogger = errorLogger->embedding();
        pybind::setStdErrorHandle( pyErrorLogger );

        PyObject * py_syspath = pybind::list_new(0);

		{
			String stdPath = currentPath + "Python3Lib\\";

			char utf8_shortpath[MENGINE_MAX_PATH * 2];
			size_t utf8_shortpath_len = PLATFORM_SERVICE(m_serviceProvider)
				->getShortPathName( stdPath, utf8_shortpath, MENGINE_MAX_PATH * 2 );

			wchar_t unicode_shortpath[MENGINE_MAX_PATH];

			size_t unicode_shortpath_len;

			if( UNICODE_SERVICE(m_serviceProvider)
				->utf8ToUnicode( utf8_shortpath, utf8_shortpath_len, unicode_shortpath, MENGINE_MAX_PATH, &unicode_shortpath_len ) == false )
			{
				LOGGER_ERROR(m_serviceProvider)("ScriptEngine.updateModulePath_: invalid filepath '%s'"
					, stdPath.c_str()
					);

				return false;
			}

			PyObject * py_stdPath = pybind::unicode_from_wchar_size( unicode_shortpath, unicode_shortpath_len );

			pybind::list_appenditem( py_syspath, py_stdPath );

			pybind::decref( py_stdPath );
		}

		{
			String stdPath = currentPath + "XlsxExport\\";

			char utf8_shortpath[MENGINE_MAX_PATH * 2];
			size_t utf8_shortpath_len = PLATFORM_SERVICE(m_serviceProvider)
				->getShortPathName( stdPath, utf8_shortpath, MENGINE_MAX_PATH * 2 );

			wchar_t unicode_shortpath[MENGINE_MAX_PATH];

			size_t unicode_shortpath_len;

			if( UNICODE_SERVICE(m_serviceProvider)
				->utf8ToUnicode( utf8_shortpath, utf8_shortpath_len, unicode_shortpath, MENGINE_MAX_PATH, &unicode_shortpath_len ) == false )
			{
				LOGGER_ERROR(m_serviceProvider)("ScriptEngine.updateModulePath_: invalid filepath '%s'"
					, stdPath.c_str()
					);

				return false;
			}

			PyObject * py_stdPath = pybind::unicode_from_wchar_size( unicode_shortpath, unicode_shortpath_len );

			pybind::list_appenditem( py_syspath, py_stdPath );

			pybind::decref( py_stdPath );
		}

		pybind::set_syspath( py_syspath );

		pybind::decref( py_syspath );		

		this->proccess_( projectCodename.c_str() );

		pybind::finalize();

        delete logger;
        delete errorLogger;

        return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void XlsExportPlugin::finalize()
	{

	}
	//////////////////////////////////////////////////////////////////////////
	void XlsExportPlugin::destroy()
	{
		delete this;
	}
	//////////////////////////////////////////////////////////////////////////
	bool XlsExportPlugin::proccess_( const char * _projectName )
	{
		bool exist = false;
		PyObject * py_xlsxExporter = pybind::module_import( "xlsxExporter", exist );

		if( py_xlsxExporter == 0 )
		{
			return false;
		}

		PyObject * module_builtins = pybind::get_builtins();

		pybind::def_functor( "Error", this, &XlsExportPlugin::error_, module_builtins );

		pybind::call_method( py_xlsxExporter, "export", "(s)"
			, _projectName
			);

        pybind::decref( py_xlsxExporter );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void XlsExportPlugin::error_( const wchar_t * _msg )
	{
		LOGGER_ERROR(m_serviceProvider)("%ls"
			, _msg
			);
	}
}