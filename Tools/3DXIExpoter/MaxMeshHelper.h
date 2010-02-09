#include "BaseLib/xStringHash.h"

void assign(Point3& point , const sVector_t& v)
{
	point.x = v.x;
	point.y = v.y;
	point.z = v.z;
}

void assign(sVector_t& v , const Point3& point)
{
	v.x = point.z;
	v.y = point.y;
	v.z = point.z;
}

struct sTempWeight_t
{
	sVector_t             m_InitPos;
	vector<sVertWeight_t> m_Weights;
	bool _sort()
	{
		std::sort(m_Weights.begin() , m_Weights.end() , std::greater<sVertWeight_t>() );
		return true;
	}
};

wstring getTextureName(std::wstring Name)
{
	std::wstring::size_type pos;
	std::wstring fileName ;
	pos = Name.find_last_of(L"\\");
	if(pos == std::wstring::npos) 
		fileName = Name;

	fileName = Name.substr(pos + 1);
	return fileName;
}

Point3 CalcNormal(const Point3& p1,const Point3& p2,const Point3& p3)
{
	Point3 e1 = p2-p1;
	Point3 e2 = p3-p1;

	Point3 n = CrossProd(e1,e2);
	n.Normalize();
	return n;
}

sColor_t& ColorConv(sColor_t& cl, Color& max_cl)
{
	cl.r = max_cl.r * 255;
	cl.g = max_cl.g * 255;
	cl.b = max_cl.b * 255;
	cl.a = 255;
	return cl;
}
