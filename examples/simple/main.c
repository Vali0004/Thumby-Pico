// A simple example showing how to use main components of the Thumby Pico C/C++ library
#include <thumby.h>

char sendBuf[7] = "Hello!";
uint8_t dataBuf[7];
uint8_t packedBuf[10];
void removeRxBytes(){
    delay(10);
    while (Serial1_available() > 0) {
        Serial1_read();
    }
}
void playMessageSound() {
    for (u16 f = 3000; f < 10000; f += 250) {
        Thumby_play(f);
        delay(10);
    }
    // Stop the audio from playing forever
    Thumby_stopPlay();
}
void send() {
    // If packing does not fail, send over Serial1
    if (Thumby_linkPack((uint8_t*)sendBuf, sizeof(sendBuf), packedBuf, sizeof(packedBuf)) != -1) {   
        // Write on Serial1 (this this initialzed in the Thumby Lib)
        Serial1_write(packedBuf, sizeof(packedBuf));
        // Remove echoed RX bytes
        removeRxBytes();
    }
    else {
        Thumby_setCursor(0, 20);
        Thumby_print("Packing Error!");
    }
}
void receive(){
  // If enough bytes in RX to fit packed message, read and try to unpack
    if (Serial1_available() >= 10) {
        size_t read = Serial1_readBytes(packedBuf, sizeof(packedBuf));
        if (Thumby_linkUnpack(packedBuf, read, dataBuf, sizeof(dataBuf)) != -1) {
            Thumby_setCursor(0, 20);
            Thumby_print((char*)dataBuf);
            playMessageSound();
        }
        else {
            Thumby_setCursor(0, 20);
            Thumby_print("Unacking Error!");
            removeRxBytes();
        }
    }
}
void setup() {
    Thumby_begin();
    Serial_begin(115200);
    removeRxBytes();
}
void loop() {
    Thumby_clear();
    Thumby_setCursor(0, 0);
    Thumby_print("Thumby!");
    if (Thumby_isPressed(BUTTON_A | BUTTON_B)) {
        send();
        Thumby_setCursor(0, 10);
        Thumby_print("A/B Pressed!");
    }
    receive();
    Thumby_writeBuffer(Thumby_getBuffer(), Thumby_getBufferSize());
}