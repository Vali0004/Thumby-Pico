// A more complex example of using the Thumby Pico C/C++ library to draw a simple voxel terrain

#include <thumby.h>
#include "D1Dither.h"
#include "D1Height.h"

s32 mapWidth = 256;
s32 mapHeight = 256;
s32 mapTotal = 256*256;
// This fails on C 'mapWidth * mapWidth;'

int cameraX = 128;
int cameraY = 128;
int cameraZ = 200;
float cameraAngle = 0;
int cameraHorizon = -200;
int cameraDistance = 128;
bool viewMode = 0;

void setup() {
    // Sets up buttons, audio, link pins, and screen
    Thumby_begin();
    // Init duplex UART for Thumby to PC comms
    Serial_begin(115200);
}

void control() {
    // Move on XY plane of in viewMode 1, else rotate camera left/right or up/down
    if (viewMode) {
        if (Thumby_isPressed(BUTTON_L)) {
            cameraX += 1;
        }
        else if (Thumby_isPressed(BUTTON_R)) {
            cameraX -= 1;
        }
        else if (Thumby_isPressed(BUTTON_U)) {
            cameraY += 1;
        }
        else if (Thumby_isPressed(BUTTON_D)) {
            cameraY -= 1;
        }
        else if (Thumby_isPressed(BUTTON_A)) {
            cameraZ += 1;
        }
    }
    else {
        if (Thumby_isPressed(BUTTON_L)) {
            cameraAngle += 0.05f;
        }
        else if (Thumby_isPressed(BUTTON_R)) {
            cameraAngle -= 0.05f;
        }
        else if (Thumby_isPressed(BUTTON_U)) {
            cameraHorizon -= 25;
        }
        else if (Thumby_isPressed(BUTTON_D)) {
            cameraHorizon += 25;
        }
        else if (Thumby_isPressed(BUTTON_A)) {
            cameraZ -= 1;
        }
    }
    // Change view mode
    if (Thumby_isPressed(BUTTON_B)) {
        viewMode = !viewMode;
        while(Thumby_isPressed(BUTTON_B)) {}
    }
}
float lastUpdateTime = 0.f;
void displayFPS() {
    Thumby_setCursor(0, 0);
    Thumby_printf("%i", (s32)(1000.f / (millis() - lastUpdateTime)));
    lastUpdateTime = millis();
}
void render() {
    Thumby_clear();
    float sinang = sin(cameraAngle);
    float cosang = cos(cameraAngle);
    float deltaz = 1.0f;
    float z = 10.0f;
    while (z < cameraDistance) {
        // 90 degree field of view
        float plx =  -cosang * z - sinang * z;
        float ply =   sinang * z - cosang * z;
        float prx =   cosang * z - sinang * z;
        float pry =  -sinang * z - cosang * z;
        float dx = (prx - plx) / THUMBY_SCREEN_WIDTH;
        float dy = (pry - ply) / THUMBY_SCREEN_WIDTH;
        plx += cameraX;
        ply += cameraY;
        float invz = 1.0f / z * 240.0f;
        for (u8 i = 0; i != THUMBY_SCREEN_WIDTH; ++i) {
            if (plx >= 0 && plx < mapWidth && ply >= 0 && ply < mapHeight) {
                s32 byteIndex = (s32)(((s32)(ply) * mapWidth) + (s32)(plx));
                if (byteIndex >= 0 && byteIndex < mapTotal) {
                    if (D1Dither[byteIndex] > 0) {
                        s32 heightOnScreen = (s32)(((cameraZ - D1Height[byteIndex]) * invz + cameraHorizon) / 12);
                        if (heightOnScreen >= 0 && heightOnScreen < THUMBY_SCREEN_WIDTH) {
                            Thumby_drawPixel(i, heightOnScreen, 1);
                        }
                    }
                }
            }
            plx += dx;
            ply += dy;
        }
        deltaz += 0.05f;
        z += deltaz;
    }
    displayFPS();
    Thumby_writeBuffer(Thumby_getBuffer(), Thumby_getBufferSize());
}
void loop() {
    control();
    render();
}