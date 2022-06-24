#define STB_IMAGE_IMPLEMENTATION

#include <GL/glut.h>
#include "../include/SpriteSheet.hpp"
#include "../stb_image.h"

SpriteSheet::SpriteSheet() {

}

SpriteSheet::SpriteSheet(int t, int tW, int tH, int sW, int sH) {
    this->texture = t;
    this->textureWidth = tW;
    this->textureHeight = tH;
    this->spriteWidth = sW;
    this->spriteHeight = sH;
}

unsigned char* SpriteSheet::getImage() {
    return this->image;
}

void SpriteSheet::load_sprite_sheet(const char* file) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    this->image = stbi_load(file, &width, &height, &channels, STBI_rgb);

    if(image == NULL) {
        const char* a = stbi_failure_reason();
        printf("%s\n", a);
        printf("Error loading the %s image\n", file);
        exit(1);
    }

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    printf("Image loaded %dx%d pixels and %d channels\n", width, height, channels);

    //stbi_image_free(image);
}

void SpriteSheet::draw_sprite(float posX, float posY, int frameIndex) {
    const float vertices[] = {
        posX, posY, posX + spriteWidth, posY, posX + spriteWidth,
        posY + spriteHeight, posX, posY + spriteHeight
    };
    const float tw = float(spriteWidth) / textureWidth;
    const float th = float(spriteHeight) / textureHeight;

    const int numPerRow = textureWidth / spriteWidth;
    const float tx = (frameIndex % numPerRow) * tw;
    const float ty = (frameIndex / numPerRow + 1) * th;  

    const float texVerts[] = {
        tx, ty, tx + tw, ty, tx + tw, ty + th, tx, ty + th
    };

    glVertexPointer(2, GL_FLOAT, 0,  vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texVerts);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
