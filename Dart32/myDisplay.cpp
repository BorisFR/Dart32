#include "myDisplay.hpp"

// ************************************************************************************

MyDisplay::MyDisplay()
{
    dartPosX[0] = DELTA_X;
    dartPosX[1] = DELTA_X + 98;
    dartPosX[2] = DELTA_X + 98 + 98;
}

void MyDisplay::setupLcd()
{
    pinMode(TFT_LED, OUTPUT);
    digitalWrite(TFT_LED, HIGH);

    tft.begin();
    tft.setRotation(3);
    // read diagnostics (optional but can help debug problems)
    /*uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    MY_DEBUG("Display Power Mode: 0x");
    MY_DEBUG_LN((x, HEX));
    x = tft.readcommand8(ILI9341_RDMADCTL);
    MY_DEBUG("MADCTL Mode: 0x");
    MY_DEBUG_LN((x, HEX));
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    MY_DEBUG("Pixel Format: 0x");
    MY_DEBUG_LN((x, HEX));
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    MY_DEBUG("Image Format: 0x");
    MY_DEBUG_LN((x, HEX));
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    MY_DEBUG("Self Diagnostic: 0x");
    MY_DEBUG_LN((x, HEX));*/
    drawScreenBaseInfo("Starting...");
}

// ************************************************************************************

String MyDisplay::on2(uint16_t number)
{
    if (number > 9)
        return String(number);
    return "0" + String(number);
}

// ************************************************************************************

String MyDisplay::on3(uint16_t number)
{
    if (number > 99)
        return String(number);
    if (number > 9)
        return "0" + String(number);
    return "00" + String(number);
}

void MyDisplay::buttonText(String text, uint16_t x, uint16_t y, uint16_t dy, uint16_t width, uint16_t height, uint32_t colorRect)
{
    tft.drawRect(x - width, y, width, height, colorRect);
    int16_t sizeX = tft.textWidth(text);
    tft.drawString(text, x - width + (width - sizeX) / 2, y + dy);
}

void MyDisplay::drawButtons()
{
    tft.setFreeFont(&Roboto_Black_32);
    tft.setTextColor(ILI9341_CYAN);
    buttonText("+", 320, DELTA_Y, 4, 80, 40, ILI9341_CYAN);
    buttonText("-", 320, DELTA_Y + 60, 4, 80, 40, ILI9341_CYAN);

    tft.setTextColor(ILI9341_GREEN);
    buttonText("OK", 320, DELTA_Y + 120, 4, 80, 40, ILI9341_GREEN);
}

void MyDisplay::lcdTextCenterX(String text, int16_t y)
{
    int16_t sizeX = tft.textWidth(text);
    tft.drawString(text, DELTA_X + MIDDLE_X - sizeX / 2, y);
    //MY_DEBUG_LN("Text: " + String(text));
    //MY_DEBUG_LN("SizeX: " + String(sizeX));
    //MY_DEBUG_LN("Pos: " + String(DELTA_X + MIDDLE_X - sizeX / 2));
}

//void MyDisplay::drawScreenGameName(String name, uint32_t colorBG = tft.color565(228, 203, 111), uint32_t colorText = tft.color565(246, 132, 123), uint32_t colorBorder = tft.color565(13, 18, 6), int16_t decalY = 0)
void MyDisplay::drawScreenGameName(String name, uint32_t colorBG, uint32_t colorText, uint32_t colorBorder, int16_t decalY)
{
    tft.fillScreen(colorBG);
    tft.setFreeFont(&Roboto_Black_66);
    int16_t sizeX = tft.textWidth(name);
    int32_t posX = DELTA_X + MIDDLE_X - sizeX / 2;
    int16_t posY = DELTA_Y + (240 - DELTA_Y) / 2 - 33 + decalY;
    tft.setTextColor(colorBorder);
    tft.drawString(name, posX, posY - 2);
    tft.drawString(name, posX, posY + 2);
    tft.drawString(name, posX - 2, posY);
    tft.drawString(name, posX + 2, posY);
    tft.drawString(name, posX - 2, posY - 2);
    tft.drawString(name, posX + 2, posY - 2);
    tft.drawString(name, posX - 2, posY + 2);
    tft.drawString(name, posX + 2, posY + 2);
    tft.setTextColor(colorText);
    tft.drawString(name, posX, posY);
}

void MyDisplay::refreshScreenBaseInfo(String ip)
{
    tft.fillRect(DELTA_X, 240 - 60 - 10, 320 - DELTA_X, 40, ILI9341_BLACK);
    lcdTextCenterX(ip, 240 - 60);
}

void MyDisplay::drawScreenBaseInfo(String ip)
{
    drawScreenGameName(THE_GAME_NAME, ILI9341_BLACK, ILI9341_BLUE, ILI9341_WHITE, -60);
    tft.setFreeFont(&Roboto_Black_32);
    tft.setTextColor(ILI9341_WHITE);
    lcdTextCenterX(ip, 240 - 60);
    tft.setFreeFont(&Roboto_Black_32);
    lcdTextCenterX("http://" + String(THE_HOST) + ".local", 240 - 100);
}

void MyDisplay::refreshScreenPlayersNumber(uint8_t number)
{
    tft.setFreeFont(&DSEG7_Classic_Mini_Regular_100);
    tft.setTextColor(SHADOW_NUMBER);
    tft.drawString("88", DELTA_X + 20, DELTA_Y + 70);
    tft.setTextColor(ILI9341_WHITE);
    tft.drawString(on2(number), DELTA_X + 20, DELTA_Y + 70);
}

void MyDisplay::drawScreenPlayersNumber()
{
    tft.fillScreen(ILI9341_BLACK);
    tft.setFreeFont(&Roboto_Black_32);
    tft.setTextColor(ILI9341_WHITE);
    tft.drawString("Number of", DELTA_X + 20, DELTA_Y);
    tft.drawString("   Players", DELTA_X + 20, DELTA_Y + 30);
    drawButtons();
}

void MyDisplay::drawScreenChooseGame()
{
    tft.fillScreen(ILI9341_BLACK);
    tft.setFreeFont(&Roboto_Black_48);
    tft.setTextColor(ILI9341_WHITE);
    int16_t dx = (50 - tft.textWidth("G")) / 2;
    tft.drawString("G", DELTA_X + 30 + dx, DELTA_Y);
    dx = (50 - tft.textWidth("A")) / 2;
    tft.drawString("A", DELTA_X + 30 + dx, DELTA_Y + 45);
    dx = (50 - tft.textWidth("M")) / 2;
    tft.drawString("M", DELTA_X + 30 + dx, DELTA_Y + 45 * 2);
    dx = (50 - tft.textWidth("E")) / 2;
    tft.drawString("E", DELTA_X + 30 + dx, DELTA_Y + 45 * 3);

    tft.setFreeFont(&Roboto_Black_32);
    tft.setTextColor(ILI9341_CYAN);
    buttonText("COUNT-UP", 320, DELTA_Y, 4, 190, 40, ILI9341_CYAN);
    buttonText("ZERO ONE", 320, DELTA_Y + 60, 4, 190, 40, ILI9341_CYAN);

    tft.setTextColor(ILI9341_GREEN);
    buttonText("MORE...", 320, DELTA_Y + 120, 4, 190, 40, ILI9341_GREEN);
}

#define MOVEY 15

void MyDisplay::drawScreenRound(uint8_t roundNumber)
{
    for (uint16_t i = 0; i < 11; i++)
    {
        tft.drawFastHLine(DELTA_X, DELTA_Y + MIDDLE_Y + MOVEY + i, 320 - DELTA_X, tft.color565(50 + i * 20, 50 + i * 20, 50 + i * 20));
        tft.drawFastHLine(DELTA_X, 240 - i, 320 - DELTA_X, tft.color565(50 + i * 20, 50 + i * 20, 50 + i * 20));
    }
    tft.fillRect(DELTA_X, DELTA_Y + MIDDLE_Y + 11 + MOVEY, 320 - DELTA_X, 240 - MIDDLE_Y - DELTA_Y - 21 - MOVEY, ILI9341_WHITE);
    tft.setTextColor(ILI9341_BLACK);
    tft.setFreeFont(&Roboto_Black_48);
    lcdTextCenterX("ROUND " + String(roundNumber), 240 - MIDDLE_Y / 2 - 22 + MOVEY - 9);
}

void MyDisplay::loopPlayer()
{
    //tft.fillRect(DELTA_X, DELTA_Y + MIDDLE_Y + 11, 320 - DELTA_X, 240 - MIDDLE_Y - DELTA_Y - 21, ILI9341_WHITE);
    tft.fillRect(DELTA_X, DELTA_Y + MIDDLE_Y + 11 + MOVEY, 320 - DELTA_X, 240 - MIDDLE_Y - DELTA_Y - 21 - MOVEY, ILI9341_WHITE);
    tft.setTextColor(colorText);
    lcdTextCenterX(memoText, 240 - MIDDLE_Y / 2 - 22 + MOVEY - 9);
}

void MyDisplay::drawFirstDartPlayer(String playerName, uint16_t playerScore, uint8_t roundNumber)
{
    voidMemoLoop = 1;
    memoText = playerName;
    memoText1 = memoText;
    memoText2 = "ROUND " + String(roundNumber);
    drawScreenDartScores();
    printPlayerScore(playerScore);
    drawScreenRound(roundNumber);

    colorText = ILI9341_BLACK;
    colorBorder = ILI9341_BLACK;
    colorText1 = colorText;
    colorBorder1 = colorBorder;
    colorText2 = colorBorder;
    colorBorder2 = colorText;
    memoTimeVisible = MEMO_ROUND;
    memoTimeHidden = MEMO_ROUND;
    memoVisible = false;
    lastMemo = millis();
}

void MyDisplay::drawScreenMemo()
{
    tft.setTextColor(colorBorder);
    tft.drawString(memoText, posX, posY - 2);
    tft.drawString(memoText, posX, posY + 2);
    tft.drawString(memoText, posX - 2, posY);
    tft.drawString(memoText, posX + 2, posY);
    tft.drawString(memoText, posX - 2, posY - 2);
    tft.drawString(memoText, posX + 2, posY - 2);
    tft.drawString(memoText, posX - 2, posY + 2);
    tft.drawString(memoText, posX + 2, posY + 2);
    tft.setTextColor(colorText);
    tft.drawString(memoText, posX, posY);
}

void MyDisplay::drawStartPlayer(String playerName, uint8_t roundNumber)
{
    voidMemoLoop = 0;
    memoText = playerName;
    memoText1 = memoText;
    memoText2 = memoText;
    tft.fillScreen(ILI9341_BLACK);
    drawScreenRound(roundNumber);
    tft.setFreeFont(&Roboto_Black_66);
    uint8_t dy = 33;
    int16_t width = tft.textWidth(playerName);
    if (width > (320 - DELTA_X))
    {
        tft.setFreeFont(&Roboto_Black_48);
        dy = 24;
        width = tft.textWidth(playerName);
        if (width > (320 - DELTA_X))
        {
            tft.setFreeFont(&Roboto_Black_32);
            dy = 16;
            width = tft.textWidth(playerName);
        }
    }
    posX = DELTA_X + MIDDLE_X - width / 2;
    posY = DELTA_Y + (240 - DELTA_Y) / 4 - dy;
    colorText = ILI9341_GREEN;
    colorBorder = tft.color565(0, 50, 0);
    colorText1 = colorText;
    colorBorder1 = colorBorder;
    colorText2 = colorBorder;
    colorBorder2 = colorText;
    memoTimeVisible = MEMO_VISIBLE;
    memoTimeHidden = MEMO_HIDE;
    drawScreenMemo();
    memoVisible = true;
    lastMemo = millis();
}

void MyDisplay::stopMemo()
{
    lastMemo = 0;
}

void MyDisplay::loopMemo()
{
    if (lastMemo == 0)
        return;
    if (memoVisible)
    {
        if ((millis() - lastMemo) > memoTimeVisible)
        {
            colorText = colorText2;
            colorBorder = colorBorder2;
            memoText = memoText2;
            switch (voidMemoLoop)
            {
            case 0:
                drawScreenMemo();
                break;
            case 1:
                loopPlayer();
                break;
            case 2:
                blinkScore();
                break;
            }
            memoVisible = false;
            lastMemo = millis();
        }
    }
    else
    {
        if ((millis() - lastMemo) > memoTimeHidden)
        {
            colorText = colorText1;
            colorBorder = colorBorder1;
            memoText = memoText1;
            switch (voidMemoLoop)
            {
            case 0:
                drawScreenMemo();
                break;
            case 1:
                loopPlayer();
                break;
            case 2:
                blinkScore();
                break;
            }
            memoVisible = true;
            lastMemo = millis();
        }
    }
}

void MyDisplay::drawScreenDartScores()
{
    tft.fillScreen(ILI9341_BLACK);
    // player score
    tft.setFreeFont(&DSEG7_Classic_Mini_Regular_100);
    tft.setTextColor(SHADOW_NUMBER);
    lcdTextCenterX("888", DELTA_Y);

    tft.drawFastVLine(DELTA_X + 98 - 2, 240 - 60, 60, ILI9341_RED);
    tft.drawFastVLine(DELTA_X + 98 - 1, 240 - 60, 60, ILI9341_RED);
    tft.drawFastVLine(DELTA_X + 98 + 98 - 2, 240 - 60, 60, ILI9341_RED);
    tft.drawFastVLine(DELTA_X + 98 + 98 - 1, 240 - 60, 60, ILI9341_RED);
    tft.setFreeFont(&DSEG7_Classic_Mini_Regular_58);
    tft.setTextColor(SHADOW_NUMBER);
    for (uint8_t i = 0; i < 3; i++)
        tft.drawString("88", dartPosX[i], DART_POS_Y);
}

void MyDisplay::printPlayerScore(uint16_t score)
{
    tft.setFreeFont(&DSEG7_Classic_Mini_Regular_100);
    tft.setTextColor(SHADOW_NUMBER);
    lcdTextCenterX("888", DELTA_Y);
    tft.setTextColor(ILI9341_GREEN);
    lcdTextCenterX(on3(score), DELTA_Y);
}

void MyDisplay::blinkScore()
{
    tft.setTextColor(colorText);
    lcdTextCenterX(memoText, DELTA_Y);
}

void MyDisplay::blinkPlayerScore(uint16_t score)
{
    tft.setFreeFont(&DSEG7_Classic_Mini_Regular_100);
    voidMemoLoop = 2;
    colorText1 = ILI9341_GREEN;
    colorText2 = ILI9341_RED;
    memoText1 = on3(score);
    memoText2 = memoText1;
    memoTimeVisible = MEMO_ROUND;
    memoTimeHidden = MEMO_ROUND;
    memoVisible = true;
    lastMemo = millis();
}

void MyDisplay::printDartScore(uint8_t dartNumber, uint8_t mult, uint8_t value)
{
    switch (mult)
    {
    case 1:
        break;
    case 2:
        tft.fillRect(dartPosX[dartNumber] - 2, DART_POS_Y - 2, 98 + 2 * 2, 60 + 2, ILI9341_DARKCYAN);
        break;
    case 3:
        tft.fillRect(dartPosX[dartNumber] - 2, DART_POS_Y - 2, 98 + 2 * 2, 60 + 2, ILI9341_DARKGREEN);
        break;
    }

    tft.setFreeFont(&DSEG7_Classic_Mini_Regular_58);
    tft.setTextColor(SHADOW_NUMBER);
    tft.drawString("88", dartPosX[dartNumber], DART_POS_Y);
    tft.setTextColor(ILI9341_WHITE);
    switch (mult)
    {
    case 0: // pas joué
        break;
    case 254: // pas touché
        tft.drawString("--", dartPosX[dartNumber], DART_POS_Y);
        break;
    default:
        tft.drawString(on2(mult * value), dartPosX[dartNumber], DART_POS_Y);
        break;
    }
}