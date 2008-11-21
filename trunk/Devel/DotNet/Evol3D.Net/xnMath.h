// Evol3D.Net.h

#pragma once
#include "XMathLib/xMathlib.h"
#include "XMathLib/XGeomLib.h"
#include "xDotNetUlti.h"
using namespace XEvol3D;
using namespace xMathLib;
using namespace xGeomLib;
using namespace System;

namespace xEvol3D 
{

	public ref class xVector2
	{
	internal:
		xvec2*  m_vec;
		bool    m_bNeedDel;
		float*  data() { return m_vec->v ; }
		xvec2&  xvec() { return *m_vec ; }
		xVector2(xvec2* vec){  m_vec = vec      ; m_bNeedDel = false; }
	public:
		xVector2(float x, float y) {  m_vec = new xvec2(x , y); m_bNeedDel = true; }
		xVector2() {  m_vec = new xvec2; m_bNeedDel = true; }
		~xVector2(){ if( m_bNeedDel ) delete m_vec ; }
		float fabs()                    {return m_vec->fabs() ; }
		float dp (const xVector2^ v1)   {return m_vec->dp(*v1->m_vec) ; }
		void  add(const xVector2^ v1)   {return m_vec->add(*v1->m_vec) ; }
		void  sub(const xVector2^ v1)   {return m_vec->sub(*v1->m_vec) ; }
		float len()                     {return m_vec->len() ;}
		float squardlen()               {return m_vec->squardlen() ;}
		void  normalize()               {return m_vec->normalize() ;}
		void  normalize(xVector2^ out)   {return m_vec->normalize(*out->m_vec); }
		xVector2^ operator + (const xVector2^ v1)
		{
			xvec2 ret = *v1->m_vec + xvec();
			return gcnew xVector2(ret.x , ret.y);
		}

		xVector2^ operator - (const xVector2^ v1)
		{
			xvec2 ret = *v1->m_vec + xvec();
			return gcnew xVector2(ret.x , ret.y);
		}
		PS_ReadWrite(Single, x,m_vec->x,,);
		PS_ReadWrite(Single, y,m_vec->y,,);

		PS_ReadWrite(Single, u,m_vec->x,,);
		PS_ReadWrite(Single, v,m_vec->y,,);
	};

	public ref class xVector3
	{
	internal:
		xvec3*  m_vec;
		bool    m_bNeedDel;
		float*  data() { return m_vec->v ; }
		xvec3&  xvec() { return *m_vec ; }
		xVector3(xvec3* vec){  m_vec = vec      ; m_bNeedDel = false; }
	public:
		xVector3(float x ,float y ,float z)          {  m_vec = new xvec3(x , y , z); m_bNeedDel = true; }
		xVector3()          {  m_vec = new xvec3; m_bNeedDel = true; }


		~xVector3(){ if( m_bNeedDel ) delete m_vec ; }
		float fabs()                    {return m_vec->fabs() ; }
		float dp (const xVector3^ v1)   {return m_vec->dp(*v1->m_vec) ; }
		void  add(const xVector3^ v1)   {return m_vec->add(*v1->m_vec) ; }
		void  sub(const xVector3^ v1)   {return m_vec->sub(*v1->m_vec) ; }
		float len()                     {return m_vec->len() ;}
		float squardlen()               {return m_vec->squardlen() ;}
		void  normalize()               {return m_vec->normalize() ;}
		void  normalize(xVector3^ out)  {return m_vec->normalize(*out->m_vec); }
		xVector3^ operator + (const xVector3^ v1)
		{
			xvec3 ret = *v1->m_vec + xvec();
			return gcnew xVector3(ret.x , ret.y , ret.z);
		}

		xVector3^ operator - (const xVector3^ v1)
		{
			xvec3 ret = *v1->m_vec + xvec();
			return gcnew xVector3(ret.x , ret.y , ret.z);
		}

		xVector3^ cp(const xVector3^ v1)  ;
		PS_ReadWrite(Single, x,m_vec->x,,);
		PS_ReadWrite(Single, y,m_vec->y,,);
		PS_ReadWrite(Single, z,m_vec->z,,);
	};

	public ref class xVector
	{
	internal:
		xvec4*  m_vec;
		bool    m_bNeedDel;
		float*  data() { return m_vec->v ; }
		xvec4&  xvec() { return *m_vec ; }
	public:
		xVector(float x ,float y ,float z ,float w)          {  m_vec = new xvec4(x , y , z ,w); m_bNeedDel = true; }
		xVector()          {  m_vec = new xvec4; m_bNeedDel = true; }
		xVector(xvec4* vec){  m_vec = vec      ; m_bNeedDel = false; }

		~xVector(){ if( m_bNeedDel ) delete m_vec ; }
		float fabs()                    {return m_vec->fabs() ; }
		float dp3(const xVector^ v1)    {return m_vec->dp3(*v1->m_vec) ; }
		float dp4(const xVector^ v1)    {return m_vec->dp4(*v1->m_vec) ; }
		void  add(const xVector^ v1)    {return m_vec->add(*v1->m_vec) ; }
		void  sub(const xVector^ v1)    {return m_vec->sub(*v1->m_vec) ; }
		void  rhw()                     {return m_vec->rhw() ;}
		float len()                     {return m_vec->len() ;}
		float squardlen()               {return m_vec->squardlen() ;}
		void  normalize()               {return m_vec->normalize() ;}
		void  normalize(xVector^ out)   {return m_vec->normalize(*out->m_vec); }

		xVector^ operator + (const xVector^ v1)
		{
			xvec4 ret = *v1->m_vec + xvec();
			return gcnew xVector(ret.x , ret.y , ret.z, ret.w);
		}

		xVector^ operator - (const xVector^ v1)
		{
			xvec4 ret = *v1->m_vec + xvec();
			return gcnew xVector(ret.x , ret.y , ret.z , ret.w);
		}

		xVector^ cp(const xVector^ v1)  ;
		xVector^ getNormal() ;
		PS_ReadWrite(Single, x,m_vec->x,,);
		PS_ReadWrite(Single, y,m_vec->y,,);
		PS_ReadWrite(Single, z,m_vec->z,,);
		PS_ReadWrite(Single, w,m_vec->w,,);

		PS_ReadWrite(Single, u,m_vec->x,,);
		PS_ReadWrite(Single, v,m_vec->y,,);
		PS_ReadWrite(Single, s,m_vec->z,,);
		PS_ReadWrite(Single, t,m_vec->w,,);

		PS_ReadWrite(Single, left  ,m_vec->x,,);
		PS_ReadWrite(Single, top   ,m_vec->y,,);
		PS_ReadWrite(Single, width ,m_vec->z,,);
		PS_ReadWrite(Single, height,m_vec->w,,);
	};

	public ref class xMatrix
	{
	internal:
		xmat4*  m_mat;
		bool    m_bNeedDel;
		xmat4&  mat(){return *m_mat ; }
        xMatrix(xmat4* mat){m_mat = mat ; m_bNeedDel = false; };
	public:
		xMatrix() {m_mat = new xmat4 ; m_bNeedDel = true ; }
		~xMatrix(){if(m_bNeedDel) delete m_mat ; };
		void   identity() { m_mat->identity() ; }
		void   zero() { m_mat->zero() ; }
		void   transform(float tx,float ty,float tz)    { m_mat->transform(tx,ty,tz); }
		void   setTransform(float tx,float ty,float tz) { m_mat->setTransform(tx , ty , tz); }
		void   scale(float sx,float sy,float sz)        { m_mat->scale(sx , sy , sz) ; }
		void   noTrans() { m_mat->noTrans() ; }
		bool   inverse() { return  m_mat->inverse() ; }
		float* data() {return m_mat->data ; }
		float  get(int x , int y)            {return m_mat->m[y][x] ; }
		void   set(int x , int y , float v)  { m_mat->m[y][x]  = v; }
	};

	public ref class  xQuat
	{
	internal:
		xquat*  m_quat;
		bool    m_bNeedDel;
		float*  data() { return  &m_quat->x ; }
		xquat&  quat() { return *m_quat ; }
		xQuat(xquat* vec){  m_quat = vec      ; m_bNeedDel = false; }
	public:
		xQuat(float x ,float y ,float z ,float w) {  m_quat = new xquat(x , y , z ,w); m_bNeedDel = true; }
		xQuat()          {  m_quat = new xquat; m_bNeedDel = true; }
		xQuat(xVector^ axis,float angle){  m_quat = new xquat(axis->xvec() , angle); m_bNeedDel = true; }
	public:
		float  magnitude(){ return quat().magnitude() ; }
		void   conjugate(){ return quat().conjugate() ; }
		void   conjugate(xQuat^ out){ return quat().conjugate( out->quat() ) ; }
		void   set(float x , float y , float z,float angle){ return quat().set( x , y , z , angle ) ; }
		float  toAxis(xVector3^ axis){float angle = 0.0f ; quat().toAxis(axis->xvec() , angle) ; return angle ;}
		void   set(xVector3^ axis,float angle){ return quat().set(axis->xvec() , angle ) ; }
		void   set(xVector^ axis,float angle) { return quat().set(axis->xvec() , angle ) ; }
		void   normalize(){quat().normalize( ) ; }
		void   normalize(xQuat^  qOut) {quat().normalize( qOut->quat() ) ; }
		void   inverse() { quat().inverse( ) ; }
		void   inverse(xQuat^  qOut)  {quat().inverse( qOut->quat() ) ; }
		void   toMatrix(xMatrix^ mOut) {quat().toMatrix( mOut->mat() ) ; }
		xQuat^ operator * (xQuat^ q2)      { xquat ret = quat().operator * (q2->quat())  ;   return gcnew xQuat(  ret.x , ret.y , ret.z , ret.w) ;}
		xQuat^ operator *= (xQuat^ q2)     { xquat ret = quat().operator *=(q2->quat())  ;   return gcnew xQuat(  ret.x , ret.y , ret.z , ret.w) ;}
		xQuat^    slerp(float t,xQuat^ q2) { xquat ret = quat().slerp(t, q2->quat())     ;   return gcnew xQuat(  ret.x , ret.y , ret.z , ret.w) ;}
		xVector^  rotate(xVector^ vIn)     { xvec4 ret = quat().rotate(vIn->xvec())      ;   return gcnew xVector(ret.x , ret.y , ret.z , ret.w) ;}
		PS_ReadWrite(Single, x,m_quat->x,,);
		PS_ReadWrite(Single, y,m_quat->y,,);
		PS_ReadWrite(Single, z,m_quat->z,,);
		PS_ReadWrite(Single, w,m_quat->w,,);
	};

	public ref class xAABB
	{
	internal:
		xaabb*  m_aabb;
		bool    m_bNeedDel;
		xaabb&  aabb() { return *m_aabb ; }
		xAABB(xaabb* vec){  m_aabb = vec      ; m_bNeedDel = false; }
	public:
		xAABB(xVector3^ _min , xVector3^ _max) {  m_aabb = new xaabb(_min->xvec() , _max->xvec() ); m_bNeedDel = true; }
		xAABB() {  m_aabb = new xaabb( ); m_bNeedDel = true; }
		~xAABB(){ if( m_bNeedDel ) delete m_aabb ; }
		void Add(xVector3^ pt) {m_aabb->AddPoint(pt->xvec() ) ; }
	};

	public ref class xSphere
	{
	internal:
		xshpere*  m_shpere;
		bool      m_bNeedDel;
		xshpere&  shpere() { return *m_shpere ; }
		xSphere(xshpere* vec){  m_shpere = vec      ; m_bNeedDel = false; }
	public:
		xSphere(xVector3^ _ct , float _r) {  m_shpere = new xshpere ; m_shpere->m_Orign = _ct->xvec()  ; m_shpere->m_R = _r;  m_bNeedDel = true; }
		xSphere() {  m_shpere = new xshpere ; m_bNeedDel = true; }
		~xSphere(){ if( m_bNeedDel ) delete m_shpere ; }
	};

	public ref class xMathlib
	{
	public:
		//xmat4func.h
		static void Sub(xMatrix^ m1,xMatrix^ m2,xMatrix^ mOut) { XEvol3D::XMathLib::XM_Sub(m1->mat() , m2->mat() , mOut->mat() );}
		static void Add(xMatrix^ m1,xMatrix^ m2,xMatrix^ mOut) { XEvol3D::XMathLib::XM_Add(m1->mat() , m2->mat() , mOut->mat() );}
		static void Mul(xMatrix^ m1,xMatrix^ m2,xMatrix^ mOut) { XEvol3D::XMathLib::XM_Mul(m1->mat() , m2->mat() , mOut->mat() );}
		static void Mul(xMatrix^ m1,float s    ,xMatrix^ mOut) { XEvol3D::XMathLib::XM_Mul(m1->mat() ,  s        , mOut->mat() );}
		static void Mul(float s    ,xMatrix^ m1,xMatrix^ mOut) { XEvol3D::XMathLib::XM_Mul(m1->mat() ,  s        , mOut->mat() );}
        static void Lerp(xMatrix^ m1, xMatrix^ m2 , float t , xMatrix^ mOut)	{ XEvol3D::XMathLib::XM_Lerp(m1->mat() , m2->mat() , t , mOut->mat() ) ;	}
        static void Mul(xMatrix^   m,xVector^ v   ,xVector3^  vOut){ XEvol3D::XMathLib::XM_Mul(m->mat() ,  v->xvec()        , vOut->xvec() );}
		static void Mul(xVector^ v,xMatrix^ m,xVector3^  vOut) { XEvol3D::XMathLib::XM_Mul(  v->xvec() , m->mat() , vOut->xvec() );}
		static void Mul(xVector3^ v,xMatrix^ m,xVector3^  vOut){ XEvol3D::XMathLib::XM_Mul(  v->xvec() , m->mat() , vOut->xvec() );}
		static void Mul(xMatrix^ m,xVector3^ v,xVector3^  vOut){ XEvol3D::XMathLib::XM_Mul(  m->mat()  , v->xvec(), vOut->xvec() );}
		static void Mul(xMatrix^ m,xVector^ v,xVector^ vOut){ XEvol3D::XMathLib::XM_Mul(  m->mat()  , v->xvec(), vOut->xvec() );}
		static void Mul(xVector^ v,xMatrix^ m,xVector^ vOut){ XEvol3D::XMathLib::XM_Mul(  v->xvec() , m->mat() , vOut->xvec() );}
		
		//xmatDecl.h
		static void Mul(xQuat^ qA,xQuat^ qB,xQuat^  qOut)          { XM_Mul    (qA->quat() , qB->quat() , qOut->quat())  ; }
		static void Rotate(xQuat^ q,xVector^ v,xVector^ vOut)      { XM_Rotate (q->quat()  , v->xvec()  , vOut->xvec())  ; }
		static void Add(xQuat^ qA,xQuat^ qB,xQuat^  qOut)          { XM_Add    (qA->quat() , qB->quat() , qOut->quat())  ; }
		static void Inverse(xQuat^ qA ,xQuat^  qOut)               { XM_Inverse(qA->quat() , qOut->quat())  ; }
		static void Slerp(float t,xQuat^ q1,xQuat^ q2,xQuat^  qOut){ XM_Slerp  (t          , q1->quat() , q2->quat() , qOut->quat())  ; }

		//bool	XM_Inv(const xmat4x3& m1 , xmat4x3& mOut);
		//bool	XM_Inv(const xmat3& m1 , xmat3& mOut);
		static bool Inverse(xMatrix^ m1 , xMatrix^ mOut)     { return XM_Inv(m1->mat() , mOut->mat() )  ; }
		static void Transpos(xMatrix^ m , xMatrix^ mOut) { XM_Transpos(m->mat() , mOut->mat() )  ; }
		static void Perspective(xMatrix^ mOut,float fov_angle,float aspect,float Znear,float Zfar)                  { XM_Perspective(mOut->mat() , fov_angle,aspect,Znear,Zfar)  ; }
		static void Ortho3D(xMatrix^ mOut,float Znear ,float Zfar ,float top,float bottom ,float left ,float right) { XM_Ortho3D(mOut->mat() , Znear , Zfar , top, bottom , left , right)  ; }

		static void Transform(xMatrix^ mOut,float tx,float ty,float tz) { XM_Transform(mOut->mat() , tx,ty,tz)  ; }
		//void	XM_Transform(xmat3& mOut,float tx,float ty);

		static void Scale(xMatrix^ mOut,float sx,float sy,float sz) { XM_Scale(mOut->mat() , sx,sy,sz)  ; }
		static void XForm_Trans(xMatrix^ mOut, xMatrix^ mRot, xVector3^ vCenter){ XM_XForm_Trans(mOut->mat() , mRot->mat() , vCenter->xvec() )  ; }
		static void RotateX(xMatrix^ mOut,float angle) { XM_RotateX(mOut->mat() , angle)  ; }
		static void RotateY(xMatrix^ mOut,float angle) { XM_RotateY(mOut->mat() , angle)  ; }
		static void RotateZ(xMatrix^ mOut,float angle) { XM_RotateZ(mOut->mat() , angle)  ; }
		static void RotatePV(xMatrix^ mOut,xVector^ v , float angle) { XM_RotatePV(mOut->mat() , v->xvec() , angle)  ; }
		static void Rotate(xMatrix^ mOut,xVector^ v , float angle)   { XM_Rotate  (mOut->mat() , v->xvec() , angle)  ; }
		static void LookAt(xMatrix^ mOut,xVector^ eyeAt,xVector^ eyeTarget,xVector^ upDir) { XM_LookAt  (mOut->mat() , eyeAt->xvec(),eyeTarget->xvec(),upDir->xvec())  ; }
	};	 
}