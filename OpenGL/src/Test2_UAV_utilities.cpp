#include "Test2_UAV_utilities.h"


int gWIDTH = 80;
int gHeight = 60;


void CreateTextureData(unsigned char* texture1Data, int width, int height, int nChannels)
{
	unsigned char* ptr = texture1Data;
	for (int i = 0; i < height; i++)
	{
		unsigned char data = (i & 8) ? 0x00 : 0xFF;
		for (int j = 0; j < width; j++)
		{
			*ptr++ = (j & 8) ? data : ~data;
			*ptr++ = (j & 8) ? data : ~data;
			*ptr++ = (j & 8) ? data : ~data;

		}
	}

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	
}

