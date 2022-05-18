#include <Arduino.h>

#include <SPIFFS.h>

//#include "configuration.h"
#include "myWifi.hpp"
#include "MyWeb.hpp"
#include "myDisplay.hpp"
#include "myDartBoard.h"
#include "myDartGame.hpp"
#include "myTools.h"
#include "ArduinoJson.h"

MyDisplay myDisplay = MyDisplay();
MyDartGame myDartGame = MyDartGame();
MyWifi myWifi = MyWifi();
//MyWeb myWeb = MyWeb();
extern MyWeb myWeb;
StaticJsonDocument<2000> jsonReceived;
StaticJsonDocument<2000> jsonSend;

void setupFS()
{
  if (!SPIFFS.begin())
  {
    MY_DEBUG_LN("LittleFS Mount Failed");
    return;
  }
  MY_DEBUG_LN("SPIFFS Mount OK");
  MY_DEBUG_LN("Total : " + String(SPIFFS.totalBytes()));
  MY_DEBUG_LN("Used :  " + String(SPIFFS.usedBytes()));
  MY_DEBUG_LN("Free :  " + String(SPIFFS.totalBytes() - SPIFFS.usedBytes()));
}

// ************************************************************************************

void changeToScreen()
{
  switch (myDartGame.getBoardState())
  {
  case BOARD_STATE::STATE_BOOT: // nothing to display
    break;
  case BOARD_STATE::STATE_BASE_INFO: // welcome with @IP
    myDisplay.drawScreenBaseInfo(myWifi.getIP());
    //myWeb.sendToAll("");
    break;
  case BOARD_STATE::STATE_NUMBER_PLAYERS: // choosing number of players
    myDisplay.refreshScreenPlayersNumber(myDartGame.getNumberOfPlayers());
    break;
  case BOARD_STATE::STATE_CHOOSE_GAME: // choosing the game
    myDisplay.drawScreenChooseGame();
    break;
  case BOARD_STATE::STATE_PLAYER_STARTING:
    myDisplay.drawStartPlayer(myDartGame.getCurrentPlayerName(), myDartGame.getRoundNumber());
    break;
  case BOARD_STATE::STATE_WAITING_FIRST_DART:
    myDisplay.drawFirstDartPlayer(myDartGame.getCurrentPlayerName(), myDartGame.getCurrentPlayerScore(), myDartGame.getRoundNumber());
    break;
  case BOARD_STATE::STATE_WAITING_SECOND_DART:
    myDisplay.printPlayerScore(myDartGame.getCurrentPlayerScore());
    myDisplay.printDartScore(0, myDartGame.getDartMult(0), myDartGame.getDartValue(0));
    break;
  case BOARD_STATE::STATE_WAITING_THIRD_DART:
    myDisplay.printPlayerScore(myDartGame.getCurrentPlayerScore());
    myDisplay.printDartScore(0, myDartGame.getDartMult(0), myDartGame.getDartValue(0));
    myDisplay.printDartScore(1, myDartGame.getDartMult(1), myDartGame.getDartValue(1));
    break;
  case BOARD_STATE::STATE_WAITING_PLAYER_VALID:
    myDisplay.printPlayerScore(myDartGame.getCurrentPlayerScore());
    myDisplay.printDartScore(0, myDartGame.getDartMult(0), myDartGame.getDartValue(0));
    myDisplay.printDartScore(1, myDartGame.getDartMult(1), myDartGame.getDartValue(1));
    myDisplay.printDartScore(2, myDartGame.getDartMult(2), myDartGame.getDartValue(2));
    myDisplay.blinkPlayerScore(myDartGame.getCurrentPlayerScore());
    break;
  case BOARD_STATE::STATE_GAME_RESULT:
    break;
  }
}

void sendGotoScreen()
{
  jsonSend.clear();
  switch (myDartGame.getBoardState())
  {
  case BOARD_STATE::STATE_NUMBER_PLAYERS:
    jsonSend["goto"] = "players";
    break;
  case BOARD_STATE::STATE_CHOOSE_GAME:
    jsonSend["goto"] = "choose";
    break;
  case BOARD_STATE::STATE_BASE_INFO:
  case BOARD_STATE::STATE_BOOT:
    break;
  case BOARD_STATE::STATE_GAME_RESULT:
    jsonSend["goto"] = "result";
    break;
  default:
    jsonSend["goto"] = "game";
    break;
  }
  String info;
  serializeJson(jsonSend, info);
  Serial.println("Sending: " + info);
  myWeb.sendToAll(info);
}

void gotoChoosePlayersNumber()
{
  myDisplay.stopMemo();
  myDisplay.drawScreenPlayersNumber();
  myDartGame.setBoardState(BOARD_STATE::STATE_NUMBER_PLAYERS);
}

// ************************************************************************************

void pressButtonTop()
{
  switch (myDartGame.getBoardState())
  {
  case BOARD_STATE::STATE_BOOT:
    break;
  case BOARD_STATE::STATE_BASE_INFO:
    gotoChoosePlayersNumber();
    sendGotoScreen();
    break;
  case BOARD_STATE::STATE_NUMBER_PLAYERS:
    if (myDartGame.addOnePlayer())
    {
      changeToScreen();
    }
    break;
  case BOARD_STATE::STATE_CHOOSE_GAME:
    myDartGame.setGame(CURRENT_GAME::GAME_COUNTUP);
    myDartGame.setMaxRound(8);
    break;
  case BOARD_STATE::STATE_PLAYER_STARTING:
    break;
  case BOARD_STATE::STATE_WAITING_FIRST_DART:
    break;
  case BOARD_STATE::STATE_WAITING_SECOND_DART:
    break;
  case BOARD_STATE::STATE_WAITING_THIRD_DART:
    break;
  case BOARD_STATE::STATE_WAITING_PLAYER_VALID:
    break;
  case BOARD_STATE::STATE_GAME_RESULT:
    break;
  }
}

void pressButtonMiddle()
{
  switch (myDartGame.getBoardState())
  {
  case BOARD_STATE::STATE_BOOT:
    break;
  case BOARD_STATE::STATE_BASE_INFO:
    gotoChoosePlayersNumber();
    sendGotoScreen();
    break;
  case BOARD_STATE::STATE_NUMBER_PLAYERS:
    if (myDartGame.removeOnePlayer())
    {
      changeToScreen();
    }
    break;
  case BOARD_STATE::STATE_CHOOSE_GAME:
    myDartGame.setGame(CURRENT_GAME::GAME_ZEROONE);
    myDartGame.setMaxRound(255);
    break;
  case BOARD_STATE::STATE_PLAYER_STARTING:
    break;
  case BOARD_STATE::STATE_WAITING_FIRST_DART:
    break;
  case BOARD_STATE::STATE_WAITING_SECOND_DART:
    break;
  case BOARD_STATE::STATE_WAITING_THIRD_DART:
    break;
  case BOARD_STATE::STATE_WAITING_PLAYER_VALID:
    break;
  case BOARD_STATE::STATE_GAME_RESULT:
    break;
  }
}

void pressButtonBottom()
{
  switch (myDartGame.getBoardState())
  {
  case BOARD_STATE::STATE_BOOT:
    break;
  case BOARD_STATE::STATE_BASE_INFO:
    gotoChoosePlayersNumber();
    sendGotoScreen();
    break;
  case BOARD_STATE::STATE_NUMBER_PLAYERS:
    if (myDartGame.getNumberOfPlayers() == 0)
      return;
    myDartGame.setBoardState(BOARD_STATE::STATE_CHOOSE_GAME);
    sendGotoScreen();
    break;
  case BOARD_STATE::STATE_CHOOSE_GAME:
    myDartGame.setBoardState(BOARD_STATE::STATE_BASE_INFO);
    sendGotoScreen();
    break;
  case BOARD_STATE::STATE_PLAYER_STARTING:
    myDisplay.stopMemo();
    myDisplay.drawScreenDartScores();
    myDartGame.doDartMissed();
    break;
  case BOARD_STATE::STATE_WAITING_FIRST_DART:
    myDisplay.stopMemo();
    myDisplay.drawScreenDartScores();
    myDartGame.doDartMissed();
    break;
  case BOARD_STATE::STATE_WAITING_SECOND_DART:
    myDartGame.doDartMissed();
    break;
  case BOARD_STATE::STATE_WAITING_THIRD_DART:
    myDartGame.doDartMissed();
    break;
  case BOARD_STATE::STATE_WAITING_PLAYER_VALID:
    myDisplay.stopMemo();
    myDartGame.doNextPlayer();
    //myDartGame.setBoardState(myDartGame.getNextBoardState());
    break;
  case BOARD_STATE::STATE_GAME_RESULT:
    myDartGame.setBoardState(BOARD_STATE::STATE_BASE_INFO);
    sendGotoScreen();
    break;
  }
}

void oneDart(String dart)
{
  if (dart.length() < 1)
    return;

  //Serial.println(dart);
  myWeb.sendToAll(dart);
  if (dart[0] == 'X')
    return;

  switch (myDartGame.getBoardState())
  {
  case BOARD_STATE::STATE_PLAYER_STARTING:
    myDisplay.stopMemo();
    myDisplay.drawScreenDartScores();
    myDartGame.doDart(dart);
    break;
  case BOARD_STATE::STATE_WAITING_FIRST_DART:
    myDisplay.stopMemo();
    myDisplay.drawScreenDartScores();
    myDartGame.doDart(dart);
    break;
  case BOARD_STATE::STATE_WAITING_SECOND_DART:
    myDartGame.doDart(dart);
    break;
  case BOARD_STATE::STATE_WAITING_THIRD_DART:
    myDartGame.doDart(dart);
    break;
  case BOARD_STATE::STATE_BOOT:
  case BOARD_STATE::STATE_BASE_INFO:
  case BOARD_STATE::STATE_NUMBER_PLAYERS:
  case BOARD_STATE::STATE_CHOOSE_GAME:
  case BOARD_STATE::STATE_WAITING_PLAYER_VALID:
  case BOARD_STATE::STATE_GAME_RESULT:
    break;
  }
}

// ************************************************************************************

// we received a command from a browser
void doCommandFromWeb(String command)
{
  if (command.length() < 1)
    return;
  MY_DEBUG_LN(command);

  DeserializationError error = deserializeJson(jsonReceived, command);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  if (jsonReceived["name"] == "Players")
  {
    myDartGame.setNumberOfPlayers(jsonReceived["players"].size());
    for (uint8_t i = 0; i < jsonReceived["players"].size(); i++)
      myDartGame.setPlayerName(i, jsonReceived["players"][i]);
    if (jsonReceived["valid"])
    {
      myDartGame.setBoardState(BOARD_STATE::STATE_CHOOSE_GAME);
    }
    else
    {
      if (myDartGame.getBoardState() != BOARD_STATE::STATE_NUMBER_PLAYERS)
        gotoChoosePlayersNumber();
      else
        changeToScreen();
    }
  }
}

// ************************************************************************************

void setup()
{
  delay(1200);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  MY_DEBUG_LN("*** Dart32 ***");
  myDisplay.setupLcd();
  delay(100);
  myDisplay.drawScreenBaseInfo("I2C buttons...");
  setupDart(oneDart, pressButtonTop, pressButtonMiddle, pressButtonBottom);
  delay(100);
  myDisplay.refreshScreenBaseInfo("SPIFFS...");
  setupFS();
  delay(100);
  myDisplay.refreshScreenBaseInfo("WiFi...");
  myWifi.setupWifi();
  delay(100);
  myDisplay.refreshScreenBaseInfo("Web...");
  myWeb.begin(doCommandFromWeb);
  myWeb.setupWeb(SPIFFS);
  delay(100);
  myDisplay.refreshScreenBaseInfo("Game...");
  myDartGame.begin(changeToScreen);
}

// ************************************************************************************

void loop()
{
  myWeb.browserWebSocket.cleanupClients();
  if (Serial.available())
  {
    String message = Serial.readString();
    //browserWebSocket.textAll(message);
    //MY_DEBUG_LN(delta);
  }
  readDart();
  myDartGame.loop();
  myDisplay.loopMemo();
}