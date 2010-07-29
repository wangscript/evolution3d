struct PS_INPUT
{
      float4 Color    : COLOR;
};

float4 main( PS_INPUT input) : COLOR0
{
      return input.Color;
}

