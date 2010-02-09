#ifndef _TERRAIN_DEF_H_
#define _TERRAIN_DEF_H_



// 思路:
// 128 x 128 : 16KB[count] Vertices / patch
// 10000 x 10000 : 大约78 x 78 patch，约有6084次提交过程
// 多级Patch可以减少提交过程，比如将10000x10000继续细分为100个1000x1000，最近的一个采用细Patch提交，远处的采用简化顶点提交。
#define PATCH_SIDETILECOUNT 128
#define PATCH_TILECOUNT (PATCH_SIDETILECOUNT*PATCH_SIDETILECOUNT)
#define PATCH_SIDEVERTCOUNT (PATCH_SIDETILECOUNT+1)
#define PATCH_VERTCOUNT (PATCH_SIDEVERTCOUNT*PATCH_SIDEVERTCOUNT)




#endif