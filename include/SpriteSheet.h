#ifndef SPRITESHEET_H_
#define SPRITESHEET_H_

#include <stdio.h>

class SpriteSheet {
    private:
        int spriteWidth;        
        int spriteHeight;

        int textureWidth;
        int textureHeight;

        int texture;

        unsigned char* image;

    public:
        SpriteSheet(int t, int tW, int tH, int sW, int sH);

        SpriteSheet();

        void draw_sprite(float posX, float posY, int frameIndex);

        void load_sprite_sheet(const char* file); 

        unsigned char* getImage();
};

#endif
