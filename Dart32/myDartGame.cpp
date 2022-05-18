#include "myDartGame.hpp"

MyDartGame::MyDartGame()
{
    for (uint8_t i = 0; i < MAX_PLAYERS; i++)
    {
        playersName[i] = "Player " + String(i + 1);
    }
}

BOARD_STATE MyDartGame::getBoardState()
{
    return boardState;
}

void MyDartGame::setBoardState(BOARD_STATE state)
{
    boardState = state;
    _changeToScreen();
    timeDrawScreen = millis();
    switch (boardState)
    {
    case BOARD_STATE::STATE_BOOT:
        drawScreenTimeOut = TIMEOUT_SCREEN_WELCOME;
        nextBoardState = BOARD_STATE::STATE_NUMBER_PLAYERS;
        break;
    case BOARD_STATE::STATE_BASE_INFO:
        drawScreenTimeOut = 0;
        break;
    case BOARD_STATE::STATE_NUMBER_PLAYERS:
        drawScreenTimeOut = 0;
        break;
    case BOARD_STATE::STATE_CHOOSE_GAME:
        drawScreenTimeOut = 0;
        break;
    case BOARD_STATE::STATE_PLAYER_STARTING:
        drawScreenTimeOut = 0;
        break;
    case BOARD_STATE::STATE_WAITING_FIRST_DART:
        drawScreenTimeOut = 0;
        break;
    case BOARD_STATE::STATE_WAITING_SECOND_DART:
        drawScreenTimeOut = 0;
        break;
    case BOARD_STATE::STATE_WAITING_THIRD_DART:
        drawScreenTimeOut = 0;
        break;
    case BOARD_STATE::STATE_WAITING_PLAYER_VALID:
        drawScreenTimeOut = 0;
        break;
    case BOARD_STATE::STATE_GAME_RESULT:
        drawScreenTimeOut = 0;
        break;
    }
}

bool MyDartGame::addOnePlayer()
{
    numberOfPlayers++;
    return true;
}

bool MyDartGame::removeOnePlayer()
{
    if (numberOfPlayers > 1)
    {
        numberOfPlayers--;
        return true;
    }
    return false;
}

uint8_t MyDartGame::getNumberOfPlayers()
{
    return numberOfPlayers;
}

void MyDartGame::setNumberOfPlayers(uint8_t number)
{
    numberOfPlayers = number;
}

void MyDartGame::setPlayerName(uint8_t number, String name)
{
    playersName[number] = name;
}

CURRENT_GAME MyDartGame::getGame()
{
    return game;
}

void MyDartGame::setGame(CURRENT_GAME _game)
{
    game = _game;
    roundNumber = 0;
    currentPlayer = 0;
    dartNumber = 0;
    for (uint8_t i = 0; i < MAX_PLAYERS; i++)
    {
        switch (game)
        {
        case CURRENT_GAME::GAME_COUNTUP:
            playersScore[i] = 0;
            break;
        case CURRENT_GAME::GAME_ZEROONE:
            playersScore[i] = 301;
            break;
        case CURRENT_GAME::GAME_CRICKET:
            playersScore[i] = 0; // TODO: implement good rule
            break;
        }
        for (uint8_t j = 0; j < 3; j++)
        {
            for (uint8_t k = 0; k < MAX_ROUNDS; k++)
            {
                playersDartsPoints[i][k][j] = 0;
                playersDartsMult[i][k][j] = 0;
            }
        }
    }
    setBoardState(BOARD_STATE::STATE_PLAYER_STARTING);
}

void MyDartGame::setMaxRound(uint8_t number)
{
    numberOfRounds = number;
}

uint8_t MyDartGame::getRoundNumber()
{
    return roundNumber + 1;
}

String MyDartGame::getCurrentPlayerName()
{
    return playersName[currentPlayer];
}

uint16_t MyDartGame::getCurrentPlayerScore()
{
    return playersScore[currentPlayer];
}

void MyDartGame::doDartMissed()
{
    playersDartsPoints[currentPlayer][roundNumber][dartNumber] = 0;
    playersDartsMult[currentPlayer][roundNumber][dartNumber] = 254;
    nextDart();
}

void MyDartGame::doDart(String dart)
{
    uint8_t index = 0;
    uint8_t search = dart[1];
    while (index < 70 && keymap[index] != search)
    {
        index++;
    }
    if (index >= 70) // if ok, it's a bug!
        return;
    uint8_t points = keyMult[index] * keyValue[index];
    playersDartsPoints[currentPlayer][roundNumber][dartNumber] = keyValue[index];
    playersDartsMult[currentPlayer][roundNumber][dartNumber] = keyMult[index];
    switch (game)
    {
    case CURRENT_GAME::GAME_COUNTUP:
        playersScore[currentPlayer] += points;
        break;
    case CURRENT_GAME::GAME_ZEROONE:
        if (playersScore[currentPlayer] - points >= 0)
        {
            playersScore[currentPlayer] -= points;
            if (playersScore[currentPlayer] == 0)
            {
                // TODO: WINNER!!!
                boardState = BOARD_STATE::STATE_GAME_RESULT;
                _changeToScreen();
                return;
            }
        }
        else
        {
            // TODO: bad, so play sound
            playersScore[currentPlayer] = saveCurrentScore;
            nextPlayer();
            return;
        }
        break;
    case CURRENT_GAME::GAME_CRICKET:
        playersScore[currentPlayer] += points; // TODO : implement good rule break;
    }

    nextDart();
}

void MyDartGame::nextDart()
{
    dartNumber++;
    if (dartNumber == 3)
    {
        nextPlayer();
        return;
    }
    if (dartNumber == 1)
        boardState = BOARD_STATE::STATE_WAITING_SECOND_DART;
    else
        boardState = BOARD_STATE::STATE_WAITING_THIRD_DART;
    _changeToScreen();
}

uint8_t MyDartGame::getDartValue(uint8_t dartNumber)
{
    return playersDartsPoints[currentPlayer][roundNumber][dartNumber];
}

uint8_t MyDartGame::getDartMult(uint8_t dartNumber)
{
    return playersDartsMult[currentPlayer][roundNumber][dartNumber];
}

uint8_t MyDartGame::getDartPoints(uint8_t dartNumber)
{
    return playersDartsMult[currentPlayer][roundNumber][dartNumber] * playersDartsPoints[currentPlayer][roundNumber][dartNumber];
}

void MyDartGame::doNextPlayer()
{
    dartNumber = 0;
    currentPlayer++;
    if (currentPlayer >= numberOfPlayers)
    {
        currentPlayer = 0;
        roundNumber++;
        if (roundNumber >= numberOfRounds)
        {
            // TODO: END
            boardState = BOARD_STATE::STATE_GAME_RESULT;
            //_changeToScreen();
        }
        else
        {
            // TODO: show change round (& player)
            boardState = BOARD_STATE::STATE_WAITING_FIRST_DART;
            //_changeToScreen();
            saveCurrentScore = playersScore[currentPlayer];
        }
    }
    else
    {
        // TODO: show change player
        if (roundNumber == 0)
            boardState = BOARD_STATE::STATE_PLAYER_STARTING;
        else
            boardState = BOARD_STATE::STATE_WAITING_FIRST_DART;
        //_changeToScreen();
        saveCurrentScore = playersScore[currentPlayer];
    }
    _changeToScreen();
}

void MyDartGame::nextPlayer()
{
    boardState = BOARD_STATE::STATE_WAITING_PLAYER_VALID;
    _changeToScreen();
    return;
}

BOARD_STATE MyDartGame::getNextBoardState()
{
    return nextBoardState;
}

void MyDartGame::begin(void (*changeToScreen)())
{
    _changeToScreen = changeToScreen;
    setBoardState(BOARD_STATE::STATE_BASE_INFO);
    /*boardState = BOARD_STATE::STATE_BASE_INFO;
    _changeToScreen();
    timeDrawScreen = millis();
    drawScreenTimeOut = TIMEOUT_SCREEN_WELCOME;
    nextBoardState = BOARD_STATE::STATE_NUMBER_PLAYERS;*/
}

void MyDartGame::loop()
{
    if (drawScreenTimeOut > 0)
    {
        if ((millis() - timeDrawScreen) > drawScreenTimeOut)
        {
            drawScreenTimeOut = 0;
            boardState = nextBoardState;
            _changeToScreen();
        }
    }
}