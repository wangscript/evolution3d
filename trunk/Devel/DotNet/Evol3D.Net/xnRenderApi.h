// Evol3D.Net.h

#pragma once

#include "BaseLib/xCfgParser.h"
#include "BaseLib/xLogger.h"
#include "OperationSys/xOperationSys.h"
#include "Application/xPluginMgr.h"
#include "Application/xPlatform.h"
#include "Application/xApplication.h"
#include "RenderApi/xRenderApi.h"
#include "RenderAPI/xBaseShader.h"
#include "RenderAPI/xShaderNode.h"
#include "RenderAPI/xShaderManager.h"
#include <tchar.h>
#include "XMathLib/xMathlib.h"
#include "Image/xILImage.h"
#include "RenderAPI/xBaseTexture.h"
#include "RenderAPI/xTextureManager.h"
#include "RenderAPI/xRenderAPI.h"
#include "xDotNetUlti.h"
#include "xnMath.h"

using namespace XEvol3D;
using namespace System;

namespace xEvol3D 
{
	ref class   xColorF;


#undef  ENUMCLASS
#define ENUMCLASS( v ) public enum class


#include "Image/xPixelFormatEnum.h"
#include "RenderAPI/xRenderApiEnum.h"
#include "RenderAPI/xShaderSemanticEnum.h"

	public enum  class eCameraProject
	{
		PT_PROJECT = 0, 
		PT_ORTHO   = 1, 
		PT_FRUSTUM = 2  // Not implemented
	};

	public enum  class eFontFilter
	{
		eFontFilter_Linear,
		eFontFilter_Point,
		eFontFilter_SubPixel,
	};

	public enum  class eTextAlign
	{
		eAlign_Center,
		eAlign_Left,
		eAlign_Right,
		eAlign_Top,
		eAlign_Bottom,
	};

	public ref   class xCamera
	{
	internal:
		float           m_cameraStep;
		IRenderCamera*  m_obj;
	private:
		void             setProjectType(eCameraProject type)   { m_obj->setProjectType( (xGeomLib::xCamera::PROJECT_T)type); }
		eCameraProject   getProjectType()                      { return (eCameraProject)m_obj->getProjectType(); }
        void             setEyePos(xVector^ eyePos ){m_obj->m_Eye.m_EyePos    = eyePos->xvec(); }
		void             setEyeTag(xVector^ eyeTarg){m_obj->m_Eye.m_EyeTarget = eyeTarg->xvec(); }
		void             setEyeUp (xVector^ eyeUp  ){m_obj->m_Eye.m_Up        = eyeUp->xvec(); }
		xVector^         getEyePos(){ return gcnew xVector(&m_obj->m_Eye.m_EyePos   ) ;};
		xVector^         getEyeTag(){ return gcnew xVector(&m_obj->m_Eye.m_EyeTarget) ;};
		xVector^         getEyeUp (){ return gcnew xVector(&m_obj->m_Eye.m_Up       ) ;};

	public:
		xCamera(IRenderCamera* pCamera) {m_obj = pCamera; }
		void yaw(float angle)                                            { m_obj->yaw(angle) ;    }
		void pitch(float angle)                                          { m_obj->pitch(angle) ;  }
		void roll(float angle)                                           { m_obj->roll(angle) ;   }
		void circle(float angle)                                         { m_obj->circle(angle) ; }
		void toward(float dist)                                          { m_obj->toward(dist) ;  }
		void upDown(float dist)                                          { m_obj->upDown(dist) ;  }
		void shift(float  dist)                                          { m_obj->shift (dist) ;  }

		void toward()                                                    { m_obj->toward(m_cameraStep) ;  }
		void up()                                                        { m_obj->upDown(m_cameraStep) ;  }
		void shiftLeft()                                                 { m_obj->shift (m_cameraStep) ;  }
															   
		void back()                                                      { m_obj->toward(-m_cameraStep) ;  }
		void down()                                                      { m_obj->upDown(-m_cameraStep) ;  }
		void shiftRight()                                                { m_obj->shift (-m_cameraStep) ;  }

		void rotate(xVector^ axis,float angle)                           { m_obj->rotate ( *axis->m_vec , angle) ; }
		void focus(float dist,float factor )                             { m_obj->focus ( dist , factor) ; }
		void setPerspective2D(int w , int h , float fov, bool bResetEye) { m_obj->setPerspective2D ( w , h , fov,  bResetEye) ; }
		void setViewRect(float  l,float r,float t,float b)               { m_obj->setViewRect ( l, r, t, b ) ; }

		PF_ReadWrite(eCameraProject  , projType ,  this  , getProjectType ,  setProjectType , , ); 
		PF_ReadWrite(float           , fov      ,  m_obj , getFOV         ,  setFOV         , , );
		PF_ReadWrite(float           , nearPlan ,  m_obj , getNearPlan    ,  setNearPlan    , , );
		PF_ReadWrite(float           , farPlan  ,  m_obj , getFarPlan     ,  setFarPlan     , , );
		PF_ReadWrite(float           , aspect   ,  m_obj , getAspect      ,  setAspect      , , );
		PF_ReadWrite(xVector^        , EyePos   , this   , getEyePos      ,  setEyePos      , , );
		PF_ReadWrite(xVector^        , EyeTarget, this   , getEyeTag      ,  setEyeTag      , , );
		PF_ReadWrite(xVector^        , EyeUp    , this   , getEyeUp       ,  setEyeUp       , , );
		PS_ReadWrite(float           , CameraStep, m_cameraStep , ,) ;
	};

	public ref   class xRenderObject
	{
	internal:
		IRenderObject* m_pRenderObject;
		xRenderObject(IRenderObject* pObject) { m_pRenderObject = pObject        ; }
		~xRenderObject()                      { if(m_pRenderObject) m_pRenderObject->ReleaseObject() ; }
	public:
		PF_ReadOnly(UInt32, Handle , m_pRenderObject , handle ,, );
	};
	public ref   class xRenderResource
	{
	internal:
		IRenderResource* m_pResource;
		xRenderResource(IRenderResource* pResource) : m_pResource(pResource) {};
		~xRenderResource(){if(m_pResource) m_pResource->ReleaseObject() ; }
	public:
		PF_ReadOnly(eResourceType, ResourceType , m_pResource , res_type , , );
		PF_ReadOnly(unsigned long, memUsage     , m_pResource , memUsage , , );
		PF_ReadOnly(Boolean      , isLoaded     , m_pResource , isLoaded , , );
		bool  load(String^ _fileName, unsigned long  arg) {	pin_ptr<const wchar_t> fileName   = PtrToStringChars(_fileName); return m_pResource->load(fileName , arg);	} ;
		bool  unload() {return m_pResource->unload() ; }
		//bool        load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	};

	public ref   class xRenderState : public xRenderObject
	{
	internal:
		IRenderState* m_pState;
		xRenderState(IRenderState* pState):xRenderObject(pState) { m_pState = pState ; };
		~xRenderState() {}
	public:
		PF_ReadOnly(eRenderStateType, StateType , m_pState , state_type ,, );
	};

	public ref   class xSamplerState : public xRenderState
	{
	internal:
		ISamplerState* m_pSampler;
		xSamplerState(ISamplerState* pState) :m_pSampler(pState) , xRenderState(pState) {} ;
		~xSamplerState(){};
	public:
		xSamplerState() : m_pSampler(NULL) , xRenderState(NULL) {};
	};

	public ref   class xBlenderState : public xRenderState
	{
	internal:
		IBlenderState* m_pBlender;
		xBlenderState(IBlenderState* pState) : xRenderState(pState) {m_pBlender = pState; } ;
		~xBlenderState(){};
		void     setFactor(xColorF^ cl) { m_pBlender->set_factor( *cl->m_cl) ; } ;
		xColorF^ getFactor()   {	float* f = m_pBlender->factor();	return gcnew xColorF(f[0] , f[1] , f[2] , f[3]) ; } 
	public:
		xBlenderState() : m_pBlender(NULL) , xRenderState(NULL) {};
		PF_ReadWrite(xColorF^, factor     , this       , getFactor   , setFactor      , ,);
		PF_ReadWrite(UInt32  , samplemask , m_pBlender , samplemask  , set_samplemask , ,);
	};
	public ref   class xDepthStencilState : public xRenderState
	{
	internal:
		IDepthStencilState* m_pZStencil;
		xDepthStencilState(IDepthStencilState* pState) : xRenderState(pState) {m_pZStencil = pState; } ;
		~xDepthStencilState(){};
	public:
		xDepthStencilState() : m_pZStencil(NULL) , xRenderState(NULL) {};
		PF_ReadWrite(UInt32 , stencilRef , m_pZStencil ,  stencil_ref  ,  stencil_ref , , )
	};
	public ref   class xRasterizerState : public xRenderState
	{
	internal:
		IRasterizerState * m_pRaszier;
		xRasterizerState(IRasterizerState* pState) : xRenderState(pState) {m_pRaszier = pState; } ;
		~xRasterizerState(){};
	public:
		xRasterizerState() : m_pRaszier(NULL) , xRenderState(NULL) {};
	};

	//字体对象
	
	public ref   class xFont
	{
	internal:
		HFontRender* m_pRendeDevice;
	public:
		//virtual const  xFontInfo& getInfo() = 0;
		//virtual bool  drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, xColor_4f cl) = 0;

		PF_ReadWrite(Boolean , antialias , m_pRendeDevice->getResource() , isAntialias  , enableAntialias , ,);
        PF_ReadWrite(int     , cacheSize , m_pRendeDevice->getResource() , getCacheSize , setCacheSize    , ,);
		PF_ReadOnly(int      , height	 , m_pRendeDevice->getResource() , getFontHeight, ,); 
		PF_ReadOnly(int      , linePitch , m_pRendeDevice->getResource() , getLinePitch , ,); 
        xFont(HFontRender pFontRender);
		~xFont();
		void  releaseChache();
		bool  getCharDim(wchar_t _chr , int^ w , int^ h) ;
		bool  getTextDim(String^ text , int^ w , int^ h);
		bool  getTextDim(String^ strText, int^ dx , int^ dy , int maxWidth);
		int	  getTextWidth(String^ strText, int iBegin, int^ nLen);
		int	  getCharCount(String^ strText, int iBegin, int^ nMaxWidth, bool bRoundVal);

		//公共函数
		bool  drawText(String^strText, float x , float y , int maxWidth , xColorF^ cl );
		bool  drawText(String^ strText, float x , float y ,xColorF^ cl );
		bool  drawText(String^ strText, float x , float y , int iBgnIdx, int nCount, xColorF^ cl, int nMaxW );
		bool  drawTextOneLine(String^ strText, xVector^ rect, xColorF^ cl, eTextAlign hAlignMode , eTextAlign vAlignMode);

	};


	public ref   class xGpuProgram : public xRenderResource
	{
	internal:
		IGpuProgram* m_pProgram;
		xGpuProgram(IGpuProgram* pProgram): xRenderResource(pProgram) , m_pProgram(pProgram) {}
		~xGpuProgram() {} 
	public:
	};

	public ref   class xBaseTexture : public xRenderResource
	{
	internal :
		IBaseTexture* m_pBaseTexture;
		xBaseTexture(IBaseTexture* pTexture) : m_pBaseTexture(pTexture) , xRenderResource(pTexture) {};
		~xBaseTexture() {}
	public:
		xBaseTexture() :  m_pBaseTexture(NULL) , xRenderResource(NULL) {};
	};

	public ref   class HBaseTexture 
	{
	internal :
		XEvol3D::HBaseTexture* m_hBaseTexture;
		HBaseTexture(XEvol3D::HBaseTexture hTexture){ m_hBaseTexture = new XEvol3D::HBaseTexture , *m_hBaseTexture = hTexture ; }
		~HBaseTexture() {delete m_hBaseTexture ;}
		IBaseTexture* getResource(){ return m_hBaseTexture->getResource() ; };
	public:		
	};

	public ref class xBaseTextureManager 
	{
	internal:
		xBaseTextureMgr* m_pTexMgr;
		IRenderApi*      m_Api;
	public:
		~xBaseTextureManager();
		xBaseTextureManager(xRenderApi^ pApi , String^ name , bool bNonRefKeep);
		void addPackage(String^ pkgName , String^ _dir);
		void addPath(String^ pathName);
		HBaseTexture^ load(String^ texName) 
		{
			XEvol3D::HBaseTexture hTexture = m_pTexMgr->add(ToPtr(texName) ,0 , true);
			return gcnew HBaseTexture(hTexture);
		}
	};

	public ref   class x2DRectObject
	{
	internal:
		I2DRectObject* m_pObject;
		x2DRectObject(I2DRectObject*  pObject): m_pObject(pObject) {};
		~x2DRectObject(){};
		void     setColor(xColorF^ cl)       { m_pObject->setColor( *cl->m_cl) ; }
		void     setRect (xVector^ destRect) { m_pObject->setRect( destRect->m_vec->v ) ; }
	public:
		PF_ReadWrite(UInt32        , nUVLayer  , m_pObject , nUVChanel   , setUVChanel,  , );
		PF_WriteOnly(xColorF^      , Color     , this      , setColor    , , );
		PF_WriteOnly(xVector^      , Rect      , this      , setRect    , , );
		void     setTexture(int iStage , xBaseTexture^ pTexture)                    { m_pObject->setTexture(iStage , pTexture->m_pBaseTexture) ; }
		void     setTexture(int iStage , xBaseTexture^ pTexture , xVector^ srcRect) { m_pObject->setTexture(iStage , pTexture->m_pBaseTexture , srcRect->data() ) ; }
		void     commit()  {return m_pObject->commit() ; };
		bool     draw()    {return m_pObject->draw()   ; };

	};

	public ref   class xFontRenderDevice
	{
	internal:
		IFontRenderDevice* m_pRenderDevice;
		xFontRenderDevice(IFontRenderDevice* pDevice) :m_pRenderDevice(pDevice) {} ;
		~xFontRenderDevice(){};
		bool            setShaderProgram(xGpuProgram^ pProgram) { return m_pRenderDevice->setShaderProgram(pProgram->m_pProgram)  ; }
		x2DRectObject^  get2DObject(){ return gcnew x2DRectObject(m_pRenderDevice->get2DObject() ); }
		void            setFontFilter(eFontFilter fontFilter) {m_pRenderDevice->setFontFilter(XEvol3D::eFontFilter (fontFilter) ) ; }
	public:
		PF_ReadOnly(x2DRectObject^ , RectObject, this            , get2DObject  , , );
		PF_WriteOnly(UInt32        , nUVLayer  , m_pRenderDevice , setUVLayer   , , );
        PF_WriteOnly(eFontFilter   , FontFilter, this            , setFontFilter, , );
		PF_WriteOnly(xGpuProgram^  , GpuShader , this            , setShaderProgram ,, );
		bool isTextureSupport(ePIXEL_FORMAT fmt , bool lockable) {return m_pRenderDevice->isTextureSupport((XEvol3D::ePIXEL_FORMAT)fmt , lockable) ; }
		bool drawRectf(xBaseTexture^ pTexture, xVector^ vDestRect , xColorF^ color)
		{
			return m_pRenderDevice->drawRectf(pTexture->m_pBaseTexture , vDestRect->data() , *color->m_cl) ; 
		}
		bool drawRectf(xBaseTexture^ pTexture, xVector^ vDestRect , xVector^ vSrcRect  , xColorF^ color)
		{
			return m_pRenderDevice->drawRectf(pTexture->m_pBaseTexture , vDestRect->data() , vSrcRect->data() , *color->m_cl) ; 
		}
		xBaseTexture^   createLockableTexture(int w , int h , ePIXEL_FORMAT fmt ,bool bReadable, int nMipMap, int nArraySize)
		{
			IBaseTexture* pTexture = m_pRenderDevice->createLockableTexture(w , h , (XEvol3D::ePIXEL_FORMAT)fmt , bReadable , nMipMap , nArraySize);
			return gcnew xBaseTexture(pTexture);
		}
	};
	
	
	public ref   class xShaderProgramName
	{
	internal:
		xGpuProgramName*    m_pName;
		xGpuProgNameParser* m_pParser;
		xGpuProgramName&    name() ;
	public:
		xShaderProgramName();
		~xShaderProgramName();
		void  set(String^ vs , String^ ps , String^ gs);
		void  set(String^ _shaderName, bool bLoadFromeFile);
		bool  setShaderName(eShaderType _shaderType , String^ _shaderName);
		void  addShaderNode(eShaderType _shaderType , String^ _nodeName, String^_socketName);
		void  addShaderNode(eShaderType _shaderType , String^ _nodeName, size_t _socketIdx);
	};

	
	//RenderApi对象
	public ref   class xRenderApi
	{
	internal:
		IRenderApi*     m_pRenderApi;
	public:
		xRenderApi(IRenderApi*  pRenderApi);
		bool     onResize(int w , int h){  return m_pRenderApi->onResize(w , h );	}
		bool     begin(float r , float g , float b , float a , float z , unsigned int stencil);
		bool     beginScene(){  return m_pRenderApi->beginScene();	}
		bool     endScene()  {  return m_pRenderApi->endScene();	}
		bool     swapBuffer(){  return m_pRenderApi->swapBuffer();	}
		bool     multiMatrix(xMatrix^ mat , eMatrixMode matMode){	return m_pRenderApi->multiMatrix(mat->data() , (XEvol3D::eMatrixMode) matMode);	}
		bool     setMatrix(xMatrix^   mat, eMatrixMode matMode) {  return m_pRenderApi->setMatrix(mat->data()   , (XEvol3D::eMatrixMode) matMode);	}
		bool     identityMatrix(eMatrixMode matMode){	return m_pRenderApi->identityMatrix((XEvol3D::eMatrixMode) matMode );	}
		bool     getMatrix(xMatrix^ mat, eMatrixMode matMode){	return m_pRenderApi->getMatrix(mat->data() , (XEvol3D::eMatrixMode) matMode);	}
		bool     pushMatrix(eMatrixMode matMode){	return m_pRenderApi->pushMatrix( (XEvol3D::eMatrixMode) matMode);	}
		bool     popMatrix(eMatrixMode matMode){ return m_pRenderApi->popMatrix((XEvol3D::eMatrixMode) matMode );	}
		xMatrix^ getWorldMatrix()   { xMatrix^ mat = gcnew xMatrix() ; getMatrix(mat , eMatrixMode::MATRIXMODE_World)   ; return mat; } 
		xMatrix^ getViewMatrix()    { xMatrix^ mat = gcnew xMatrix() ; getMatrix(mat , eMatrixMode::MATRIXMODE_View)    ; return mat; }     
		xMatrix^ getProjectMatrix() { xMatrix^ mat = gcnew xMatrix() ; getMatrix(mat , eMatrixMode::MATRIXMODE_Project) ; return mat; } 
		xMatrix^ getTextureMatrix() { xMatrix^ mat = gcnew xMatrix() ; getMatrix(mat , eMatrixMode::MATRIXMODE_Texture) ; return mat; } 
        
		void     setTextureMatrix(xMatrix^ mat) {  setMatrix(mat , eMatrixMode::MATRIXMODE_Texture) ;} 
		void     setWorldMatrix  (xMatrix^ mat) {  setMatrix(mat , eMatrixMode::MATRIXMODE_World)   ;} 
		void     setViewMatrix   (xMatrix^ mat) {  setMatrix(mat , eMatrixMode::MATRIXMODE_View)    ;}     
		void     setProjectMatrix(xMatrix^ mat) {  setMatrix(mat , eMatrixMode::MATRIXMODE_Project) ;} 

		PF_ReadWrite(xMatrix^ , WorldMatrix   , this , getWorldMatrix    , setWorldMatrix    , , );
		PF_ReadWrite(xMatrix^ , ViewMatrix    , this , getViewMatrix     , setViewMatrix     , , );
		PF_ReadWrite(xMatrix^ , ProjectMatrix , this , getProjectMatrix  , setProjectMatrix  , , );
		PF_ReadWrite(xMatrix^ , TextureMatrix , this , getTextureMatrix  , setTextureMatrix  , , );

		bool  drawRectf(xVector^ vDestRect , xColorF^ color){return m_pRenderApi->drawRectf(NULL , vDestRect->data() , *color->m_cl) ;}

		bool  drawRectf(HBaseTexture^ hTexture, xVector^ vDestRect , xColorF^ color){return m_pRenderApi->drawRectf(hTexture->getResource() , vDestRect->data() , *color->m_cl) ;}
		bool  drawRectf(HBaseTexture^ hTexture, xVector^ vDestRect , xVector^ vSrcRect  , xColorF^ color){return m_pRenderApi->drawRectf( hTexture->getResource() , vDestRect->data() , vSrcRect->data() , *color->m_cl) ;}
		
		bool  drawRectf(xBaseTexture^ pTexture, xVector^ vDestRect , xColorF^ color){return m_pRenderApi->drawRectf(pTexture->m_pBaseTexture , vDestRect->data() , *color->m_cl) ;}
		bool  drawRectf(xBaseTexture^ pTexture, xVector^ vDestRect , xVector^ vSrcRect  , xColorF^ color){return m_pRenderApi->drawRectf(pTexture->m_pBaseTexture , vDestRect->data() , vSrcRect->data() , *color->m_cl) ;}
		void  clear2DTexture(int iStage) { m_pRenderApi->set2DTexture(iStage , NULL) ; }


		//句柄
		void  set2DTexture(int iStage , HBaseTexture^ hTexture) { m_pRenderApi->set2DTexture(iStage , hTexture->getResource() ) ; }
		void  set2DTexture(int iStage , HBaseTexture^ hTexture , xVector^ srcRect) { m_pRenderApi->set2DTexture(iStage , hTexture->getResource() , srcRect->data() ) ; }

		void  set2DTexture(int iStage , xBaseTexture^ pTexture) { m_pRenderApi->set2DTexture(iStage , pTexture->m_pBaseTexture) ; }
		void  set2DTexture(int iStage , xBaseTexture^ pTexture , xVector^ srcRect) { m_pRenderApi->set2DTexture(iStage , pTexture->m_pBaseTexture , srcRect->data() ) ; }
		bool  draw2DRect(x2DRectObject^ p2DRect){return m_pRenderApi->draw2DRect( p2DRect->m_pObject) ; }
		x2DRectObject^ create2DRectObject() {return gcnew x2DRectObject( m_pRenderApi->create2DRectObject() ) ;}
		//Vertex /Pixel / Geometry Shader
		//virtual IShaderConstBinder*   getShaderConstBinder(eShaderParamSemantic _semantic) = 0;
		//virtual IShaderConstBinder*   getShaderConstBinder(const wchar_t* pShaderConstName) = 0;
		//virtual IBaseShader*          createShader(eShaderType type) = 0;
		//virtual IBaseShader*          createShader(const wchar_t* shaderName , eShaderType type)= 0;
		xGpuProgram^ createGpuProgram() {return gcnew xGpuProgram( m_pRenderApi->createGpuProgram() ) ;}
		bool         setGpuProgram(xGpuProgram^ pGpuProgram){return m_pRenderApi->setGpuProgram(pGpuProgram->m_pProgram) ; }
		xGpuProgram^ getGpuProgram( ) { return gcnew xGpuProgram( m_pRenderApi->getGpuProgram() ) ; }
		PF_ReadWrite(xGpuProgram^ , GpuShader , this , getGpuProgram  , setGpuProgram , , );

		xGpuProgram^ loadGpuProgram(String^ _vsname , String^ _psname , String^ _gsname){ return  gcnew xGpuProgram(m_pRenderApi->gpuProgramManager()->load( ToPtr(_vsname) , ToPtr(_psname) ,ToPtr(_gsname) ).getResource() ); }
		xGpuProgram^ loadGpuProgram(xShaderProgramName^ _name){ return  gcnew xGpuProgram(m_pRenderApi->gpuProgramManager()->load(  _name->name() ).getResource() );	}
		//virtual bool                  setVertexShader(IBaseShader* pVertShader) = 0;
		//virtual bool                  setPixelShader(IBaseShader* pPixelShader) = 0;
		//virtual bool                  setGeomShader(IBaseShader* pGeomShader) = 0;

		//
		bool clearShaderResource(eShaderType _st ,int iSlot ) { return m_pRenderApi->setShaderResource( XEvol3D::eShaderType(_st) , iSlot , (IBaseTexture *)NULL) ; }
		bool setShaderResource(eShaderType _st   ,int iSlot ,xBaseTexture^ pTexture) { return m_pRenderApi->setShaderResource( XEvol3D::eShaderType(_st) , iSlot , pTexture->m_pBaseTexture) ; }
		//bool setShaderResource(eShaderType _st ,int iSlot , IInputBuffer* pBuffer)  = 0;
		bool clearTexture(eTextureState iTexName) { return m_pRenderApi->setTexture( (int)iTexName   , NULL) ; }
		bool clearTexture(const int iTexName    ) { return m_pRenderApi->setTexture(iTexName         , NULL) ; }
		bool clearTexture(String^ iTexName      ) { return m_pRenderApi->setTexture( ToPtr(iTexName) , NULL) ; }
		
		bool setTexture(eTextureState iTexName , HBaseTexture^ hTexture) { return m_pRenderApi->setTexture( (int)iTexName  , hTexture->getResource() ) ; }
		bool setTexture(const int iTexName     , HBaseTexture^ hTexture) { return m_pRenderApi->setTexture(iTexName        , hTexture->getResource() ) ; }
		bool setTexture(String^ iTexName       , HBaseTexture^ hTexture) { return m_pRenderApi->setTexture( ToPtr(iTexName), hTexture->getResource() ) ; }


		bool setTexture(eTextureState iTexName      , xBaseTexture^ pTexture) { return m_pRenderApi->setTexture( (int)iTexName  , pTexture->m_pBaseTexture) ; }
		bool setTexture(const int iTexName      , xBaseTexture^ pTexture) { return m_pRenderApi->setTexture(iTexName  , pTexture->m_pBaseTexture) ; }
		bool setTexture(String^ iTexName , xBaseTexture^ pTexture) { return m_pRenderApi->setTexture( ToPtr(iTexName) , pTexture->m_pBaseTexture) ; }
		xBaseTexture^    getTexture(const int iTexName) {return gcnew xBaseTexture( m_pRenderApi->getTexture(iTexName) ) ; }
		xBaseTexture^    getTexture(String^ iTexName)   {return gcnew xBaseTexture( m_pRenderApi->getTexture( ToPtr(iTexName) ) ) ; }
		bool             clearAllTexture()              {return m_pRenderApi->clearAllTexture() ; }

		//Texture Funciton
		//文件类型的纹理
		//通过资源来加载。
		//xBaseTexture^         createFileTexture(String^ texFile , const unsigned int8* buf , unsigned int bufLen) = 0 ;
		//通过扩展名,创建对象后，     需要自己调用load
		xBaseTexture^ loadFileTexture(String^ texFile) {return  gcnew xBaseTexture(m_pRenderApi->createFileTexture( ToPtr(texFile) , NULL , 0 )  ); }
		xBaseTexture^ createLockableTexture(int w , int h , ePIXEL_FORMAT fmt , bool bReadable ,   int nMipMap, int nArraySize )
		{
			XEvol3D::ePIXEL_FORMAT xfmt = (XEvol3D::ePIXEL_FORMAT)fmt; 
			IBaseTexture* pTexture = m_pRenderApi->createLockableTexture(w , h , xfmt , bReadable , nMipMap , nArraySize );
			return gcnew xBaseTexture(pTexture);
		}

		bool isTextureSupport(ePIXEL_FORMAT fmt , bool lockable) 
		{
			XEvol3D::ePIXEL_FORMAT xfmt = (XEvol3D::ePIXEL_FORMAT)fmt; 
			return m_pRenderApi->isTextureSupport(xfmt , lockable);
		}


		//Mem Buffer function
		//virtual IInputAssembler*      createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc ) = 0;
		//virtual IInputBuffer*         createInputBuffer(size_t nVert , size_t nBytePerVertex ,const xInputBufferDesc* pBufferDesc , void* pData = NULL)  = 0;
		//virtual IInputBuffer*         createBuffer(size_t iBufLen ,const xInputBufferDesc* pBufferDesc , void* pData = NULL)  = 0;


		xCamera^ createCamera(String^ name)   { return  gcnew xCamera(m_pRenderApi->createCamera( ToPtr(name)) ); }
		xFont^   findFont(String^ fontName)   { return gcnew xFont(m_pRenderApi->findFont( ToPtr(fontName) ) );}
		void     applyCamera(xCamera^ camera) { m_pRenderApi->applyCamera(camera->m_obj) ; }
		xCamera^ getCamera(){return  gcnew xCamera(m_pRenderApi->getCamera() );}
		PF_ReadWrite(xCamera^      , Camera      , this , getCamera         , applyCamera , , );

		//State Object create
		xRenderState^       createRenderState(eRenderStateType _type) {return gcnew xRenderState( m_pRenderApi->createRenderState(XEvol3D::eRenderStateType(_type) ) ); } 
		xSamplerState^      createSamplerState(String^ name)     { return gcnew xSamplerState( m_pRenderApi->createSamplerState( ToPtr(name) ) )  ;  }
		xRasterizerState^   createRasterizerState(String^ name)  { return gcnew xRasterizerState( m_pRenderApi->createRasterizerState( ToPtr(name) ) )  ;  }
		xBlenderState^      createBlendState(String^ name)       { return gcnew xBlenderState( m_pRenderApi->createBlendState( ToPtr(name) ) )  ;  }
		xDepthStencilState^ createDepthStencilState(String^ name){ return gcnew xDepthStencilState( m_pRenderApi->createDepthStencilState( ToPtr(name) ) )  ;  }

		bool                setSamplerState(eShaderType _st , int iSlot , xSamplerState^ pState){ return m_pRenderApi->setSamplerState( XEvol3D::eShaderType(_st) ,iSlot, pState->m_pSampler ) ;}
		bool                setBlendState(xBlenderState^ pState)            {return m_pRenderApi->setBlendState(pState->m_pBlender) ; }
		bool                setRasterizerState(xRasterizerState^ pState)    {return m_pRenderApi->setRasterizerState(pState->m_pRaszier) ; }
		bool                setDepthStencilState(xDepthStencilState^ pState){return m_pRenderApi->setDepthStencilState(pState->m_pZStencil) ; }
		
		xBlenderState^      getBlendState( )       {return gcnew xBlenderState(m_pRenderApi->getBlendState() ); }
		xRasterizerState^   getRasterizerState()   {return gcnew xRasterizerState(m_pRenderApi->getRasterizerState() ); }
		xDepthStencilState^ getDepthStencilState( ){return gcnew xDepthStencilState(m_pRenderApi->getDepthStencilState() ); }
	
		bool                setPSSamplerState(int iSlot , xSamplerState^ pState){return setSamplerState(eShaderType::eShader_PixelShader , iSlot , pState) ; }
		bool                setGSSamplerState(int iSlot , xSamplerState^ pState){return setSamplerState(eShaderType::eShader_GeometryShader , iSlot , pState) ; }
		bool                setVSSamplerState(int iSlot , xSamplerState^ pState){return setSamplerState(eShaderType::eShader_VertexShader , iSlot , pState) ; }
		//PFIDX_WriteOnly(xSamplerState^   , PSSampler    , this , setPSSamplerState , , );
		//PFIDX_WriteOnly(xSamplerState^   , GSSampler    , this , setGSSamplerState , , );
		//PFIDX_WriteOnly(xSamplerState^   , VSSampler    , this , setVSSamplerState , , );
		PF_ReadWrite(xBlenderState^      , Blender      , this , getBlendState         , setBlendState , , );
		PF_ReadWrite(xRasterizerState^   , Rasterizer   , this , getRasterizerState    , setRasterizerState , , );
		PF_ReadWrite(xDepthStencilState^ , DepthStencil , this , getDepthStencilState  , setDepthStencilState , , );

		bool                pushGpuProgram(xGpuProgram^ GpuProgram) { return m_pRenderApi->pushGpuProgram( GpuProgram->m_pProgram) ; }
		bool                popGpuProgram() {return m_pRenderApi->popGpuProgram() ; }


		
	};
}
