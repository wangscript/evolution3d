#include "stdafx.h"
#include "TestScaleFont.h"
#include "float16/float16.h"
xTestFont::xTestFont()
{
	m_pTexture = NULL;
	m_pRenderApi = NULL;
}

xTestFont::~xTestFont()
{
}

void   xTestFont::unload()
{
	XSAFE_RELEASEOBJECT(m_pTexture);
}



bool xTestFont::load(IRenderApi* pRenderApi , const wchar_t * fileName , wchar_t ch)
{
	ds_wstring descFileName = ds_wstring(fileName) + L".dfd";
	FILE* descFile = _wfopen(descFileName.c_str() , L"rb+");
	int nChar = 0;
	fread(&nChar , 1 , sizeof(int) , descFile);
	this->m_pCharDesc = new xDFFCharDesc[nChar];
	fread(m_pCharDesc , sizeof(xDFFCharDesc) , nChar , descFile);
	fclose(descFile);

	wchar_t* ch_ = L"°¡";
	xDFFCharDesc& chDesc = m_pCharDesc[ch - ch_[0] ];
	chDesc.m_offset ;

	ds_wstring fontFileName = ds_wstring(fileName) + L".dff";
	FILE* file = _wfopen( fontFileName.c_str() , L"rb+");
	fseek(file,chDesc.m_offset,SEEK_SET);
	float16* tBuffer = new float16[chDesc.m_w * chDesc.m_h];
	fread(tBuffer , sizeof(float16) , chDesc.m_w * chDesc.m_h , file);
	fclose(file);

    m_pRenderApi = pRenderApi;
	m_pTexture = m_pRenderApi->createLockableTexture(chDesc.m_w , chDesc.m_h , PIXELFORMAT_R16F,false);
	xTextureLockArea _lckRect;
	m_pTexture->lock(eLock_WriteDiscard , _lckRect);
	for(int y = 0 ; y < chDesc.m_h ; y ++)
	{
		memcpy(_lckRect.m_pixels + y * _lckRect.m_picth , tBuffer + y * chDesc.m_w  , chDesc.m_w * sizeof( float16 ) );
	}
	m_pTexture->unlock(_lckRect);

	return false;
}


void xTestFont::draw()
{
	HGpuProgram hGpuProgram = m_pRenderApi->gpuProgramManager()->load(L"font.vertex" , L"vFont.pixel" , NULL);;
	m_pRenderApi->setGpuProgram( hGpuProgram.getResource() );
	m_pRenderApi->setBlendState(m_pRenderApi->createBlendState(L"Font.Blend"));
	m_pRenderApi->setDepthStencilState( m_pRenderApi->createDepthStencilState(L"Overlay") );
	xvec4     destFontRect(20,100,256,256);
	xColor_4f cl(1.0f,0.0f,1.0f,1.0f);
	m_pRenderApi->drawRectf(m_pTexture , &destFontRect.x, cl);
}

