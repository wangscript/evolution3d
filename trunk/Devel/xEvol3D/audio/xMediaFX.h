/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef __MEDIA_FX_H__
#define __MEDIA_FX_H__
#include "../BaseLib/xEvol3DAPI.h"

#include "FileStreamReader.h"
#include <string>
#include <vector>
#include <iostream>

#define BEGIN_NAMESPACE_MEDIAFX \
    BEGIN_NAMESPACE_XEVOL3D \
    namespace   XMediaFX {  \

#define END_NAMESPACE_MEDIAFX  }}

namespace xMediaFX = NS_XEVOL3D::XMediaFX;

BEGIN_NAMESPACE_MEDIAFX


//Sound Source类型属性：
//   1. 是否流式播放
//   2. 是否为BackGround格式
enum eSoundSrcType
{
    eSoundStream       = 0x00000001,  //支持流式播放
    eSoundMusic        = 0x00000002,  //是否为Background。Music即为背景音乐

    eSndStreamMusic    = 0x00000003,  //使用流式播放的Music。
    eSndNonStreamMusic = 0x00000002,  //使用非流式播放的Music，

    eSndStreamFX       = 0x00000001,  //使用流式播放的音效。
    eSndNonStreamFX    = 0x00000000,  //使用非流式播放的音效。
};

struct sPosition_t
{
    float x , y ,z;
};

struct xSoundListener
{
    sPosition_t pos;
    sPosition_t orient;
    sPosition_t velocity;
};

/*
音源
*/
class xSoundSource
{
public:
    virtual  ~xSoundSource(){};
    enum eState
    {
        ePlaying,
        eNotOpened,
        ePause,
        eFinish,
        eStoped,
    };

    virtual const wchar_t*  name()  = 0;
    virtual eSoundSrcType   type() = 0;
    virtual void            setPos(const sPosition_t& pos) = 0;
    virtual void            getPos(sPosition_t& pos) = 0;

    virtual bool            setVolume(int volume) = 0;
    virtual int             getVolume()           = 0;
    virtual bool            setAutoDelete(bool bAutoDelete) = 0;
    virtual bool            isAutoDelete() = 0;
    virtual bool            seek(double timePos) = 0;
    virtual bool            play(bool playImm = false) = 0;
    virtual bool            stop() = 0;
    virtual bool            pause() = 0;
    virtual bool            resume() = 0;
    virtual eState          state() = 0;
    virtual bool            setLoop(int loopTime , int intValTime) = 0;

    virtual bool            open(IReadStream* pReader) = 0;
    virtual bool            open(const wchar_t* fileName) = 0;
    virtual bool            close() = 0;

};

class  xMusicSequence
{
public:
    xMusicSequence(){};
    virtual ~xMusicSequence(){};
    virtual void   addSource(xSoundSource* pSource) = 0 ;
    virtual void   removeSource(xSoundSource* pSource) = 0 ;
    virtual void   removeSource(const char* name) = 0 ;
    virtual void   setInterValTime(int time) = 0 ;
    virtual void   clear(bool deleteAllSource) = 0 ;
    virtual void   update(long passeTime) = 0 ;

    virtual void   stop() = 0 ;
    virtual void   pause() = 0 ;
    virtual void   resume() = 0 ;
    virtual void   play() = 0 ;
    virtual void   loop(bool bLooped) = 0; 
    virtual bool   isLoop() = 0;
    virtual bool   enable_debug_info(bool bEnable)  = 0 ;
    virtual bool   next() = 0;
    virtual bool   prev() = 0;
    virtual bool   load(istream& in) = 0;
    virtual bool   dump(ostream& out) = 0;
};

/*
声音系统
*/
class  xSoundFXSystem
{
public:
    virtual ~xSoundFXSystem(){}
    virtual void            lock()  = 0 ;
    virtual void            unlock() = 0;
    virtual void            destory()= 0;
    virtual void            manageSoundSource(xSoundSource* pSound) = 0;
    virtual void            unmanageSoundSource(xSoundSource* pSound) = 0;
    virtual void            unmanageSoundSource(const char* name)= 0;
    virtual int             findSoundSource(const char* sourceName, vector<xSoundSource*>& outSource) = 0;
    virtual bool            setBackgroundMusic(xSoundSource* pSound) = 0;
    virtual xSoundSource*   getBackgroundMusic() = 0;
    virtual xSoundSource*   createBackgroundMusic(const char* name,eSoundSrcType type) = 0;


    virtual bool            destorySource(xSoundSource* pSound) = 0;
    virtual xSoundSource*   createSoundSource(const char* name,eSoundSrcType type,bool isManage = true) = 0;

    virtual void            setListener(xSoundListener* pListener) = 0;
    virtual xSoundListener& getListener() = 0;
    virtual void            upadate(long passTime) = 0;

    virtual void            setMusicVolume(int volume) = 0;
    virtual void            setFXVolume(int volue) = 0;

    virtual int             getMusicVolume() = 0;
    virtual int             getFXVolume() = 0;

    virtual void            setVolume(int volume) = 0;
    virtual int             getVolume() = 0;

    virtual int             getNumOfMixerChannel() = 0;

    virtual xMusicSequence* createSequence() = 0;
    virtual void            destorySequence(xMusicSequence* pSeque) = 0;
    virtual bool            dump(ostream& out)  = 0;
    virtual bool            thread_start() = 0 ; 
    virtual bool            thread_pause() = 0;
    virtual bool            thread_resume() = 0;x
};

END_NAMESPACE_MEDIAFX

#endif

