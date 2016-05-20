#include "texture.h"
#include "Texture/Texture Loader/stb_image.h"
#include <iostream>
using namespace std;

Texture::Texture(const std::string& fileName, int texUnit_)
{
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);
	texUnit = texUnit_;
    if(data == NULL)
		cout<< "Unable to load texture: " << fileName << endl;

	glActiveTexture(texUnit);
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture); 
    
	     
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0); 

    stbi_image_free(data);
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}