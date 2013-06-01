#	pragma once

#	include "ServiceInterface.h"

#   include "Interface/ImageCodecInterface.h"

#	include "Config/Typedef.h"

#	include "Core/Viewport.h"
#	include "Core/Resolution.h"
#	include "Core/Rect.h"
#	include "Core/ConstString.h"
#	include "Core/FilePath.h"
#	include "Core/PixelFormat.h"

#	include "Math/mat4.h"

#	define MENGE_MAX_TEXTURE_STAGES 4

namespace Menge
{
	class ServiceProviderInterface;

	enum EBlendFactor 
	{
		BF_ONE = 0,
		BF_ZERO,
		BF_DEST_COLOUR,
		BF_SOURCE_COLOUR,
		BF_ONE_MINUS_DEST_COLOUR,
		BF_ONE_MINUS_SOURCE_COLOUR,
		BF_DEST_ALPHA,
		BF_SOURCE_ALPHA,
		BF_ONE_MINUS_DEST_ALPHA,
		BF_ONE_MINUS_SOURCE_ALPHA,

		BF_FORCE_DWORD = 0x7fffffff
	};

	enum EPrimitiveType
	{
		PT_POINTLIST = 0,
		PT_LINELIST,
		PT_LINESTRIP,
		PT_TRIANGLELIST,
		PT_TRIANGLESTRIP,
		PT_TRIANGLEFAN,

		PT_FORCE_DWORD = 0x7fffffff
	};

	enum ETextureAddressMode
	{
		TAM_CLAMP = 0,
		TAM_WRAP,
		TAM_MIRROR,

		TAM_FORCE_DWORD = 0x7fffffff
	};

	enum ECullMode
	{
		CM_CULL_NONE = 0,
		CM_CULL_CW,
		CM_CULL_CCW,

		CM_CULL_FORCE_DWORD = 0x7fffffff
	};

	enum ECompareFunction
	{
		CMPF_ALWAYS_FAIL = 0,
		CMPF_ALWAYS_PASS,
		CMPF_LESS,
		CMPF_LESS_EQUAL,
		CMPF_EQUAL,
		CMPF_NOT_EQUAL,
		CMPF_GREATER_EQUAL,
		CMPF_GREATER,

		CMPF_FORCE_DWORD = 0x7fffffff
	};

	enum EFillMode
	{
		FM_POINT = 0,
		FM_WIREFRAME,
		FM_SOLID,

		FM_FORCE_DWORD = 0x7fffffff
	};

	enum EFrameBufferType 
	{
		FBT_COLOR	= 0x1,
		FBT_DEPTH   = 0x2,
		FBT_STENCIL = 0x4,

		FBT_FORCE_DWORD = 0x7fffffff
	};

	enum EShadeType
	{
		SHT_FLAT = 0,
		SHT_GOURAUD,
		SHT_PHONG,

		SHT_FORCE_DWORD = 0x7fffffff
	};

	enum ETextureOp
	{
		TOP_DISABLE = 0,
		TOP_SELECTARG1,
		TOP_SELECTARG2,
		TOP_MODULATE,
		TOP_ADD,
		TOP_SUBSTRACT,

		TOP_FORCE_DWORD = 0x7fffffff
	};

	enum ETextureArgument
	{
		TARG_CURRENT = 0,
		TARG_DIFFUSE,
		TARG_SPECULAR,
		TARG_TEXTURE,
		TARG_TFACTOR,

		TARG_FORCE_DWORD = 0x7fffffff
	};

	enum ETextureFilterType
	{
		TFT_MAGNIFICATION = 0,
		TFT_MINIFICATION,
		TFT_MIPMAP,

		TFT_FORCE_DWORD = 0x7fffffff
	};

	enum ETextureFilter
	{
		TF_NONE = 0,
		TF_POINT,
		TF_LINEAR,
		TF_ANISOTROPIC,
		TF_FLATCUBIC,
		TF_GAUSSIANCUBIC,

		TF_FORCE_DWORD = 0x7fffffff
	};

    enum ELogicPrimitiveType
    {
        LPT_QUAD = 0,
        //LPT_TRIANGLE,
        LPT_LINE,
        //LPT_RECTANGLE,

        LPT_PRIMITIVE_COUNT,

        LPT_MESH,

        LPT_FORCE_DWORD = 0x7FFFFFFF
    };

    const unsigned int LOCK_READONLY = 0x00000010L;
    const unsigned int LOCK_DISCARD = 0x00002000L;
    const unsigned int LOCK_NOOVERWRITE = 0x00001000L;
    const unsigned int LOCK_NOSYSLOCK = 0x00000800L;

	typedef size_t VBHandle; // Vertex Buffer Handle
	typedef size_t IBHandle; // Index Buffer Handle

    struct RenderTextureStage
    {
        RenderTextureStage()
            : addressU(TAM_CLAMP)
            , addressV(TAM_CLAMP)
            , filter(TF_LINEAR)
            , colorOp(TOP_DISABLE)
            , colorArg1(TARG_TEXTURE)
            , colorArg2(TARG_DIFFUSE)
            , alphaOp(TOP_DISABLE)
            , alphaArg1(TARG_TEXTURE)
            , alphaArg2(TARG_DIFFUSE)
            , texCoordIndex(0)
        {
        }

        ETextureAddressMode addressU;
        ETextureAddressMode addressV;

        ETextureFilter filter;

        ETextureOp colorOp;
        ETextureArgument colorArg1;
        ETextureArgument colorArg2;
        ETextureOp alphaOp;
        ETextureArgument alphaArg1;
        ETextureArgument alphaArg2;

        size_t texCoordIndex;
    };

    struct RenderMaterial
    {
        RenderMaterial()
            : blendSrc(BF_ONE)
            , blendDst(BF_ZERO)
            , depthBufferWriteEnable(false)
            , alphaTestEnable(false)
            , alphaBlendEnable(false)
            , filterMaterial(NULL)
        {
        }

        RenderTextureStage textureStage[MENGE_MAX_TEXTURE_STAGES];

        EBlendFactor blendSrc;
        EBlendFactor blendDst;

        bool depthBufferWriteEnable;
        bool alphaTestEnable;
        bool alphaBlendEnable;

        const RenderMaterial * filterMaterial;

        const RenderMaterial * getFilterMaterial() const
        {
            return filterMaterial;
        }
    };

    struct RenderMaterialGroup
    {
        RenderMaterial group[4];
        RenderMaterial filter_group[4];

        inline const RenderMaterial * getMaterial( ETextureAddressMode _textureU, ETextureAddressMode _textureV) const
        {
            return &group[_textureU + (_textureV * 2)];
        }
    };

    const int VDECL_XYZ = 0x002;
    const int VDECL_XYZRHW = 0x004;
    const int VDECL_NORMAL = 0x010;
    const int VDECL_DIFFUSE = 0x040;
    const int VDECL_SPECULAR = 0x080;
    const int VDECL_TEX1 = 0x100;
    const int VDECL_TEX2 = 0x200;

    struct Vertex2D
    {
        mt::vec3f pos;
        uint32 color;
        mt::vec2f uv;
        mt::vec2f uv2;
    };

    static const uint32 Vertex2D_declaration = VDECL_XYZ | VDECL_DIFFUSE | VDECL_TEX1 | VDECL_TEX2;

    struct Vertex3D
    {
        float pos[3];
        float n[3];
        uint32 color;
        float uv[2];
    };

    static const uint32 Vertex3D_declaration = VDECL_XYZ | VDECL_NORMAL | VDECL_DIFFUSE | VDECL_TEX1;

    typedef std::vector<uint16> TVectorIndices;
    typedef std::vector<Vertex2D> TVectorVertex2D;
    typedef std::vector<Vertex3D> TVectorVertex3D;

    class ApplyColor2D
    {
    public:
        ApplyColor2D( uint32 _argb )
            : m_argb( _argb )
        {
        }
        void operator()( Vertex2D& _vtx )
        {
            _vtx.color = m_argb;
        }

    protected:
        uint32 m_argb;
    };

	class RenderImageInterface
        : public FactorablePtr
	{
    public:
        virtual size_t getHWWidth() const = 0;
        virtual size_t getHWHeight() const = 0;
        virtual size_t getHWChannels() const = 0;

        virtual PixelFormat getHWPixelFormat() const = 0;

	public:	
        virtual unsigned char* lock( int* _pitch, const Rect& _rect, bool _readOnly = true ) = 0;
		virtual void unlock() = 0;
	};

    typedef IntrusivePtr<RenderImageInterface> RenderImageInterfacePtr;

	class RenderTextureInterface;

	class RenderTextureInterfaceListener
	{
	public:
		virtual void onRenderTextureRelease( const RenderTextureInterface * _texture ) = 0;
	};
	
	class RenderTextureInterface
        : public FactorablePtr
	{
	public:
		virtual RenderImageInterfacePtr getImage() const = 0;
                		
		virtual size_t getId() const = 0;

		virtual const Rect & getRect() const = 0;
		virtual const Rect & getHWRect() const = 0;

		virtual const mt::vec4f & getUV() const = 0;

		virtual void setFileName( const FilePath & _filename ) = 0;
		virtual const FilePath & getFileName() const = 0;

		virtual size_t getWidth() const = 0;
		virtual size_t getHeight() const = 0;
		virtual size_t getChannels() const = 0;

		virtual unsigned char* lock( int * _pitch, const Rect & _rect, bool _readOnly = true ) const = 0;
		virtual void unlock() const = 0;

		virtual size_t getHWWidth() const = 0;
		virtual size_t getHWHeight() const = 0;
        virtual size_t getHWChannels() const = 0;

		virtual PixelFormat getHWPixelFormat() const = 0;				

		virtual size_t getMemoryUse() const = 0;
	};
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<RenderTextureInterface> RenderTextureInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
	class RenderCameraInterface
	{
	public:
		virtual const mt::mat4f & getProjectionMatrix() const = 0;
		virtual const mt::mat4f & getViewMatrix() const = 0;
		virtual const Viewport & getViewport() const = 0;

	public:
		virtual const ConstString & getRenderTarget() const = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	class RenderSystemListener        
	{
	public:
		virtual bool onRenderSystemDeviceRestored() = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	class RenderSystemInterface
        : public ServiceInterface
	{
        SERVICE_DECLARE("RenderSystem")

	public:
		virtual bool initialize() = 0;
        virtual void finalize() = 0;

    public:
        virtual void setRenderListener( RenderSystemListener * _listener ) = 0;

		virtual bool createRenderWindow( size_t _width, size_t _height, int _bits, bool _fullscreen, WindowHandle _winHandle,
			bool _waitForVSync, int _FSAAType, int _FSAAQuality ) = 0;

    public:
        virtual void makeProjectionOrthogonal( mt::mat4f & _projectionMatrix, const Viewport & _viewport, float _near, float _far ) = 0;
        virtual void makeProjectionFrustum( mt::mat4f & _projectionMatrix, const Viewport & _viewport, float _near, float _far ) = 0;
        virtual void makeProjectionPerspective( mt::mat4f & _projectionMatrix, float _fov, float _aspect, float zn, float zf ) = 0;
        virtual void makeViewMatrixFromViewport( mt::mat4f & _viewMatrix, const Viewport & _viewport ) = 0;

    public:
		// Render frame into _image
		// int rect[4] - rectangle represents desired frame area in pixels
		virtual void screenshot( const RenderImageInterfacePtr & _image, const float * _rect ) = 0;
		// ������� ������: ������� 4 �� 4
		virtual	void setProjectionMatrix( const mt::mat4f & _projection ) = 0;
		virtual	void setModelViewMatrix( const mt::mat4f & _view ) = 0;
		virtual	void setWorldMatrix( const mt::mat4f & _view ) = 0;

		virtual void setTextureMatrix( size_t _stage, const float* _texture ) = 0;

		virtual VBHandle createVertexBuffer( size_t _verticesNum, size_t _vertexSize ) = 0;
		virtual void releaseVertexBuffer( VBHandle _vbHandle ) = 0;
		virtual void* lockVertexBuffer(  VBHandle _vbHandle, size_t _offset, size_t _size, uint32 _flags ) = 0;
		virtual bool unlockVertexBuffer( VBHandle _vbHandle ) = 0;
		virtual void setVertexBuffer( VBHandle _vbHandle ) = 0;

		virtual IBHandle createIndexBuffer( size_t _indiciesNum ) = 0;
		virtual void releaseIndexBuffer( IBHandle _ibHandle ) = 0;
		virtual uint16* lockIndexBuffer(  IBHandle _ibHandle ) = 0;
		virtual bool unlockIndexBuffer( IBHandle _ibHandle ) = 0;
		virtual void setIndexBuffer( IBHandle _ibHandle, size_t _baseVertexIndex ) = 0;

		virtual void setVertexDeclaration( size_t _vertexSize, uint32 _declaration ) = 0;

		virtual void drawIndexedPrimitive( EPrimitiveType _type, size_t _baseVertexIndex,
			size_t _minIndex, size_t _verticesNum, size_t _startIndex, size_t _indexCount ) = 0;

		virtual void setTexture( size_t _stage, const RenderImageInterfacePtr & _texture ) = 0;
		virtual void setTextureAddressing( size_t _stage, ETextureAddressMode _modeU, ETextureAddressMode _modeV ) = 0;
		virtual void setTextureFactor( uint32 _color ) = 0;
		virtual void setSrcBlendFactor( EBlendFactor _src ) = 0;
		virtual void setDstBlendFactor( EBlendFactor _dst ) = 0;
		virtual void setCullMode( ECullMode _mode ) = 0;
		virtual void setDepthBufferTestEnable( bool _depthTest ) = 0;
		virtual void setDepthBufferWriteEnable( bool _depthWrite ) = 0;
		virtual void setDepthBufferCmpFunc( ECompareFunction _depthFunction ) = 0;
		virtual void setFillMode( EFillMode _mode ) = 0;
		virtual void setColorBufferWriteEnable( bool _r, bool _g, bool _b, bool _a ) = 0;
		virtual void setShadeType( EShadeType _sType ) = 0;
		virtual void setAlphaTestEnable( bool _alphaTest ) = 0;
		virtual void setAlphaBlendEnable( bool _alphaBlend ) = 0;
		virtual void setAlphaCmpFunc( ECompareFunction _alphaFunc, uint8 _alpha ) = 0;
		virtual void setLightingEnable( bool _light ) = 0;
		virtual void setTextureStageColorOp( size_t _stage, ETextureOp _textrueOp,
												ETextureArgument _arg1, ETextureArgument _arg2 ) = 0;
		virtual void setTextureStageAlphaOp( size_t _stage, ETextureOp _textrueOp,
												ETextureArgument _arg1, ETextureArgument _arg2 ) = 0;

        virtual void setTextureStageTexCoordIndex( size_t _stage, size_t _index ) = 0;
		virtual void setTextureStageFilter( size_t _stage, ETextureFilterType _filterType, ETextureFilter _filter ) = 0;
		// create texture
		// [in/out] _width ( desired texture width, returns actual texture width )
		// [in/out] _height ( desired texture height, returns actual texture height )
		// [in/out] _format ( desired texture pixel format, returns actual texture pixel format )
		// returns Texture interface handle or NULL if fails
		virtual RenderImageInterfacePtr createImage( size_t _width, size_t _height, size_t _channels, PixelFormat _format ) = 0;
		virtual RenderImageInterfacePtr createDynamicImage( size_t _width, size_t _height, size_t _channels, PixelFormat _format ) = 0;
		// create render target image
		// [in/out] _width ( desired texture width, returns actual texture width )
		// [in/out] _height ( desired texture height, returns actual texture height )
		// returns Texture interface handle or NULL if fails
		virtual RenderImageInterfacePtr createRenderTargetImage( size_t _width, size_t _height, size_t _channels, PixelFormat _format ) = 0;

		//
		// ��������� �����������

		
		virtual bool beginScene() = 0;
		virtual void endScene() = 0;
		virtual void swapBuffers() = 0;
		virtual void clearFrameBuffer( uint32 _frameBufferTypes
											, uint32 _color = 0
											, float _depth = 1.0f
											, uint16 _stencil = 0 ) = 0;

		virtual void setViewport( const Viewport & _viewport ) = 0;

		virtual void changeWindowMode( const Resolution & _resolution, bool _fullscreen ) = 0;
		virtual bool setRenderTarget( const RenderImageInterfacePtr & _renderTarget, bool _clear ) = 0;

		//new
		//virtual LightInterface * createLight( const String & _name ) = 0;
		//virtual void releaseLight( LightInterface * _light ) = 0;

		virtual void onWindowMovedOrResized() = 0;
		virtual void onWindowClose() = 0;

		virtual void setVSync( bool _vSync ) = 0;
		virtual void clear( uint32 _color ) = 0;

        virtual bool supportTextureFormat( PixelFormat _format ) const = 0;

		virtual void setSeparateAlphaBlendMode() = 0;
	};

#   define RENDER_SYSTEM( serviceProvider )\
    (Menge::Helper::getService<Menge::RenderSystemInterface>(serviceProvider))

    struct RenderDebugInfo
    {
        size_t frameCount;
        size_t dips;
        size_t textureMemory;
        size_t textureCount;

        double fillrate;
        size_t object;
        size_t triangle;
        //size_t megatextures;
    };

	class RenderServiceInterface
		: public ServiceInterface
	{
        SERVICE_DECLARE("RenderService")

    public:
        virtual bool initialize( size_t _maxQuadCount ) = 0;
        virtual void finalize() = 0;

    public:
        virtual void addRenderObject2D( const RenderCameraInterface * _camera, const RenderMaterial* _material, const RenderTextureInterfacePtr * _textures, size_t _texturesNum,
            const Vertex2D * _vertices, size_t _verticesNum, 
            ELogicPrimitiveType _type, size_t _indicesNum = 0, IBHandle ibHandle = 0 ) = 0;

    public:
        virtual RenderTextureInterfacePtr loadTexture( const ConstString& _pakName, const FilePath& _filename, const ConstString& _codec, size_t _width, size_t _height ) = 0;

    public:
        virtual const RenderMaterialGroup * getMaterialGroup( const ConstString & _name ) const = 0;

	public:
		virtual bool createRenderWindow( const Resolution & _resolution, const Resolution & _contentResolution, const Viewport & _viewport, int _bits, bool _fullscreen, 
			WindowHandle _winHandle, int _FSAAType , int _FSAAQuality ) = 0;

		virtual void changeWindowMode( const Resolution & _resolution, const Resolution & _contentResolution, const Viewport & _viewport, bool _fullscreen ) = 0;

	public:
		virtual void clear( uint32 _color ) = 0;
		virtual bool beginScene() = 0;
		virtual void endScene() = 0;
		virtual void swapBuffers() = 0;
		virtual void screenshot( const RenderTextureInterfacePtr & _renderTargetImage, const mt::vec4f & _rect ) = 0;		
		virtual void setVSync( bool _vSync ) = 0;
		virtual bool getVSync() const = 0;
		virtual void setSeparateAlphaBlendMode() = 0;

	public:
		virtual RenderTextureInterfacePtr createTexture( size_t _width, size_t _height, size_t _channels, PixelFormat _format, size_t _textureWidth, size_t _textureHeight ) = 0;
		//virtual RenderTextureInterface * createSubTexture( RenderTextureInterface * _texture, const Rect & _rect, RenderTextureInterfaceListener * _listener ) = 0;
        virtual RenderTextureInterfacePtr createDynamicTexture( size_t _width, size_t _height, size_t _channels, PixelFormat _format ) = 0;
		virtual RenderTextureInterfacePtr createRenderTargetTexture( size_t _width, size_t _height, size_t _channels, PixelFormat _format ) = 0;

        virtual RenderTextureInterfacePtr getTexture( const FilePath& _filename ) = 0;

        virtual bool hasTexture( const FilePath & _filename, RenderTextureInterfacePtr * _texture ) const = 0;

    public:
        virtual void makeProjectionOrthogonal( mt::mat4f & _projectionMatrix, const Viewport & _viewport, float _near, float _far ) = 0;
        virtual void makeProjectionFrustum( mt::mat4f & _projectionMatrix, const Viewport & _viewport, float _near, float _far ) = 0;
        virtual void makeProjectionPerspective( mt::mat4f & _projectionMatrix, float _fov, float _aspect, float zn, float zf ) = 0;
        virtual void makeViewMatrixFromViewport( mt::mat4f & _viewMatrix, const Viewport & _viewport ) = 0;

	public:
		virtual bool loadTextureRectImageData( const RenderTextureInterfacePtr & _texture, const Rect & _rect, const ImageDecoderInterfacePtr & _imageDecoder ) = 0;

	public:
		virtual void setRenderTargetTexture( const RenderTextureInterfacePtr & _texture, bool _clear ) = 0;

    public:
        virtual void enableTextureFiltering( bool _enable ) = 0;
        virtual bool isTextureFilteringEnabled() const = 0;
        
    public:
        virtual void enableDebugMode( bool _enable ) = 0;

    public:
        virtual void onWindowClose() = 0;
        virtual bool isWindowCreated() const = 0;

    public:
        //virtual void sweezleAlpha( RenderTextureInterface * _texture, unsigned char * _textureBuffer, size_t _texturePitch ) = 0;
        virtual void imageQuality( const RenderTextureInterfacePtr & _texture, unsigned char * _textureBuffer, size_t _texturePitch ) = 0;

        virtual void cacheFileTexture( const FilePath& _filename, const RenderTextureInterfacePtr & _texture ) = 0;

    public:
        virtual const RenderDebugInfo & getDebugInfo() const = 0;
        virtual void resetFrameCount() = 0;
        
    public:
        virtual bool saveImage( const RenderTextureInterfacePtr & _texture, const ConstString& _fileSystemName, const FilePath & _filename ) = 0;
	};

#   define RENDER_SERVICE( serviceProvider )\
    (Menge::Helper::getService<Menge::RenderServiceInterface>(serviceProvider))
}
