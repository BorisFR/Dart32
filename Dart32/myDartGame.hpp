#pragma once
#ifndef MYDARTGAME_H
#define MYDARTGAME_H

#include <Arduino.h>

#include "configuration.h"

enum class BOARD_STATE : uint8_t
{
    STATE_BOOT = 0,
    STATE_BASE_INFO = 5,
    STATE_NUMBER_PLAYERS = 10,
    STATE_CHOOSE_GAME = 15,
    STATE_PLAYER_STARTING = 18,
    STATE_WAITING_FIRST_DART = 20,
    STATE_WAITING_SECOND_DART = 25,
    STATE_WAITING_THIRD_DART = 30,
    STATE_WAITING_PLAYER_VALID = 35,
    STATE_GAME_RESULT = 40
};

enum class CURRENT_GAME : uint8_t
{
    GAME_COUNTUP = 0,
    GAME_ZEROONE = 1,
    GAME_CRICKET = 2
};

#define MAX_PLAYERS 10
#define MAX_ROUNDS 100

class MyDartGame
{
public:
    MyDartGame();
    void begin(void (*changeToScreen)());
    BOARD_STATE getBoardState();
    void loop();
    void setBoardState(BOARD_STATE state);
    BOARD_STATE getNextBoardState();
    bool addOnePlayer();
    bool removeOnePlayer();
    uint8_t getNumberOfPlayers();
    void setNumberOfPlayers(uint8_t number);
    void setPlayerName(uint8_t number, String name);
    CURRENT_GAME getGame();
    void setGame(CURRENT_GAME game);
    void setMaxRound(uint8_t number);
    uint8_t getRoundNumber();
    String getCurrentPlayerName();
    uint16_t getCurrentPlayerScore();
    void doDart(String dart);
    void doDartMissed();
    uint8_t getDartValue(uint8_t dartNumber);
    uint8_t getDartMult(uint8_t dartNumber);
    uint8_t getDartPoints(uint8_t dartNumber);
    void doNextPlayer();

private:
    void (*_changeToScreen)();
    BOARD_STATE boardState = BOARD_STATE::STATE_BOOT;
    unsigned long drawScreenTimeOut;
    unsigned long timeDrawScreen;
    BOARD_STATE nextBoardState;
    uint8_t numberOfPlayers = 1;
    uint8_t numberOfRounds;
    uint8_t roundNumber;
    uint8_t dartNumber;
    void nextDart();
    CURRENT_GAME game;
    String playersName[MAX_PLAYERS];
    uint16_t playersScore[MAX_PLAYERS];
    uint16_t playersDartsPoints[MAX_PLAYERS][MAX_ROUNDS][3];
    uint16_t playersDartsMult[MAX_PLAYERS][MAX_ROUNDS][3];
    uint16_t saveCurrentScore;
    uint8_t currentPlayer;
    void nextPlayer();

    uint8_t keymap[70] = {'0', '1', '2', '4', '5', '6', '7', '8', '9', '*',
                          'a', 'c', 'd', 'e', 'f', 'g', 'h', 'i', '?', '!',
                          'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', '&',
                          't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', '@', '#',
                          'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', '}', '(',
                          'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', ')', '=',
                          'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '+', '-', ']'};
    uint8_t keyMult[70] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                           2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                           0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                           2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                           3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    uint8_t keyValue[70] = {7, 19, 3, 17, 2, 15, 16, 8, 11, 14,
                            7, 19, 3, 17, 2, 15, 16, 8, 11, 14,
                            7, 19, 3, 17, 2, 15, 16, 8, 11, 14,
                            0, 0, 0, 0, 0, 0, 0, 0, 25, 30,
                            1, 18, 4, 13, 6, 10, 20, 5, 12, 9,
                            1, 18, 4, 13, 6, 10, 20, 5, 12, 9,
                            1, 18, 4, 13, 6, 10, 20, 5, 12, 9};
};

#endif