<?xml version ="1.0" encoding="unicode" ?>
<ZStencil name="Overlay"   >
  <depth    enable="true" writeable="false"  writemask="0" compare = "always" />
  <stencil ref="1" enable="true" readmask ="0xff" writemask="0xff" >
    <ccw fail="keep"  zfail="keep" pass="keep" compare = "always"/>
    <cw  fail="keep"  zfail="keep" pass="keep" compare = "always"/>
  </stencil>
</ZStencil>