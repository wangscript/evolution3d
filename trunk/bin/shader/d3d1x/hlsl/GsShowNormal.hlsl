struct GS_VERTEX
{
      float4 Pos      : SV_POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float4 Tex      : TEXCOORD;  

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
      float4 wTangent  : TEXCOORD4;
};


[maxvertexcount(12)]
void main( triangle GS_VERTEX _input[3], inout LineStream<GS_VERTEX> GSNormalStream )
{
    //for( int i = 0; i < 3; ++i )
    //{
    //    // Compute screen coordinates
    //    GS_VERTEX output = _input[i];
    //    GSNormalStream.Append( output );        
    //}

    GSNormalStream.Append( _input[0] );  
    GSNormalStream.Append( _input[1] );  
    GSNormalStream.RestartStrip();

    GSNormalStream.Append( _input[1] );  
    GSNormalStream.Append( _input[2] );  
    GSNormalStream.RestartStrip();

    GSNormalStream.Append( _input[2] );  
    GSNormalStream.Append( _input[0] );  
    GSNormalStream.RestartStrip();

    for( int k = 0; k < 3; ++k )
    {
        //Ë³Ê±ÕëÒ»¸ö
        GS_VERTEX p1 = _input[k];
        p1.Color = float4(100.0f,0.0f,0.0f,1.0f);



        GS_VERTEX p2 = _input[k];
        p2.Color = float4(100.0f,0.0f,0.0f,1.0f);
        p2.Pos = p1.Pos + 500.0f * normalize(p1.wNormal);
        p2.Pos.w = 1.0f;
        
        //GSNormalStream.Append( p1 );
        GSNormalStream.Append( p1 );
        GSNormalStream.Append( p2 );
        GSNormalStream.RestartStrip();


        //GSNormalStream.Append( p1 );
        //GSNormalStream.Append( p2 );
        //GSNormalStream.Append( p1 );
        //GSNormalStream.RestartStrip();

    }
    
}
