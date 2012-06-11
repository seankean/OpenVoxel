TODO:



Prepare as example attaching shader code.
Attach buffer to empty VertexAttributes(x,y,z).
Use rply reading(for now) code to copy raw float values to matrix4 double unchanged (waste a column, yes) as XYZA
    add the setup code for this in the paint routine before the QUAD defs.
Call the shader with the level(0-7),mode(mono,rgb),bitdepth etc.
Apply rotation, translation and scaling.
draw the 
The precompiled shader is called automatically when needed.  Precompiled shader is passed vertex co-ordinates in the rendered screen context and sets the color bit(s) appropriatly then should be written to the color buffer owned by the quad texture.  the model can be shown and manipulated on one display in a gui widget and the voxelized data sent to the other screen ideally.  

test and debug windows version although linux works well currently


***

The recorder example in the openNI area and the NITE docs and RBGdemo should be enough example for us to capture data into matrix4d.



