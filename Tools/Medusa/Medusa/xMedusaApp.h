#include "xMedusaEditor.h"
class IMedusaApplication
{
public:
    virtual bool Start() = 0;
	virtual bool Exit() = 0;
	virtual void SetAppName(const wchar_t* _appName) = 0;
	virtual void SetUILayout(const wchar_t* layoutFile , const wchar_t* layoutName , bool bRelPath = true) = 0;
	virtual void SetInitScene(const wchar_t* sceneFile ) = 0;
	virtual void SetWorkDir(const wchar_t* _workDir) = 0;
	virtual void DeleteObject() = 0;
	virtual void updateFrame() = 0;
	virtual bool messageLoop() = 0;
};

extern "C" MEDUSA_API IMedusaApplication * GetMedusaApp();