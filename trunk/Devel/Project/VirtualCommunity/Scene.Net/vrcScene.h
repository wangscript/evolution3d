#include "../xVRCBaseInclude.h"
#include "../Scene/vrcSceneNode.h"
#include "../Scene/vrcSceneManager.h"
#include "../Scene/vrcModelNode.h"
namespace VirtualCommunity
{
	using namespace xEvol3DNet;
	public ref class xVRSceneNode
	{
	internal:
		xVR_SceneNode* m_pSceneNode;
		xVRSceneNode(xVR_SceneNode* pNode):m_pSceneNode(pNode) {};
		~xVRSceneNode(){ m_pSceneNode->ReleaseObject() ; };
	public:
		xVector3^  center() 
		{
			xvec3 v ; m_pSceneNode->center(v) ; 
			return gcnew xVector3(v.x , v.y , v.z) ; 
		}

		xAABB^     aabb()
		{ 
			xAABB^ _aabb = gcnew xAABB();
			m_pSceneNode->aabb( handle_refcast<xGeomLib::xaabb>(_aabb->Handle) );
			return _aabb;
		};

		xSphere^       shpere()
		{
			xSphere^ _shpere = gcnew xSphere();
			m_pSceneNode->shpere( handle_refcast<xGeomLib::xshpere>(_shpere->Handle ) );
			return _shpere;
		}

		void setTrans(xMatrix^ _mat)
		{
			m_pSceneNode->setTrans( handle_refcast<xMathLib::xmat4>(_mat->Handle ) );			
		}
		xMatrix^ getTrans( )
		{
			xMatrix^ _mat = gcnew xMatrix();
			m_pSceneNode->getTrans( handle_refcast<xMathLib::xmat4>(_mat->Handle) );
			return _mat;
		}

		xMatrix^ getLocalTrans()
		{
			xMatrix^ _mat = gcnew xMatrix();
			m_pSceneNode->getLocalTrans( handle_refcast<xMathLib::xmat4>(_mat->Handle) );
			return _mat;
		}

		bool        render(long passedTime){return m_pSceneNode->render(passedTime) ; }
		bool        update(long passedTime){return m_pSceneNode->update(passedTime) ; }
	public:
		xRenderApi^ renderApi(){ return  gcnew xRenderApi( (int)m_pSceneNode->renderApi() ) ; };
		String^     type(){return gcnew String( m_pSceneNode->type() ) ; }
	public:
		int             numOfChildren() {return m_pSceneNode->numOfChildren() ; }
		xVRSceneNode^   getChild(int index){return gcnew xVRSceneNode(m_pSceneNode->getChild(index)) ; }
		void            removeChild(xVRSceneNode^ pNode){ return m_pSceneNode->removeChild( pNode->m_pSceneNode) ; }
		String^         name(){return gcnew String(m_pSceneNode->name() ); }
		xVRSceneNode^   getParent() { return gcnew xVRSceneNode(m_pSceneNode->getParent()) ;  }
		void            setname(String^ _name) {return m_pSceneNode->setname( ToPtr(_name) ) ; }
		bool            insertChild(xVRSceneNode^ pNode){return m_pSceneNode->insertChild(pNode->m_pSceneNode) ; }
	};

	public ref class xVRSceneManager : public xVRSceneNode
	{
		XVR_SceneManager* m_pMgr;
		xVRSceneNode^     m_SelectedNode;
	public:
		~xVRSceneManager(){ m_pMgr->ReleaseObject() ; };
		xVRSceneManager(xRenderApi^ renderApi);
	public:
		void              SelectNode(int x ,int y);
		bool              update(unsigned long passedTime) { return m_pMgr->update(passedTime) ; }
		bool              render(unsigned long passedTime)  { return m_pMgr->render(passedTime) ; }
		xVRSceneNode^     getSelected(int x , int y ) {return gcnew xVRSceneNode(m_pMgr->getSelected(x , y) ); }
		bool              load(String^ sceneFile) { return m_pMgr->load( ToPtr(sceneFile)) ; }
		bool              save(String^ sceneFile) { return m_pMgr->save( ToPtr(sceneFile)) ; }
		String^           type() {return  gcnew String(m_pMgr->type()) ; }

        bool              addModelNode(String^ modelFile, xVRSceneNode^ sceneNode);
		bool              addModelPath(String^ modelPath);
		bool              addTexturePath(String^ texturePath);
	};
}