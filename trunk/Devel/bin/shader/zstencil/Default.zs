<?xml version ="1.0" encoding="unicode" ?>
<ZStencil name="Default"   >
  <depth    enable="true" writemask="1" compare = "less" />
  <stencil ref="1" enable="true" readmask ="0xff" writemask="0xff" >
    <front fail="keep"  zfail="keep" pass="keep" />
    <back  fail="keep"  zfail="keep" pass="keep" />
  </stencil>
</ZStencil>