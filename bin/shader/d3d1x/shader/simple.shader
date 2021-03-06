<?xml version ="1.0" encoding="unicode" ?>
<shader>
   <Direct3D10>
       <Paramaters>
              <sampler  name="diffTexture"  stage="0"address="repeat"/>
              <sampler  name="videoTexure"  stage="1" manager="default" file="videoTexture.mpeg" address="clamp"/>
              <sampler  name="ToonShaderTexture"  stage="3" manager="default" file="Toon.bmp" address="clamp"/>
       </Paramaters>

       <shader file="simple.vertex" type="VertexShader"/>
       <shader file="simple.pixel"  type="PixelShader" />
    </Direct3D10>

  <Direct3D11>
    <Paramaters>
      <sampler  name="diffTexture"  stage="0"address="repeat"/>
      <sampler  name="videoTexure"  stage="1" manager="default" file="videoTexture.mpeg" address="clamp"/>
      <sampler  name="ToonShaderTexture"  stage="3" manager="default" file="Toon.bmp" address="clamp"/>
    </Paramaters>

    <shader file="simple.vertex" type="VertexShader"/>
    <shader file="simple.pixel"  type="PixelShader" />
  </Direct3D11>
  
</shader>