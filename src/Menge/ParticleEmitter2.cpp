#	include "ParticleEmitter2.h" 

#	include "Logger/Logger.h"

#   include "Interface/RenderSystemInterface.h"
#   include "Interface/EventInterface.h"
#   include "Interface/ApplicationInterface.h"

#	include "Camera2D.h"

#	include "Player.h"

#	include "Kernel/Scene.h"
#	include "Kernel/Layer.h"

#   include "Interface/ParticleSystemInterface.h"

#	include "ResourceEmitterContainer.h"
#	include "ResourceImageDefault.h"
#	include "ResourceHIT.h"

#   include "Interface/ResourceInterface.h"

#   include "Interface/AlphaChannelInterface.h"

#	include "Sprite.h"
#	include "Consts.h"

#	include "math/box2.h"
#	include "math/angle.h"
//#	include "math/triangulation.h"

#   include "pybind/pybind.hpp"

#	include "Core/RenderUtil.h"

namespace	Menge
{
    //////////////////////////////////////////////////////////////////////////
#	ifndef MENGINE_PARTICLE_MAX_COUNT
#	define MENGINE_PARTICLE_MAX_COUNT 2000
#	endif
	//////////////////////////////////////////////////////////////////////////
#	ifndef MENGINE_PARTICLE_MAX_MESH
#	define MENGINE_PARTICLE_MAX_MESH 100
#	endif
	//////////////////////////////////////////////////////////////////////////
	ParticleEmitter2::ParticleEmitter2()
		: m_emitter(nullptr)
		, m_startPosition(0.f)
        , m_randomMode(false)
		, m_renderCamera3D(nullptr)
		, m_renderViewport(nullptr)
		, m_vertices(nullptr)
		, m_verticesCount(0)
        , m_emitterRelative(false)
        , m_emitterPosition(0.f, 0.f, 0.f)
        , m_emitterTranslateWithParticle(true)
		, m_invalidateMaterial(true)
	{
		for( size_t i = 0; i != MENGINE_PARTICLE_MAX_ATLAS_TEXTURE; ++i )
		{
			m_materials[i * 2 + 0] = nullptr;
			m_materials[i * 2 + 1] = nullptr;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	ParticleEmitter2::~ParticleEmitter2()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_setEventListener( PyObject * _listener )
	{
		Node::_setEventListener( _listener );

		this->registerEvent( EVENT_PARTICLE_EMITTER_END, ("onParticleEmitterEnd"), _listener );
		this->registerEvent( EVENT_PARTICLE_EMITTER_RESTART, ("onParticleEmitterRestart"), _listener );
	}
	///////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::_activate()
	{
		if( Node::_activate() == false )
		{
			return false;
		}

		this->invalidateWorldMatrix();
        
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_deactivate()
	{
		this->stop();

		Node::_deactivate();		
	}
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::_compile()
	{      		
		if( m_resourceParticle == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::_compile '%s' resource is null"
				, m_name.c_str()
				);

			return false;
		}

        if( m_resourceParticle.compile() == false )
        {
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::_compile '%s' resource '%s' not compile"
                , m_name.c_str()
                , m_resourceParticle->getName().c_str()
                );

            return false;
        }

		const ParticleEmitterContainerInterface2Ptr & container = m_resourceParticle->getContainer();

		if( container == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::_compile '%s' can't open container file '%s'"
				, m_name.c_str()
				, m_resourceParticle->getName().c_str() 
				);

			return false;
		}

		m_emitter = container->createEmitter();

		if( m_emitter == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("ParticleEmitter '%s' can't create emitter source '%s'"
				, m_name.c_str()
				, m_resourceParticle->getName().c_str()
				);

			return false;
		}

		m_emitter->setEmitterTranslateWithParticle( m_emitterTranslateWithParticle );

		if( m_emitterImageName.empty() == false )
		{
			if( this->compileEmitterImage_() == false )
			{
				return false;
			}
		}

        m_emitter->setRandomMode( m_randomMode );

        bool loop = this->getLoop();
        m_emitter->setLoop( loop );

        size_t polygon_count = boost::geometry::num_points( m_polygon );

        if( polygon_count != 0 )
        {
            if( this->compilePolygon_() == false )
            {
                return false;
            }
        }

		if( m_emitter->isBackground() == false )
		{
			m_emitter->setPosition( m_emitterPosition );
		}

		if( m_emitter->is3d() == true )
		{
			if( this->createCamera3D_() == false )
			{
				return false;
			}
		}

		this->invalidateMaterial_();

		return true;		
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_release()
	{
        m_emitter = nullptr;

        m_resourceParticle.release();

		delete [] m_vertices;        
		m_vertices = nullptr;

		m_verticesCount = 0;

        m_batchs.clear();
		
		for( size_t i = 0; i != MENGINE_PARTICLE_MAX_ATLAS_TEXTURE; ++i )
		{
			m_materials[i * 2 + 0] = nullptr;
			m_materials[i * 2 + 1] = nullptr;
		}

		this->destroyCamera3D_();
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_render( const RenderViewportInterface * _viewport, const RenderCameraInterface * _camera )
	{		
		Node::_render( _viewport, _camera );

		bool enabled = APPLICATION_SERVICE(m_serviceProvider)
			->getParticlesEnabled();

		if( enabled == false )
		{
			return;
		}

		if( this->isPlay() == false)
		{
			return;
		}

        if( m_vertices == nullptr )
        {
            return;
        }
		
		for( TVectorBatchs::const_iterator
			it = m_batchs.begin(),
			it_end = m_batchs.end();
		it != it_end;
		++it )
		{
			const Batch & batch = *it;

            RenderVertex2D * batch_vertices = m_vertices + batch.begin;

            if( batch.begin + batch.size > m_verticesCount )
            {
                return;
            }

            RENDER_SERVICE(m_serviceProvider)
				->addRenderQuad( _viewport, _camera, batch.material, batch_vertices, batch.size, &batch.bb );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::_play( float _time )
	{
        (void)_time;

		if( this->isActivate() == false )
		{
			return false;
		}

		m_emitter->play();

		//TODO!???
		if( m_startPosition > 0.f )
		{
			bool stop;
			m_emitter->update( m_startPosition, stop );
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::_restart( float _time, size_t _enumerator )
	{
        (void)_time;
        (void)_enumerator;

		if( this->isActivate() == false )
		{
			return false;
		}

		EVENTABLE_CALL(m_serviceProvider, this, EVENT_PARTICLE_EMITTER_RESTART)( "(OiO)", this->getEmbed(), _enumerator, pybind::get_bool(false) );

		m_emitter->restart();

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::pause()
	{
		if( this->isActivate() == false )
		{
			return;
		}

		m_emitter->pause();
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_stop( size_t _enumerator )
	{
		if( this->isActivate() == false )
		{
			return;
		}

		m_emitter->stop();

		EVENTABLE_CALL(m_serviceProvider, this, EVENT_PARTICLE_EMITTER_END)( "(OiO)", this->getEmbed(), _enumerator, pybind::get_bool(false) );		
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_end( size_t _enumerator )
	{
		EVENTABLE_CALL(m_serviceProvider, this, EVENT_PARTICLE_EMITTER_END)( "(OiO)", this->getEmbed(), _enumerator, pybind::get_bool(true) );
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_setLoop( bool _value )
	{
		if( this->isCompile() == false )
		{
			return;
		}

		m_emitter->setLoop( _value );
	}
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::_interrupt( size_t _enumerator )
	{
        (void)_enumerator;

		if( this->isCompile() == false )
		{
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::_interrupt '%s' can't compile"
                , m_name.c_str()
                );

			return false;
		}

		m_emitter->interrupt();

		//this->callEventDeferred( EVENT_PARTICLE_EMITTER_END, "(OiO)", this->getEmbed(), _enumerator, pybind::get_bool(true) );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::setLeftBorder( float _leftBorder )
	{
		if( this->isCompile() == false )
		{
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::setLeftBorder '%s' can't compile"
                , m_name.c_str()
                );

			return;
		}

		return m_emitter->setLeftBorder( _leftBorder );
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_update( float _current, float _timing )
	{
		if( this->isPlay() == false )
		{
			return;
		}

		bool enabled = APPLICATION_SERVICE(m_serviceProvider)
			->getParticlesEnabled();

		if( enabled == false )
		{
			return;
		}

		if( m_playTime > _current )
		{
			float deltha = m_playTime - _current;
			_timing -= deltha;
		}

		bool stop;
		m_emitter->update( _timing, stop );
		
		this->updateParticleVertex_();

		if( stop == true )
		{
			this->end();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::updateParticleVertex_()
	{
		size_t partCount = 0;
				
		Node::_updateBoundingBox( m_localBoundingBox );

        ParticleVertices s_particles[MENGINE_PARTICLE_MAX_COUNT];
		ParticleMesh s_meshes[MENGINE_PARTICLE_MAX_MESH];

		m_batchs.clear();
		        
		ParticleEmitterRenderFlush flush;
		if( m_emitter->flushParticles( s_meshes, MENGINE_PARTICLE_MAX_MESH, s_particles, MENGINE_PARTICLE_MAX_COUNT, flush ) == false )
		{
			return false;
		}

		if( m_verticesCount < flush.particleCount * 4 )
		{			
			m_verticesCount = flush.particleCount * 4;

			delete [] m_vertices;
			m_vertices = nullptr;

			m_vertices = new RenderVertex2D [m_verticesCount];
		}

		ColourValue color;
		this->calcTotalColor(color);

		ARGB color_argb = color.getAsARGB();
		
		for( size_t
			it_mesh = 0,
			it_mesh_end = flush.meshCount;
		it_mesh != it_mesh_end;
		++it_mesh )
		{
			const ParticleMesh & mesh = s_meshes[it_mesh];

			ResourceImage * image = m_resourceParticle->getAtlasImageResource( mesh.texture );

			const RenderTextureInterfacePtr & texture = image->getTexture();

			const mt::vec4f & mesh_uv = texture->getUV();

			for( size_t
				it = mesh.begin,
				it_end = mesh.begin + mesh.size;
			it != it_end;
			++it )
			{
				const ParticleVertices & p = s_particles[it];

				uint32_t argb;

				if( color.isIdentity() == true )
				{
					argb = p.color;
				}
				else if( p.color == 0xFFFFFFFF )
				{
					argb = color_argb;
				}
				else
				{
					ColourValue cv( ARGB(p.color) );
					cv *= color;
					argb = cv.getAsARGB();
				}

				RenderVertex2D * vertice = &m_vertices[it * 4];

                const mt::vec3f & wm_pos0 = p.v[0];

                vertice[0].pos.x = wm_pos0.x;
                vertice[0].pos.y = wm_pos0.y;
				vertice[0].pos.z = wm_pos0.z;

				vertice[0].color = argb;

                const mt::vec3f & wm_pos1 = p.v[1];

                vertice[1].pos.x = wm_pos1.x;
                vertice[1].pos.y = wm_pos1.y;
				vertice[1].pos.z = wm_pos1.z;

                vertice[1].color = argb;
		
                const mt::vec3f & wm_pos2 = p.v[2];

				vertice[2].pos.x = wm_pos2.x;
				vertice[2].pos.y = wm_pos2.y;
				vertice[2].pos.z = wm_pos2.z;

				vertice[2].color = argb;

                const mt::vec3f & wm_pos3 = p.v[3];

                vertice[3].pos.x = wm_pos3.x;
                vertice[3].pos.y = wm_pos3.y;
				vertice[3].pos.z = wm_pos3.z;

				vertice[3].color = argb;
				
				mt::vec2f uv[4];

				uv[0].x = mesh_uv.x + (mesh_uv.z - mesh_uv.x) * p.uv[0].x;
				uv[0].y = mesh_uv.y + (mesh_uv.w - mesh_uv.y) * p.uv[0].y;
				uv[1].x = mesh_uv.x + (mesh_uv.z - mesh_uv.x) * p.uv[1].x;
				uv[1].y = mesh_uv.y + (mesh_uv.w - mesh_uv.y) * p.uv[1].y;
				uv[2].x = mesh_uv.x + (mesh_uv.z - mesh_uv.x) * p.uv[2].x;
				uv[2].y = mesh_uv.y + (mesh_uv.w - mesh_uv.y) * p.uv[2].y;
				uv[3].x = mesh_uv.x + (mesh_uv.z - mesh_uv.x) * p.uv[3].x;
				uv[3].y = mesh_uv.y + (mesh_uv.w - mesh_uv.y) * p.uv[3].y;

				vertice[0].uv.x = uv[0].x;
				vertice[0].uv.y = uv[0].y;
				vertice[1].uv.x = uv[1].x;
				vertice[1].uv.y = uv[1].y;
				vertice[2].uv.x = uv[2].x;
				vertice[2].uv.y = uv[2].y;
				vertice[3].uv.x = uv[3].x;
				vertice[3].uv.y = uv[3].y;

                vertice[0].uv2.x = 0.f;
                vertice[0].uv2.y = 0.f;
                vertice[1].uv2.x = 0.f;
                vertice[1].uv2.y = 0.f;
                vertice[2].uv2.x = 0.f;
                vertice[2].uv2.y = 0.f;
                vertice[3].uv2.x = 0.f;
                vertice[3].uv2.y = 0.f;
			}

			++partCount;

			Batch batch;
			batch.begin = mesh.begin * 4;
			batch.size = mesh.size * 4;
			
			batch.material = this->getMaterial( mesh.texture * 2 + (mesh.intense ? 0 : 1) );

			m_batchs.push_back( batch );
		}

        if( m_emitterRelative == false )
        {
            this->updateVertexWM_();
        }

		this->updateBB_();

		PARTICLE_SERVICE(m_serviceProvider)
			->renderParticlesCount( partCount );

        return true;
	}
    //////////////////////////////////////////////////////////////////////////
    void ParticleEmitter2::updateVertexWM_()
    {
        const mt::mat4f & wm = this->getWorldMatrix();

        if( mt::is_ident_m34( wm ) == true )
        {
            return;
        }

        for( TVectorBatchs::const_iterator
            it = m_batchs.begin(),
            it_end = m_batchs.end();
        it != it_end;
        ++it )
        {
            const Batch & batch = *it;

            for( size_t 
                index = batch.begin, 
                index_end = batch.begin + batch.size; 
            index != index_end; 
            ++index )
            {
                mt::vec3f & pos = m_vertices[index].pos;

                mt::vec3f wm_pos;
                mt::mul_v3_m4( wm_pos, pos, wm);

                pos = wm_pos;
            }			
        }
    }
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::updateBB_()
	{
		for( TVectorBatchs::iterator
			it = m_batchs.begin(),
			it_end = m_batchs.end();
		it != it_end;
		++it )
		{
			Batch & batch = *it;

			Helper::makeRenderBoundingBox( batch.bb, m_vertices + batch.begin, batch.size );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::invalidateMaterial_()
	{
		m_invalidateMaterial = true;
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::updateMaterial_()
	{
		m_invalidateMaterial = false;

		size_t imageCount = m_resourceParticle->getAtlasImageCount();

		if( imageCount > MENGINE_PARTICLE_MAX_ATLAS_TEXTURE )
		{
			LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::updateMaterial_ %s particle resource %s max atlas texture %d"
				, this->getName().c_str()
				, m_resourceParticle->getName().c_str()
				, MENGINE_PARTICLE_MAX_ATLAS_TEXTURE
				);

			return;
		}

		for( size_t i = 0; i != imageCount; ++i )
		{
			ResourceImage * image = m_resourceParticle->getAtlasImageResource( i );

			const RenderTextureInterfacePtr & texture = image->getTexture();

			const RenderMaterialInterfacePtr & mg_intensive = RENDERMATERIAL_SERVICE(m_serviceProvider)
				->getMaterial( CONST_STRING(m_serviceProvider, ParticleIntensive), false, false, PT_TRIANGLELIST, 1, &texture );

			const RenderMaterialInterfacePtr & mg_nonintensive = RENDERMATERIAL_SERVICE(m_serviceProvider)
				->getMaterial( CONST_STRING(m_serviceProvider, ParticleBlend), false, false, PT_TRIANGLELIST, 1, &texture );

			m_materials[i*2 + 0] = mg_intensive;
			m_materials[i*2 + 1] = mg_nonintensive;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::restart()
	{
		if( this->isActivate() == false )
		{
			return;
		}

		m_emitter->restart();
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::setResourceParticle( ResourceParticle * _resourceParticle )
	{
		if( m_resourceParticle == _resourceParticle )
		{
			return;
		}

		m_resourceParticle = _resourceParticle;

		this->recompile();
	}
    //////////////////////////////////////////////////////////////////////////
    ResourceParticle * ParticleEmitter2::getResourceParticle() const
    {
        return m_resourceParticle;
    }
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::playFromPosition( float _pos )
	{
		if( this->isActivate() == false )
		{
			return;
		}

		m_emitter->seek( _pos );
		//m_interface->setLeftBorder( _pos );
		//m_interface->restart();
		//m_interface->play();
		//m_interface->update( _pos );
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::setEmitterTranslateWithParticle( bool _with )
	{
		m_emitterTranslateWithParticle = _with;
		
		if( this->isCompile() == false )
		{
			return;
		}

		m_emitter->setEmitterTranslateWithParticle( m_emitterTranslateWithParticle );
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::setEmitterPosition( const mt::vec3f & _position )
	{
		m_emitterPosition = _position;

		if( this->isCompile() == false )
		{
			return;
		}

		if( m_emitter->isBackground() == false )
		{
			m_emitter->setPosition( m_emitterPosition );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::setEmitterRelative( bool _relative )
	{
        m_emitterRelative = _relative;

		this->invalidateWorldMatrix();
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::setStartPosition( float _pos )
	{
		m_startPosition = _pos;
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::setEmitterImage( const ConstString & _emitterImageName )
	{
		m_emitterImageName = _emitterImageName;

        //this->removeEmitterPolygon();

		if( this->isCompile() == false )
		{
			return;
		}

		this->compileEmitterImage_();
	}
    //////////////////////////////////////////////////////////////////////////
    void ParticleEmitter2::removeEmitterImage()
    {
        m_emitterImageName.clear();

        if( this->isCompile() == false )
        {
            return;
        }

        m_emitter->changeEmitterImage( 0, 0, 0, 1 );
    }
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::compileEmitterImage_()
	{
		ResourceHIT * resourceHIT = RESOURCE_SERVICE(m_serviceProvider)
			->getResourceT<ResourceHIT>(m_emitterImageName);

		if( resourceHIT == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("ParticleEmitter '%s' can't compile emitter hit %s"
				, m_name.c_str()
				, m_emitterImageName.c_str()
				);

			return false;
		}

		size_t alphaWidth = resourceHIT->getWidth();
		size_t alphaHeight = resourceHIT->getHeight();

        unsigned char * alphaBuffer = resourceHIT->getBuffer();
        
		if( m_emitter->changeEmitterImage( alphaWidth, alphaHeight, alphaBuffer, 1 ) == false)
		{
			LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::setEmitterImage %s changeEmitterImage Error image %s"
                , m_name.c_str()
                , m_emitterImageName.c_str()
                );

            resourceHIT->decrementReference();

			return false;
		}
				
		resourceHIT->decrementReference();

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::changeEmitterPolygon( const Polygon & _polygon )
	{
        m_polygon = _polygon;

        boost::geometry::correct( m_polygon );

        if( this->isCompile() == false ) 
		{
			return true;
		}

        if( this->compilePolygon_() == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool ParticleEmitter2::compilePolygon_()
    {
        size_t n = boost::geometry::num_points( m_polygon );

        if( n == 0 )
        {
            return true;
        }

		TVectorPoints points;
		if( triangulate_polygon( m_polygon, points ) == false )
		{
			LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::changeEmitterPolygon '%s' wrong polygon"
				, m_name.c_str()
				);

			return false;
		}

		if( points.empty() == true )
		{
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::changeEmitterPolygon '%s' empty points"
                , m_name.c_str()
                );

			return false;
		}
        
        float * triangles_ptr = points.front().buff();
        size_t triangles_size = points.size() / 3;

        if( m_emitter->changeEmitterModel( triangles_ptr, triangles_size ) == false)
		{
			LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::changeEmitterPolygon '%s' changeEmitterModel Error polygon"
				, m_name.c_str()
				);

			return false;
		}

        return true;
	}
    //////////////////////////////////////////////////////////////////////////
    void ParticleEmitter2::removeEmitterPolygon()
    {
        m_polygon = Polygon();
        
        if( this->isCompile() == false )
        {
            return;
        }

        m_emitter->changeEmitterModel( nullptr, 0 );
    }
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_updateBoundingBox( mt::box2f& _boundingBox )
	{
        (void)_boundingBox;
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::_invalidateWorldMatrix()
	{
        if( this->isCompile() == false )
        {
            return;
        }

        if( m_emitterRelative == true )
        {
            const mt::vec3f & wm_pos = this->getWorldPosition();

            m_emitter->setPosition( wm_pos );
        }
	}
	/////////////////////////////////////////////////////////////////////////
	float ParticleEmitter2::getDuration() const
	{
        if( this->isCompile() == false )
        {
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::getDuration '%s' can't compile"
                , m_name.c_str()
                );

            return 0.f;
        }

        float duration = m_emitter->getDuration();

		return duration;
	}
	/////////////////////////////////////////////////////////////////////////
	float ParticleEmitter2::getLeftBorder() const
	{
        if( this->isCompile() == false )
        {
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::getLeftBorder '%s' can't compile"
                , m_name.c_str()
                );

            return 0.f;
        }

        float leftBoard = m_emitter->getLeftBorder();

		return leftBoard;
	}
	/////////////////////////////////////////////////////////////////////////
	float ParticleEmitter2::getRightBorder() const
	{
        if( this->isCompile() == false )
        {
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::getRightBorder '%s' can't compile"
                , m_name.c_str()
                );

            return 0.f;
        }

        float rightBoard = m_emitter->getRightBorder();

		return rightBoard;
	}
	//////////////////////////////////////////////////////////////////////////
	mt::box2f ParticleEmitter2::getEmitterBoundingBox() const
	{
		mt::box2f box;

        if( this->isCompile() == false )
        {
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::getEmitterBoundingBox '%s' can't compile"
                , m_name.c_str()
                );

            return box;
        }

		m_emitter->getBoundingBox( box );

		return box;
	}
	//////////////////////////////////////////////////////////////////////////
	mt::vec3f ParticleEmitter2::getEmitterPosition() const
	{
		mt::vec3f pos;

        if( this->isCompile() == false )
        {
            LOGGER_ERROR(m_serviceProvider)("ParticleEmitter::getEmitterPosition '%s' can't compile"
                , m_name.c_str()
                );

            return pos;
        }

		m_emitter->getPosition( pos );

		return pos;
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::setRandomMode( bool _randomMode )
	{
        m_randomMode = _randomMode;

        if( this->isCompile() == false )
        {
            return;
        }

		m_emitter->setRandomMode( _randomMode );
	}
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::getRandomMode() const
	{
        return m_randomMode;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ParticleEmitter2::createCamera3D_()
	{
		Camera3D * renderCamera3D = NODE_SERVICE(m_serviceProvider)
			->createNodeT<Camera3D>( CONST_STRING(m_serviceProvider, Camera3D) );

		if( renderCamera3D == nullptr )
		{
			return false;
		}

		const ConstString & name = this->getName();
		renderCamera3D->setName( name );

		ParticleCamera camera;
		if( m_emitter->getCamera( camera ) == false )
		{
			return false;
		}

		renderCamera3D->setCameraPosition( camera.pos );
		renderCamera3D->setCameraDir( camera.dir );
		renderCamera3D->setCameraUp( camera.up );

		renderCamera3D->setCameraFOV( camera.fov );
		renderCamera3D->setCameraAspect( camera.aspect );
		
		renderCamera3D->setCameraNear( camera.znear );
		renderCamera3D->setCameraFar( camera.zfar );
		
		renderCamera3D->setCameraRightSign( -1.f );

		Viewport rp;
		rp.begin.x = 0.f;
		rp.begin.y = 0.f;

		rp.end.x = camera.width;
		rp.end.y = camera.height;

		renderCamera3D->setRenderport( rp );		

		RenderViewport * renderViewport = NODE_SERVICE(m_serviceProvider)
			->createNodeT<RenderViewport>( CONST_STRING(m_serviceProvider, RenderViewport) );

		if( renderViewport == nullptr )
		{
			return false;
		}

		renderViewport->setName( name );

		Viewport vp;
		vp.begin.x = 0.f;
		vp.begin.y = 0.f;

		vp.end.x = camera.width;
		vp.end.y = camera.height;

		renderViewport->setViewport( vp );

		this->addChildren( renderCamera3D );
		this->addChildren( renderViewport );

		this->setRenderCamera( renderCamera3D );
		this->setRenderViewport( renderViewport );

		m_renderCamera3D = renderCamera3D;
		m_renderViewport = renderViewport;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void ParticleEmitter2::destroyCamera3D_()
	{
		if( m_renderCamera3D != nullptr )
		{
			m_renderCamera3D->destroy();
			m_renderCamera3D = nullptr;
		}

		if( m_renderViewport != nullptr )
		{
			m_renderViewport->destroy();
			m_renderViewport = nullptr;
		}
	}
}