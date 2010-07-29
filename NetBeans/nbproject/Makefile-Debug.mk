#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneCommonObject.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xHDRImage.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat4.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/stl_dllalloc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Lzma2Enc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xApplication.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindow.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32OS.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xPluginMgr.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xPlacement.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat4x3.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/adler32.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xquat.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Linux/xLinuxOS.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xrotator.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneHelperDrawable.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xFreeImage.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/MemPool.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/MtCoder.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRendererQueue.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xBaseTerrain.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaLib.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xApplicationLayer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFontRender.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xIntersection.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/infback.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xstring.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdFile.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec3.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xJPEGImage.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Threads.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiButton.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xTransitionAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xMixAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdFileImp.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/Packager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/loki/src/SmallObj.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/ToyRender/xToyRenderer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Bra.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdocbase.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xMultiFtRender.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderSemantic.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/x2DRenderer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFontManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xILImage.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindowManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/loki/src/Singleton.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseModel.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/gzio.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/compress.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseMath.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreActionMgr.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xActorModel.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/zutil.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xParamTable.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneModel.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xDistanceFont.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xTLEnvioroment.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawElement.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTransBlend.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiCaption.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xEvol3D.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xAliasFont.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xStringTable.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xXrmModel.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiStateBlend.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneVisitor.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreSkeleton.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FixSmallMemAlloc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/trees.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderStateObjManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawElementRes.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderApiImp.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xStdPch.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inftrees.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FastMemAlloc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/MemState.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xLogger.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneSimpleVistor.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/fs/xFileManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/uncompr.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTheme.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaEnc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPIManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdDirEnum.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xKeyframeAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32IME.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xActorDrawElement.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xIDManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdUtil.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xMathTrace.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xPrimitiveElement.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreMesh.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xcamera.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPIDef.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/zlibStream.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneObjectRes.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xMatrixContext.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRenderer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseAction.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreMeshMgr.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xDXTImage.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdir.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xPlatform.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmatfunc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneNode.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xMemBuffer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_operator.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xI18N.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlDocument.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xBaseShader.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xMaterial.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xBaseImage.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32DllLoader.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiHotArea.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec2.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidMaterail.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/TestMem.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzFind.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseObject.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xGdiGlyphFont.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xImage.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/xOperationSys.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inffast.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlDocBuilder.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiControl.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseModelMgr.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xTextureManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xRayIntersection.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdStringHash.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRenderCreator.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiDialog.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xVideoTexture.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiCtrlContainer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/working/xWorkingThread.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRTech.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xProperties.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec4.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32Thread.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xBaseTexture.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_dbg.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderApiObject_.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderLexer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xplane.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderManager.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xTgaImage.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneObject.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneDrawable.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xSmallSizeControl.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderView.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/fs/xFileSystem.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzFindMt.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/crc32.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiStringParser.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFT2Font.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdoc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xActionTrack.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FastSmallMemAlloc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRenderObject.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTextLabel.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseLexer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderNode.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdReadWriterBuffer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindowState.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/deflate.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat3.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xListener.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xHelperRender.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_operator_dbg.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/PixelFormatAssit.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawableHelper.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xWindow.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRenderer.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inflate.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneGraph.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xfrustum.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaDec.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/stdafx.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTextArea.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FixMemAlloc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xModelElement.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/float16/float16.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Linux/xLinuxDllLoader.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPI.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Lzma2Dec.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderName.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Alloc.o \
	${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdStream.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/netbeans

dist/Debug/GNU-Linux-x86/netbeans: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/netbeans ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneCommonObject.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneCommonObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneCommonObject.o ../xEvol3D/xScene/xSceneCommonObject.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xHDRImage.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/xHDRImage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xHDRImage.o ../xEvol3D/Image/xHDRImage.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat4.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xmat4.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat4.o ../xEvol3D/XMathLib/xmat4.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/stl_dllalloc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/stl_dllalloc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/stl_dllalloc.o ../xEvol3D/BaseLib/stl_dllalloc.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Lzma2Enc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/Lzma2Enc.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Lzma2Enc.o ../xEvol3D/lzmza/Lzma2Enc.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xApplication.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Application/xApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xApplication.o ../xEvol3D/Application/xApplication.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindow.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindow.o ../xEvol3D/xui/src/xuiWindow.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32OS.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/OperationSys/Win32/xWin32OS.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32OS.o ../xEvol3D/OperationSys/Win32/xWin32OS.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xPluginMgr.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Application/xPluginMgr.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xPluginMgr.o ../xEvol3D/Application/xPluginMgr.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xPlacement.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xPlacement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xPlacement.o ../xEvol3D/XMathLib/xPlacement.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat4x3.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xmat4x3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat4x3.o ../xEvol3D/XMathLib/xmat4x3.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/adler32.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/adler32.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/adler32.o ../xEvol3D/zlib/adler32.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xquat.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xquat.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xquat.o ../xEvol3D/XMathLib/xquat.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Linux/xLinuxOS.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/OperationSys/Linux/xLinuxOS.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Linux
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Linux/xLinuxOS.o ../xEvol3D/OperationSys/Linux/xLinuxOS.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xrotator.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xrotator.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xrotator.o ../xEvol3D/XMathLib/xrotator.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneHelperDrawable.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneHelperDrawable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneHelperDrawable.o ../xEvol3D/xScene/xSceneHelperDrawable.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xFreeImage.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/xFreeImage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xFreeImage.o ../xEvol3D/Image/xFreeImage.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/MemPool.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/MemPool.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/MemPool.o ../xEvol3D/mem/MemPool.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/MtCoder.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/MtCoder.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/MtCoder.o ../xEvol3D/lzmza/MtCoder.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRendererQueue.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xRendererQueue.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRendererQueue.o ../xEvol3D/Renderer/xRendererQueue.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xBaseTerrain.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xBaseTerrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xBaseTerrain.o ../xEvol3D/xScene/xBaseTerrain.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaLib.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/LzmaLib.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaLib.o ../xEvol3D/lzmza/LzmaLib.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xApplicationLayer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Application/xApplicationLayer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xApplicationLayer.o ../xEvol3D/Application/xApplicationLayer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFontRender.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/font/xFontRender.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFontRender.o ../xEvol3D/font/xFontRender.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xIntersection.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xIntersection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xIntersection.o ../xEvol3D/XMathLib/xIntersection.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/infback.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/infback.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/infback.o ../xEvol3D/zlib/infback.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xstring.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xstring.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xstring.o ../xEvol3D/BaseLib/xstring.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdFile.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcdFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdFile.o ../xEvol3D/xcomdoc/xcdFile.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec3.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xvec3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec3.o ../xEvol3D/XMathLib/xvec3.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xJPEGImage.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/xJPEGImage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xJPEGImage.o ../xEvol3D/Image/xJPEGImage.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Threads.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/Threads.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Threads.o ../xEvol3D/lzmza/Threads.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiButton.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiButton.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiButton.o ../xEvol3D/xui/src/xuiButton.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xTransitionAction.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xTransitionAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xTransitionAction.o ../xEvol3D/xModel/xTransitionAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xMixAction.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xMixAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xMixAction.o ../xEvol3D/xModel/xMixAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdFileImp.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcdFileImp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdFileImp.o ../xEvol3D/xcomdoc/xcdFileImp.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/Packager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/Packager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/Packager.o ../xEvol3D/xcomdoc/Packager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/loki/src/SmallObj.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/loki/src/SmallObj.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/loki/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/loki/src/SmallObj.o ../xEvol3D/loki/src/SmallObj.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/ToyRender/xToyRenderer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/ToyRender/xToyRenderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/ToyRender
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/ToyRender/xToyRenderer.o ../xEvol3D/Renderer/ToyRender/xToyRenderer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Bra.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/Bra.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Bra.o ../xEvol3D/lzmza/Bra.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdocbase.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcomdocbase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdocbase.o ../xEvol3D/xcomdoc/xcomdocbase.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xMultiFtRender.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/font/xMultiFtRender.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xMultiFtRender.o ../xEvol3D/font/xMultiFtRender.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderSemantic.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xShaderSemantic.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderSemantic.o ../xEvol3D/RenderAPI/xShaderSemantic.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/x2DRenderer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/x2DRenderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/x2DRenderer.o ../xEvol3D/RenderAPI/x2DRenderer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFontManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/font/xFontManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFontManager.o ../xEvol3D/font/xFontManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xILImage.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/xILImage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xILImage.o ../xEvol3D/Image/xILImage.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindowManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiWindowManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindowManager.o ../xEvol3D/xui/src/xuiWindowManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/loki/src/Singleton.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/loki/src/Singleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/loki/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/loki/src/Singleton.o ../xEvol3D/loki/src/Singleton.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseModel.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xBaseModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseModel.o ../xEvol3D/xModel/xBaseModel.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/gzio.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/gzio.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/gzio.o ../xEvol3D/zlib/gzio.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/compress.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/compress.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/compress.o ../xEvol3D/zlib/compress.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseMath.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xBaseMath.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseMath.o ../xEvol3D/BaseLib/xBaseMath.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreActionMgr.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xCoreActionMgr.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreActionMgr.o ../xEvol3D/xModel/xCoreActionMgr.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xActorModel.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xActorModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xActorModel.o ../xEvol3D/xScene/xActorModel.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/zutil.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/zutil.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/zutil.o ../xEvol3D/zlib/zutil.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xParamTable.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xParamTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xParamTable.o ../xEvol3D/BaseLib/xParamTable.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneModel.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneModel.o ../xEvol3D/xScene/xSceneModel.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xDistanceFont.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/font/xDistanceFont.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xDistanceFont.o ../xEvol3D/font/xDistanceFont.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xTLEnvioroment.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xTLEnvioroment.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xTLEnvioroment.o ../xEvol3D/RenderAPI/xTLEnvioroment.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawElement.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xDrawElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawElement.o ../xEvol3D/Renderer/xDrawElement.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/mem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem.o ../xEvol3D/mem/mem.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTransBlend.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiTransBlend.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTransBlend.o ../xEvol3D/xui/src/xuiTransBlend.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiCaption.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiCaption.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiCaption.o ../xEvol3D/xui/src/xuiCaption.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xEvol3D.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xEvol3D.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xEvol3D.o ../xEvol3D/xEvol3D.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xAliasFont.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/font/xAliasFont.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xAliasFont.o ../xEvol3D/font/xAliasFont.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xStringTable.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xStringTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xStringTable.o ../xEvol3D/BaseLib/xStringTable.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xXrmModel.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xXrmModel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xXrmModel.o ../xEvol3D/xModel/xXrmModel.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiStateBlend.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiStateBlend.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiStateBlend.o ../xEvol3D/xui/src/xuiStateBlend.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneVisitor.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneVisitor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneVisitor.o ../xEvol3D/xScene/xSceneVisitor.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreSkeleton.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xCoreSkeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreSkeleton.o ../xEvol3D/xModel/xCoreSkeleton.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FixSmallMemAlloc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/FixSmallMemAlloc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FixSmallMemAlloc.o ../xEvol3D/mem/FixSmallMemAlloc.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/trees.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/trees.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/trees.o ../xEvol3D/zlib/trees.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderStateObjManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xRenderStateObjManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderStateObjManager.o ../xEvol3D/RenderAPI/xRenderStateObjManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawElementRes.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xDrawElementRes.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawElementRes.o ../xEvol3D/Renderer/xDrawElementRes.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderApiImp.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xRenderApiImp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderApiImp.o ../xEvol3D/RenderAPI/xRenderApiImp.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xStdPch.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xStdPch.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xStdPch.o ../xEvol3D/xStdPch.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inftrees.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/inftrees.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inftrees.o ../xEvol3D/zlib/inftrees.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FastMemAlloc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/FastMemAlloc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FastMemAlloc.o ../xEvol3D/mem/FastMemAlloc.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/MemState.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/MemState.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/MemState.o ../xEvol3D/mem/MemState.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xLogger.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xLogger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xLogger.o ../xEvol3D/BaseLib/xLogger.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneSimpleVistor.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneSimpleVistor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneSimpleVistor.o ../xEvol3D/xScene/xSceneSimpleVistor.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/fs/xFileManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/fs/xFileManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/fs
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/fs/xFileManager.o ../xEvol3D/fs/xFileManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/uncompr.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/uncompr.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/uncompr.o ../xEvol3D/zlib/uncompr.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTheme.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiTheme.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTheme.o ../xEvol3D/xui/src/xuiTheme.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaEnc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/LzmaEnc.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaEnc.o ../xEvol3D/lzmza/LzmaEnc.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPIManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xRenderAPIManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPIManager.o ../xEvol3D/RenderAPI/xRenderAPIManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdDirEnum.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcdDirEnum.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdDirEnum.o ../xEvol3D/xcomdoc/xcdDirEnum.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xKeyframeAction.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xKeyframeAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xKeyframeAction.o ../xEvol3D/xModel/xKeyframeAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32IME.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/OperationSys/Win32/xWin32IME.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32IME.o ../xEvol3D/OperationSys/Win32/xWin32IME.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xActorDrawElement.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xActorDrawElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xActorDrawElement.o ../xEvol3D/Renderer/xActorDrawElement.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xIDManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xIDManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xIDManager.o ../xEvol3D/BaseLib/xIDManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreAction.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xCoreAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreAction.o ../xEvol3D/xModel/xCoreAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdUtil.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcdUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdUtil.o ../xEvol3D/xcomdoc/xcdUtil.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xMathTrace.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xMathTrace.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xMathTrace.o ../xEvol3D/XMathLib/xMathTrace.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xPrimitiveElement.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xPrimitiveElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xPrimitiveElement.o ../xEvol3D/Renderer/xPrimitiveElement.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xXmlManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlManager.o ../xEvol3D/BaseLib/xXmlManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreMesh.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xCoreMesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreMesh.o ../xEvol3D/xModel/xCoreMesh.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xcamera.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xcamera.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xcamera.o ../xEvol3D/XMathLib/xcamera.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPIDef.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xRenderAPIDef.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPIDef.o ../xEvol3D/RenderAPI/xRenderAPIDef.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/zlibStream.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/zlibStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/zlibStream.o ../xEvol3D/xcomdoc/zlibStream.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneObjectRes.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneObjectRes.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneObjectRes.o ../xEvol3D/xScene/xSceneObjectRes.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xMatrixContext.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xMatrixContext.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xMatrixContext.o ../xEvol3D/RenderAPI/xMatrixContext.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRenderer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xRenderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRenderer.o ../xEvol3D/Renderer/xRenderer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseAction.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xBaseAction.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseAction.o ../xEvol3D/xModel/xBaseAction.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreMeshMgr.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xCoreMeshMgr.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xCoreMeshMgr.o ../xEvol3D/xModel/xCoreMeshMgr.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xDXTImage.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/xDXTImage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xDXTImage.o ../xEvol3D/Image/xDXTImage.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdir.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcomdir.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdir.o ../xEvol3D/xcomdoc/xcomdir.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xPlatform.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Application/xPlatform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xPlatform.o ../xEvol3D/Application/xPlatform.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmatfunc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xmatfunc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmatfunc.o ../xEvol3D/XMathLib/xmatfunc.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneNode.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneNode.o ../xEvol3D/xScene/xSceneNode.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xMemBuffer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xMemBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xMemBuffer.o ../xEvol3D/RenderAPI/xMemBuffer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_operator.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/mem_operator.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_operator.o ../xEvol3D/mem/mem_operator.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xI18N.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xI18N.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xI18N.o ../xEvol3D/BaseLib/xI18N.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlDocument.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xXmlDocument.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlDocument.o ../xEvol3D/BaseLib/xXmlDocument.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xBaseShader.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xBaseShader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xBaseShader.o ../xEvol3D/RenderAPI/xBaseShader.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xMaterial.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xMaterial.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xMaterial.o ../xEvol3D/Renderer/xMaterial.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xBaseImage.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/xBaseImage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xBaseImage.o ../xEvol3D/Image/xBaseImage.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32DllLoader.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/OperationSys/Win32/xWin32DllLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32DllLoader.o ../xEvol3D/OperationSys/Win32/xWin32DllLoader.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiHotArea.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiHotArea.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiHotArea.o ../xEvol3D/xui/src/xuiHotArea.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec2.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xvec2.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec2.o ../xEvol3D/XMathLib/xvec2.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidMaterail.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/KidsRender/xKidMaterail.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidMaterail.o ../xEvol3D/Renderer/KidsRender/xKidMaterail.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/TestMem.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/TestMem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/TestMem.o ../xEvol3D/mem/TestMem.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzFind.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/LzFind.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzFind.o ../xEvol3D/lzmza/LzFind.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseObject.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xBaseObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseObject.o ../xEvol3D/BaseLib/xBaseObject.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xGdiGlyphFont.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/font/xGdiGlyphFont.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xGdiGlyphFont.o ../xEvol3D/font/xGdiGlyphFont.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xImage.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/xImage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xImage.o ../xEvol3D/Image/xImage.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/xOperationSys.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/OperationSys/xOperationSys.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/xOperationSys.o ../xEvol3D/OperationSys/xOperationSys.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inffast.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/inffast.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inffast.o ../xEvol3D/zlib/inffast.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlDocBuilder.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xXmlDocBuilder.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xXmlDocBuilder.o ../xEvol3D/BaseLib/xXmlDocBuilder.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiControl.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiControl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiControl.o ../xEvol3D/xui/src/xuiControl.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseModelMgr.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xBaseModelMgr.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xBaseModelMgr.o ../xEvol3D/xModel/xBaseModelMgr.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xTextureManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xTextureManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xTextureManager.o ../xEvol3D/RenderAPI/xTextureManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xRayIntersection.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xRayIntersection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xRayIntersection.o ../xEvol3D/XMathLib/xRayIntersection.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdStringHash.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcdStringHash.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdStringHash.o ../xEvol3D/xcomdoc/xcdStringHash.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRenderCreator.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/KidsRender/xKidRenderCreator.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRenderCreator.o ../xEvol3D/Renderer/KidsRender/xKidRenderCreator.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiDialog.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiDialog.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiDialog.o ../xEvol3D/xui/src/xuiDialog.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xVideoTexture.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xVideoTexture.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xVideoTexture.o ../xEvol3D/RenderAPI/xVideoTexture.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiCtrlContainer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiCtrlContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiCtrlContainer.o ../xEvol3D/xui/src/xuiCtrlContainer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/working/xWorkingThread.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/working/xWorkingThread.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/working
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/working/xWorkingThread.o ../xEvol3D/working/xWorkingThread.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRTech.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/KidsRender/xKidRTech.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRTech.o ../xEvol3D/Renderer/KidsRender/xKidRTech.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xProperties.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xProperties.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xProperties.o ../xEvol3D/BaseLib/xProperties.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec4.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xvec4.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xvec4.o ../xEvol3D/XMathLib/xvec4.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32Thread.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/OperationSys/Win32/xWin32Thread.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Win32/xWin32Thread.o ../xEvol3D/OperationSys/Win32/xWin32Thread.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xBaseTexture.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xBaseTexture.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xBaseTexture.o ../xEvol3D/RenderAPI/xBaseTexture.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_dbg.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/mem_dbg.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_dbg.o ../xEvol3D/mem/mem_dbg.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderApiObject_.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xRenderApiObject\ .cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderApiObject_.o ../xEvol3D/RenderAPI/xRenderApiObject\ .cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderLexer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xShaderLexer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderLexer.o ../xEvol3D/RenderAPI/xShaderLexer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xplane.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xplane.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xplane.o ../xEvol3D/XMathLib/xplane.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderManager.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xShaderManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderManager.o ../xEvol3D/RenderAPI/xShaderManager.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xTgaImage.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/xTgaImage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/xTgaImage.o ../xEvol3D/Image/xTgaImage.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneObject.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneObject.o ../xEvol3D/xScene/xSceneObject.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneDrawable.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneDrawable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneDrawable.o ../xEvol3D/xScene/xSceneDrawable.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xSmallSizeControl.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xSmallSizeControl.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xSmallSizeControl.o ../xEvol3D/xSmallSizeControl.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderView.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xRenderView.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderView.o ../xEvol3D/RenderAPI/xRenderView.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/fs/xFileSystem.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/fs/xFileSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/fs
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/fs/xFileSystem.o ../xEvol3D/fs/xFileSystem.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzFindMt.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/LzFindMt.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzFindMt.o ../xEvol3D/lzmza/LzFindMt.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/crc32.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/crc32.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/crc32.o ../xEvol3D/zlib/crc32.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiStringParser.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiStringParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiStringParser.o ../xEvol3D/xui/src/xuiStringParser.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFT2Font.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/font/xFT2Font.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/font/xFT2Font.o ../xEvol3D/font/xFT2Font.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdoc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcomdoc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcomdoc.o ../xEvol3D/xcomdoc/xcomdoc.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xActionTrack.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xModel/xActionTrack.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xModel/xActionTrack.o ../xEvol3D/xModel/xActionTrack.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FastSmallMemAlloc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/FastSmallMemAlloc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FastSmallMemAlloc.o ../xEvol3D/mem/FastSmallMemAlloc.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRenderObject.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xRenderObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xRenderObject.o ../xEvol3D/Renderer/xRenderObject.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTextLabel.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiTextLabel.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTextLabel.o ../xEvol3D/xui/src/xuiTextLabel.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseLexer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/BaseLib/xBaseLexer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/BaseLib/xBaseLexer.o ../xEvol3D/BaseLib/xBaseLexer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderNode.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xShaderNode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderNode.o ../xEvol3D/RenderAPI/xShaderNode.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdReadWriterBuffer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcdReadWriterBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdReadWriterBuffer.o ../xEvol3D/xcomdoc/xcdReadWriterBuffer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindowState.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiWindowState.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiWindowState.o ../xEvol3D/xui/src/xuiWindowState.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/deflate.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/deflate.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/deflate.o ../xEvol3D/zlib/deflate.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat3.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xmat3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xmat3.o ../xEvol3D/XMathLib/xmat3.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xListener.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Application/xListener.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xListener.o ../xEvol3D/Application/xListener.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xHelperRender.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xHelperRender.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xHelperRender.o ../xEvol3D/RenderAPI/xHelperRender.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_operator_dbg.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/mem_operator_dbg.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/mem_operator_dbg.o ../xEvol3D/mem/mem_operator_dbg.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/PixelFormatAssit.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Image/PixelFormatAssit.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Image/PixelFormatAssit.o ../xEvol3D/Image/PixelFormatAssit.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawableHelper.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xDrawableHelper.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xDrawableHelper.o ../xEvol3D/Renderer/xDrawableHelper.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xWindow.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Application/xWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Application/xWindow.o ../xEvol3D/Application/xWindow.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRenderer.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/KidsRender/xKidRenderer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/KidsRender/xKidRenderer.o ../xEvol3D/Renderer/KidsRender/xKidRenderer.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inflate.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/zlib/inflate.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/zlib/inflate.o ../xEvol3D/zlib/inflate.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneGraph.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xScene/xSceneGraph.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xScene/xSceneGraph.o ../xEvol3D/xScene/xSceneGraph.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xfrustum.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/XMathLib/xfrustum.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/XMathLib/xfrustum.o ../xEvol3D/XMathLib/xfrustum.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaDec.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/LzmaDec.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/LzmaDec.o ../xEvol3D/lzmza/LzmaDec.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/stdafx.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/stdafx.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/stdafx.o ../xEvol3D/xcomdoc/stdafx.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTextArea.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xui/src/xuiTextArea.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xui/src/xuiTextArea.o ../xEvol3D/xui/src/xuiTextArea.cpp

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FixMemAlloc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/mem/FixMemAlloc.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/mem/FixMemAlloc.o ../xEvol3D/mem/FixMemAlloc.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xModelElement.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/Renderer/xModelElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/Renderer/xModelElement.o ../xEvol3D/Renderer/xModelElement.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/float16/float16.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/float16/float16.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/float16
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/float16/float16.o ../xEvol3D/float16/float16.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Linux/xLinuxDllLoader.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/OperationSys/Linux/xLinuxDllLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Linux
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/OperationSys/Linux/xLinuxDllLoader.o ../xEvol3D/OperationSys/Linux/xLinuxDllLoader.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPI.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xRenderAPI.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xRenderAPI.o ../xEvol3D/RenderAPI/xRenderAPI.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Lzma2Dec.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/Lzma2Dec.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Lzma2Dec.o ../xEvol3D/lzmza/Lzma2Dec.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderName.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/RenderAPI/xShaderName.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/RenderAPI/xShaderName.o ../xEvol3D/RenderAPI/xShaderName.cpp

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Alloc.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/lzmza/Alloc.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/lzmza/Alloc.o ../xEvol3D/lzmza/Alloc.c

${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdStream.o: nbproject/Makefile-${CND_CONF}.mk ../xEvol3D/xcomdoc/xcdStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/xEvol3D/xcomdoc/xcdStream.o ../xEvol3D/xcomdoc/xcdStream.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/netbeans

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
