#include "DebugRenderService.h"

#include "Kernel/Assertion.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_FACTORY( DebugRenderService, Mengine::DebugRenderService );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    DebugRenderService::DebugRenderService()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    DebugRenderService::~DebugRenderService()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void DebugRenderService::render( const NodePtr & _node, const RenderContext * _context, bool _external )
    {
        if( _node->isActivate() == false )
        {
            return;
        }

        RenderInterface * selfRender = _node->getRender();

        if( selfRender != nullptr )
        {
            if( selfRender->isRenderEnable() == false )
            {
                return;
            }

            if( selfRender->isHide() == true )
            {
                return;
            }

            if( selfRender->isLocalTransparent() == true )
            {
                return;
            }

            if( selfRender->isExternalRender() == true && _external == false )
            {
                return;
            }

            RenderContext self_context;

            const RenderViewportInterfacePtr & renderViewport = selfRender->getRenderViewport();

            if( renderViewport != nullptr )
            {
                self_context.viewport = renderViewport;
            }
            else
            {
                self_context.viewport = _context->viewport;
            }

            const RenderCameraInterfacePtr & renderCamera = selfRender->getRenderCamera();

            if( renderCamera != nullptr )
            {
                self_context.camera = renderCamera;
            }
            else
            {
                self_context.camera = _context->camera;
            }

            const RenderScissorInterfacePtr & renderScissor = selfRender->getRenderScissor();

            if( renderScissor != nullptr )
            {
                self_context.scissor = renderScissor;
            }
            else
            {
                self_context.scissor = _context->scissor;
            }

            const RenderTargetInterfacePtr & renderTarget = selfRender->getRenderTarget();

            if( renderTarget != nullptr )
            {
                self_context.target = renderTarget;
            }
            else
            {
                self_context.target = _context->target;
            }

            if( selfRender->isLocalHide() == false && selfRender->isPersonalTransparent() == false )
            {
                selfRender->render( &self_context );

                for( const RenderVisitorPtr & visitor : m_renderVisitors )
                {
                    visitor->setRenderContext( &self_context );

                    _node->visit( visitor );
                }
            }

            const RenderContext * children_context = &self_context;
            _node->foreachChildrenUnslug( [this, children_context]( const NodePtr & _child )
            {
                this->render( _child, children_context, false );
            } );

            if( self_context.target != nullptr )
            {
                const RenderInterfacePtr & targetRender = selfRender->makeTargetRender( &self_context );

                if( targetRender != nullptr )
                {
                    targetRender->render( _context );
                }
            }
        }
        else
        {
            _node->foreachChildrenUnslug( [this, _context]( const NodePtr & _child )
            {
                this->render( _child, _context, false );
            } );

            for( const RenderVisitorPtr & visitor : m_renderVisitors )
            {
                visitor->setRenderContext( _context );

                _node->visit( visitor );
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void DebugRenderService::addRenderVisitor( const RenderVisitorPtr & _renderVisitor )
    {
        m_renderVisitors.emplace_back( _renderVisitor );
    }
    //////////////////////////////////////////////////////////////////////////
    void DebugRenderService::removeRenderVisitor( const RenderVisitorPtr & _renderVisitor )
    {
        VectorRenderVisitor::iterator it_erase = std::find( m_renderVisitors.begin(), m_renderVisitors.end(), _renderVisitor );

        MENGINE_ASSERTION( it_erase != m_renderVisitors.end() );

        m_renderVisitors.erase( it_erase );
    }
}