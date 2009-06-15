int4 color_id;
int2 main() : SV_Target
{  
      return  int2(color_id.x, color_id.y);
}
