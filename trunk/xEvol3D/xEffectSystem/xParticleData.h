#ifndef __XEVOL_PARTICLE_DATA_H__
#define __XEVOL_PARTICLE_DATA_H__
#include "../BaseLib/stl_dllalloc.h"
#include "../BaseLib/stl_dllalloc.h"
#include "../BaseLib/xXmlDocument.h"
#include "../RenderAPI/xRenderAPI.h"
#include "../Renderer/xRenderer.h"
#include "../XMathLib/xMathLib.h"
#include "../Image/Color.h"
BEGIN_NAMESPACE_XEVOL3D;

namespace nsParticleSystem
{
#define RAND_RAGE    1000
#define RAND_RAGE_F  (float)RAND_RAGE

    enum CHANGE_TYPE
    {
        CHANGE_UNIFORM    =  0,
        CHANGE_INDEPEDENT =  1,
    };

    enum EMITTER_TYPE
    {
        ET_INSTANT = 0,//只喷射一次．
        ET_CONTINUE = 1,//连续喷射．
        ET_INTERVAL = 2,//间隔一定时间喷射一次
    };

    inline void generate_rand(xMathLib::xvec3& out)
    {
        int f1 = ::rand()%RAND_RAGE ;
        int f2 = ::rand()%RAND_RAGE ;
        int f3 = ::rand()%RAND_RAGE ;

        out.x = f1 / RAND_RAGE_F; out.y = f2 / RAND_RAGE_F; out.z = f3/ RAND_RAGE_F;
    }

    inline void generate_rand(xMathLib::xvec4& out)
    {
        float f1 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        float f2 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        float f3 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        float f4 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        out.x = f1;out.y = f2;out.z = f3;out.w = f4;
    }

    inline void generate_rand(xColor_4f& out)
    {
        float f1 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        float f2 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        float f3 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        float f4 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        out.r = f1;out.g = f2;out.b = f3;out.a = f4;
    }

    inline void generate_rand(xMathLib::xvec2& out)
    {
        float f1 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        float f2 = ::rand()%RAND_RAGE / RAND_RAGE_F;
        out.x = f1;out.y = f2;
    }

    inline void generate_rand(float& t)
    {
        t = ::rand()%RAND_RAGE / RAND_RAGE_F;
    }


    inline void generate_rand(int & t)
    {
        t = ::rand()%RAND_RAGE;
    }

    //数据区间
    template <typename T> class xDataRange
    {
    public: 
        T m_Min;
        T m_Max;

        xDataRange()
        {

        }

        xDataRange(const T& min,const T& max):m_Max(max),m_Min(min)
        {

        }
  
        void setMin(const T& min)
        { 
            m_Min = min;
        }

        void setMax(const  T& max)
        {
            m_Max = max; 
        }

        void setRange(const T& min,const T& max)
        {
            m_Min = min;
            m_Max = max;
        }

        bool isInRange(const T& v)
        {
            if( v < m_Max && v > m_Min)
                return true; 
            return false; 
        }

        T   rand_unifrom()
        {
            float f = ::rand()%RAND_RAGE/RAND_RAGE_F;
            return m_Min + (m_Max-m_Min) * f;
        }

        T   rand()
        {
            T f;
            generate_rand(f);
            return m_Min + (m_Max-m_Min) * f;
        }
    };

    typedef xDataRange<xMathLib::xvec3>     Vec3Range;
    typedef xDataRange<float>               ScalarRange;
    typedef xDataRange<xMathLib::xvec4>     Vec4Range;
    typedef xDataRange<xColor_4f>           ColorRange;
    typedef xDataRange<xMathLib::xvec2>     Vec2Range;
    typedef xDataRange<int>                 IntRange;

    //////////////////////////////////////////////////////////////////////////
    inline void strToVec3Range(const wchar_t* str , Vec3Range& range)
    {
        swscanf(str, L"[%f,%f,%f]-[%f,%f,%f]", &range.m_Min.x, &range.m_Min.y, &range.m_Min.z,
                                               &range.m_Max.x, &range.m_Max.y, &range.m_Max.z);
    }

    inline void strToColorRange(const wchar_t* str , ColorRange& range)
    {
        swscanf(str, L"[%f,%f,%f,%f]-[%f,%f,%f,%f]", &range.m_Min.r,&range.m_Min.g,&range.m_Min.b,&range.m_Min.a,
                                                     &range.m_Max.r,&range.m_Max.g,&range.m_Max.b,&range.m_Max.a);
    }

    inline void strToVec4Range(const wchar_t* str , Vec4Range& range)
    {
        swscanf(str, L"[%f,%f,%f,%f]-[%f,%f,%f,%f]", &range.m_Min.x,&range.m_Min.y,&range.m_Min.z,&range.m_Min.w,
                                                     &range.m_Max.x,&range.m_Max.y,&range.m_Max.z,&range.m_Max.w);
    }


    inline void strToScalarRange(const wchar_t* str , ScalarRange& range)
    {
        swscanf(str, L"%f-%f",&range.m_Min,&range.m_Max);
    }

    inline void strToIntRange(const wchar_t* str , IntRange& range)
    {
        swscanf(str, L"%d-%d",&range.m_Min,&range.m_Max);
    }

    inline void strToVec2Range(const wchar_t* str , Vec2Range& range)
    {
        swscanf(str, L"[%f,%f]-[%f,%f]",&range.m_Min.x,&range.m_Min.y,
                                        &range.m_Max.x,&range.m_Max.y);
    }

    inline void strToColor(const wchar_t* str , xColor_4f& v)
    {
        swscanf(str, L"[%f,%f,%f,%f]",&v.r,&v.g,&v.b,&v.a);
    }


    inline void strToVec4(const wchar_t* str , xMathLib::xvec4& v)
    {
        swscanf(str, L"[%f,%f,%f,%f]",&v.x,&v.y,&v.z,&v.w);
    }

    inline void strToVec3(const wchar_t* str , xMathLib::xvec3& v)
    {
        swscanf(str, L"[%f,%f,%f]",&v.x,&v.y,&v.z);
    }

    inline void strToVec2(const wchar_t* str , xMathLib::xvec2& v)
    {
        swscanf(str, L"[%f,%f]",&v.x,&v.y);
    }



    //////////////////////////////////////////////////////////////////////////
    inline void setVec3Range(xXmlNode* pXml , const wchar_t* _name , const Vec3Range& range)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"[%g,%g,%g]-[%g,%g,%g]", range.m_Min.x, range.m_Min.y, range.m_Min.z,
                                                  range.m_Max.x, range.m_Max.y, range.m_Max.z);
        pXml->setValue(_name , str);
    }

    inline void setVec4Range(xXmlNode* pXml , const wchar_t* _name , const  Vec4Range& range)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"[%g,%g,%g,%g]-[%g,%g,%g,%g]", range.m_Min.x, range.m_Min.y, range.m_Min.z, range.m_Min.w,
                                                        range.m_Max.x, range.m_Max.y, range.m_Max.z, range.m_Max.w);
        pXml->setValue(_name , str);
    }

    inline void setColorRange(xXmlNode* pXml , const wchar_t* _name , const  ColorRange& range)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"[%g,%g,%g,%g]-[%g,%g,%g,%g]", range.m_Min.r, range.m_Min.g, range.m_Min.b, range.m_Min.a,
                                                        range.m_Max.r, range.m_Max.g, range.m_Max.b, range.m_Max.a);
        pXml->setValue(_name , str);
    }

    inline void setScalarRange(xXmlNode* pXml , const wchar_t* _name , const  ScalarRange& range)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"%g-%g",range.m_Min,range.m_Max);
        pXml->setValue(_name , str);
    }

    inline void setIntRange(xXmlNode* pXml , const wchar_t* _name , const  IntRange& range)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"%d-%d", range.m_Min, range.m_Max);
        pXml->setValue(_name , str);
    }

    inline void setVec2Range(xXmlNode* pXml , const wchar_t* _name , const  Vec2Range& range)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"[%g,%g]-[%g,%g]",range.m_Min.x,range.m_Min.y,
                                           range.m_Max.x,range.m_Max.y);
        pXml->setValue(_name , str);
    }


    inline void setVec4(xXmlNode* pXml , const wchar_t* _name , const  xMathLib::xvec4& v)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"[%g,%g,%g,%g]", v.x, v.y, v.z, v.w);
        pXml->setValue(_name , str);
    }

    inline void setColor(xXmlNode* pXml , const wchar_t* _name , const  xColor_4f& v)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"[%g,%g,%g,%g]", v.r, v.g, v.b, v.a);
        pXml->setValue(_name , str);
    }

    inline void setVec3(xXmlNode* pXml , const wchar_t* _name , const  xMathLib::xvec3& v)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"[%g,%g,%g]", v.x, v.y, v.z);
        pXml->setValue(_name , str);
    }


    inline void setVec2(xXmlNode* pXml , const wchar_t* _name , const  xMathLib::xvec2& v)
    {
        wchar_t str[128] = {0};
        swprintf_x(str, 128 , L"[%g,%g]", v.x, v.y);
        pXml->setValue(_name , str);
    }

    //////////////////////////////////////////////////////////////////////////
    inline bool getVec3Range(xXmlNode* pXml , const wchar_t* _name , Vec3Range& range)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToVec3Range(strValue , range);
        return true;
    }

    inline bool getVec4Range(xXmlNode* pXml , const wchar_t* _name , Vec4Range& range)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToVec4Range(strValue , range);
        return true;
    }

    inline bool getColorRange(xXmlNode* pXml , const wchar_t* _name , ColorRange& range)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToColorRange(strValue , range);
        return true;
    }

    inline bool getScalarRange(xXmlNode* pXml , const wchar_t* _name , ScalarRange& range)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToScalarRange(strValue , range);
        return true;
    }

    inline bool getIntRange(xXmlNode* pXml , const wchar_t* _name , IntRange& range)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToIntRange(strValue , range);
        return true;
    }

    inline bool getVec2Range(xXmlNode* pXml , const wchar_t* _name , Vec2Range& range)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToVec2Range(strValue , range);
        return true;
    }



    inline bool getVec4(xXmlNode* pXml , const wchar_t* _name , xMathLib::xvec4& v)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToVec4(strValue , v);
        return true;
    }

    inline bool getColor(xXmlNode* pXml , const wchar_t* _name , xColor_4f& v)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToColor(strValue , v);
        return true;
    }

    inline bool getVec3(xXmlNode* pXml , const wchar_t* _name , xMathLib::xvec3& v)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToVec3(strValue , v);
        return true;
    }

    inline bool getVec2(xXmlNode* pXml , const wchar_t* _name , xMathLib::xvec2& v)
    {
        const wchar_t* strValue = pXml->value(_name);
        if(strValue == NULL)
            return false;

        strToVec2(strValue , v);
        return true;
    }
}


END_NAMESPACE_XEVOL3D
#endif
