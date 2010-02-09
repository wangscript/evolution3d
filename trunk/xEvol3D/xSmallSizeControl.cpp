#ifndef _DEBUG
#pragma comment(linker, "/FILEALIGN:16")
//#pragma comment(linker, "/ALIGN:16")
#pragma comment(linker, "/OPT:REF")
#pragma comment(linker, "/OPT:ICF")
#pragma comment(linker, "/OPT:NOWIN98")
// Merge sections
//#pragma comment(linker, "/MERGE:.rdata=.data")
//#pragma comment(linker, "/MERGE:.text=.data")
//#pragma comment(linker, "/MERGE:.reloc=.data")
// Favour small code
#pragma optimize("gsy", on)
#endif
