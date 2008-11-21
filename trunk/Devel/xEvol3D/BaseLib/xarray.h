#ifndef __XARRAY_H__
#define __XARRAY_H__

#include <iostream>
#include <assert.h>
namespace std
{
	template <typename T,int Dim> class xarray
	{
	public:
		xarray()
		{
			m_pArrays = NULL;
			m_DimSize = 0;
			m_pData   = NULL;
		}

		xarray(int* DimSize,T* pMem = NULL)
		{
			m_pData   = NULL;
			create(DimSize,pMem);
		}

		/*
		创建
		*/
		void create(int* DimSize,T* pMem = NULL)
		{
			int size = 1;
			m_pData   = NULL;
			//内存只有被创建一次
			if(pMem == NULL)
			{ 
				for(int i = 0 ; i < Dim; i++)
				{
					size *= DimSize[i];
				}
				pMem = new T[size];
				m_pData = pMem;
			}

			m_pArrays = new xarray<T,Dim-1>[*DimSize];
			m_DimSize  = *DimSize ;

			int d_Mem = size / m_DimSize ;
			for(int i  = 0; i< m_DimSize ; i++)
			{
				m_pArrays[i].create((DimSize + 1),pMem + d_Mem * i);
			}

		}

		T* operator &()
		{
			return &(m_pArrays[0]);
		}

		xarray<T,Dim-1>& operator[](int index)
		{
			assert(index < m_DimSize && index >= 0);
			return m_pArrays[index];
		}

		void free()
		{
			delete [] m_pArrays;
			m_pArrays = NULL;
			if(m_pData  !=  NULL)
			{
				delete m_pData;
				m_pData   = NULL;
				cout<<"T 类型内存被释放,该内存统一分配 "<<Dim<<endl;
			}
		}

		~xarray()
		{
			free();
		}


	protected:
		xarray<T,Dim-1>* m_pArrays;
		int              m_DimSize ;
		T*               m_pData;


	};
	template <typename T> class xarray<T,1>
	{
	public:
		xarray()
		{
			m_Data = NULL;
			m_DimSize = 0;
			m_bNeedDelete = false;
		}

		xarray(int* DimSize,T* pMem)
		{
			m_bNeedDelete = false;
			create(DimSize,pMem);
		}
		void create(int size,T* pMem)
		{
			if(pMem == NULL)
			{
				pMem = new T[size];
				m_bNeedDelete = true;
			}
			m_Data =pMem;
			m_DimSize = size;
		}

		T* operator &()
		{
			return m_Data; 
		}
		void create(int* DimSize,T* pMem)
		{
			if(pMem == NULL)
			{
				pMem = new T[DimSize[0]];
				m_bNeedDelete = true;
			}
			m_Data = pMem;
			m_DimSize = DimSize[0];
		}
		T& operator[](int index)
		{
			assert(index < m_DimSize && index >= 0);
			return m_Data[index];
		}
		void free()
		{
			if( m_bNeedDelete)
			{
				delete [] m_Data;
				m_Data = NULL;
				cout<<"T 类型内存被释放"<<endl;
			}
		}

		~xarray()
		{
			free();
		}

	public:
		T*               m_Data;
		int              m_DimSize ;
		bool             m_bNeedDelete;
	};
}

#endif

