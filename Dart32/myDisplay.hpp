#pragma once
#ifndef MYDISPLAY_H
#define MYDISPLAY_H

#include "myTools.h"

#include "SPI.h"
#include "TFT_eSPI.h"

#include "configuration.h"

/* in platformio.ini
#define TFT_DC 13
#define TFT_CS 5
#define TFT_MOSI 27
#define TFT_CLK 22
#define TFT_MISO 23
#define TFT_RST 18
#define TFT_LED 19 */

#include "fonts/Roboto_Black_32.h"
#include "fonts/Roboto_Black_48.h"
#include "fonts/Roboto_Black_66.h"
#include "fonts/DSEG7_Classic_Mini_Regular_58.h"
#include "fonts/DSEG7_Classic_Mini_Regular_100.h"

#define MIDDLE_X 145
#define MIDDLE_Y 88
#define DELTA_X 30
#define DELTA_Y 64
#define DART_POS_Y 240 - 60

#define MEMO_VISIBLE 300
#define MEMO_HIDE 100
#define MEMO_ROUND 900

#define SHADOW_NUMBER tft.color565(50, 50, 50)

class MyDisplay
{
public:
    MyDisplay();
    void setupLcd();
    void drawScreenBaseInfo(String ip);
    void refreshScreenBaseInfo(String ip);
    void drawScreenPlayersNumber();
    void refreshScreenPlayersNumber(uint8_t number);
    void drawScreenChooseGame();
    void drawStartPlayer(String playerName, uint8_t roundNumber);
    void drawFirstDartPlayer(String playerName, uint16_t playerScore, uint8_t roundNumber);
    void drawScreenDartScores();
    void printDartScore(uint8_t dartNumber, uint8_t mult, uint8_t value);
    void printPlayerScore(uint16_t score);
    void blinkPlayerScore(uint16_t score);
    void stopMemo();
    void loopMemo();

private:
    uint8_t dartPosX[3];
    String on2(uint16_t number);
    String on3(uint16_t number);
    String memoText;
    String memoText1;
    String memoText2;
    uint16_t posX;
    uint16_t posY;
    uint32_t colorText;
    uint32_t colorBorder;
    uint32_t colorText1;
    uint32_t colorBorder1;
    uint32_t colorText2;
    uint32_t colorBorder2;
    unsigned long lastMemo = 0;
    bool memoVisible;
    TFT_eSPI tft = TFT_eSPI();
    void buttonText(String text, uint16_t x, uint16_t y, uint16_t dy, uint16_t width, uint16_t height, uint32_t colorRect);
    void drawButtons();
    void lcdTextCenterX(String text, int16_t y);
    void drawScreenGameName(String name, uint32_t colorBG, uint32_t colorText, uint32_t colorBorder, int16_t decalY = 0);
    void drawScreenRound(uint8_t roundNumber);
    void drawScreenMemo();
    uint16_t memoTimeVisible;
    uint16_t memoTimeHidden;
    uint8_t voidMemoLoop;
    void loopPlayer();
    void blinkScore();
};

#endif