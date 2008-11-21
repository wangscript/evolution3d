int4 color_id;
int4 main() : SV_Target
{   
   return  int4(color_id.x, color_id.y , color_id.z, color_id.w);
}