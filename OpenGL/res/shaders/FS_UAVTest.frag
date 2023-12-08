#version 440
//#extension GL_ARB_shader_image_load_store : require
//#extension GL_EXT_shader_image_load_store : require

precision highp uimage2D;
layout( binding=4, rgba8ui) readonly uniform uimage2D u_image0;

out vec4 FragColor;

void main()
{
	
	if(gl_FragCoord.x < 400)
	{
		FragColor = vec4(imageLoad(u_image0, uvec2(gl_FragCoord.x, gl_FragCoord.y)));
	}
	else
	{
		FragColor = vec4(0, 0, 0, 0);
	}
	

}
