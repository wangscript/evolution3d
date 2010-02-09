#include "mem.h"
#include "MemState.h"
#include <fstream>
using namespace std;


using namespace DGE;



#define new DNEW
//FastSmallMemAlloc g_FAlloc;

void test_FastMemAlloc()
{
	int i;
	FastMemAlloc g_FAlloc;
    char* p1 = (char*)g_FAlloc.alloc(1000);
	char* p2 = (char*)g_FAlloc.alloc(10004);
	char* p  = (char*)g_FAlloc.alloc(600);

	for(i = 0 ; i < 1003 ; i ++)
	{
		*(p1 + i ) = 9;
	}

	for(i = 0 ; i < 100 ; i ++)
	{
		//*(p + i ) = 9;
	}

	g_FAlloc.dealloc(p);
#ifdef _DEBUG
	g_FAlloc.diagnostic(); 
	g_FAlloc.diagBound();
#endif


}

void test_FastSmallMemAlloc()
{
	int i;
	FastSmallMemAlloc Alloc;
    Alloc.init();
	char* p1 = (char*)Alloc.alloc(10,__FILE__,__LINE__);
	char* p2 = (char*)Alloc.alloc(100,__FILE__,__LINE__);
	char* p  = (char*)Alloc.alloc(60,__FILE__,__LINE__);

	for(i = 0 ; i < 103 ; i ++)
	{
		*(p1 + i ) = 9;
	}

	for(i = 0 ; i < 60 ; i ++)
	{
		//*(p + i ) = 9;
	}

	Alloc.dealloc(p1);
	Alloc.dealloc(p2);
#ifdef _DEBUG

#endif


}
#include <stdio.h>
int main()
{
     char *pCha = new char;
     char *pCha2 = new char[2];
     char* pCharA = new char[16];
     char* pCharAG = new char[1024];

     //memcpy((void*)pCha,(void*)pCha2,32);
     diagnostic(cout);
     diagBound(cout);

     system("pause");
     return 1;
}
