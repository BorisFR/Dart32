const PAGES = {
    CHOOSE: 0,
    OPTIONS: 1,
    PLAYERS: 2,
    GAME: 3
}
const GAME = {
    UP: 10,
    CRICKET: 20,
    G301_20: 0,
    G301_30: 1
};

let currentPage = null;
let chooseGame = null;
let idGameOptions = null;
let gameOptions = null;
let cancelOptions = null;
let validOptions = null;
let idOptionsIcon = null;
let idPlayers = null;
let cancelPlayers = null;
let validPlayers = null;
let idPlayersIcon = null;
let idGamePlayers = null;
let selectedPlayers = null;
let availablePlayers = null;
let theWidth = null;
let theHeight = null;
let theCanvas = null;
let context = null;
let radius = null;
let TwoPI = 2 * Math.PI;
let targetsNumber = [1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, 20];

let selected = ["Boris"];
let available = ["Chno", "Djeff", "Manu", "Puce En Or", "Serge", "Ubik"];

let colorBack = "#000000";
let colorLine = "#FFFF00";
let color1 = "#AA0000";
let color2 = "#BBBBBB";
let color3 = "#00AA00";
let color4 = "#AAAA00";
let color5 = "#AA00AA";
let color6 = "#00BBBB";
let color7 = "#BBBBBB";
let color8 = "#0088BB";
let color9 = "#AA0000";
let color10 = "#44FF44";
let colorNumbers = "#EEEEEE";
let colorBackground = "#0000BB";
let colorOuterLine = "#FF0000";

let linePos = [1, 2.2, 9.9, 11.3, 16.2, 17.4, 20];

let timerWS = null;

let theDart = null;
let dartOpacity = null;
let dartTimer = null;

var keymap = ['0', '1', '2', '4', '5', '6', '7', '8', '9', '*',
    'a', 'c', 'd', 'e', 'f', 'g', 'h', 'i', '?', '!',
    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', '&',
    't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', '@', '#',
    'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', '}', '(',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', ')', '=',
    'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '+', '-', ']'];
var keyMult = [3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3];
var keyValue = [7, 19, 3, 17, 2, 15, 16, 8, 11, 14,
    7, 19, 3, 17, 2, 15, 16, 8, 11, 14,
    7, 19, 3, 17, 2, 15, 16, 8, 11, 14,
    0, 0, 0, 0, 0, 0, 0, 0, 25, 30,
    1, 18, 4, 13, 6, 10, 20, 5, 12, 9,
    1, 18, 4, 13, 6, 10, 20, 5, 12, 9,
    1, 18, 4, 13, 6, 10, 20, 5, 12, 9];

let game = null;
let currentTurn = null;
let maxTurn = null;
let currentDart = null;

let playersScore = [
    [[1, 19, 1, 0, 1, 6], [2, 18, 1, 0, 1, 0], [1, 17, 1, 1, 1, 6], [1, 25, 1, 20, 1, 1], [1, 17, 1, 1, 1, 6], [1, 17, 1, 1, 1, 6], [2, 16, 1, 0, 1, 0], [1, 15, 1, 2, 1, 6], [2, 14, 1, 0, 1, 0]
        , [1, 9, 1, 0, 1, 6], [2, 8, 1, 0, 1, 0], [1, 7, 1, 0, 1, 6], [2, 6, 1, 5, 1, 0], [1, 5, 1, 0, 1, 6], [2, 4, 1, 3, 1, 0],
    [1, 3, 1, 0, 1, 6], [2, 11, 1, 0, 1, 0], [1, 9, 1, 0, 1, 1], [2, 11, 1, 0, 1, 0], [1, 20, 1, 10]]
];
let playersName = ["Boris", "Chno", "Djeff", "Manu"];
let currentPlayer = null;

window.onload = initAll();

function initGame(gameType) {
    game = gameType;
    initPlayers();
    switch (gameType) {
        case GAME.UP:
            currentTurn = 0;
            maxTurn = 8;
            break;
        case GAME.G301_20:
            currentTurn = 0;
            maxTurn = 20;
            break;
        case GAME.G301_30:
            currentTurn = 0;
            maxTurn = 30;
            break;
        case GAME.CRICKET:
            currentTurn = 0;
            maxTurn = 0;
            break;
    }
    currentDart = null;
}

function startGame() {
    currentTurn = 1;
    currentPlayer = 0;
    currentDart = 1;
    playersScore[currentPlayer].push([]);
}

function initPlayers() {
    playersScore = [];
    playersName = [];
}

function addPlayer(name) {
    playersScore.push([]);
    playersName.push(name);
}

function nextPlayer() {
    currentPlayer = currentPlayer + 1;
    if (currentPlayer >= playersName.length) {
        currentPlayer = 0;
        currentTurn = currentTurn + 1;
    }
    currentDart = 1;
    playersScore[currentPlayer].push([]);
    //drawScreen();
}

function doAction(command) {
    console.log(command);
    var c = command[0];
    var n = command[1];
    if (c == 'X') {
        dartTimer = setTimeout(hideDart, 1500);
        return;
    }
    if (c == 'S') {
        return;
    }
    var pos = keymap.indexOf(n);
    var mult = keyMult[pos];
    var target = keyValue[pos];
    //dartTo(mult, target);

    /*var code = "";
    switch (mult) {
        case 1: code = "S"; break;
        case 2: code = "D"; break;
        case 3: code = "T"; break;
    }
    code = code + target;*/
    if (currentPage != PAGES.GAME) return;
    playersScore[currentPlayer][currentTurn - 1].push(mult, target);
    /*switch (currentDart) {
        case 1: playersScore[currentPlayer].push([mult, target]); break;
        default: playersScore[currentPlayer][currentTurn - 1].push(mult, target); break;
    }*/
    currentDart = currentDart + 1;
    if (currentDart > 3) {
        nextPlayer();
    }
    drawScreen();
}

function connectWS() {
    clearInterval(timerWS);
    var protocol = window.location.protocol === 'http:' ? 'ws://' : 'wss://';
    var address = protocol + window.location.host + '/dart32';
    address = 'ws://10.0.0.9/dart32'; // TODO: remove, debug
    try {
        console.log("WS: trying to connect");
        socket = new WebSocket(address);
        socket.onopen = function () { clearInterval(timerWS); console.log("WS onOpen: clearInterval"); }
        socket.onmessage = function (msg) { doAction(msg.data); };
        socket.onerror = function () { socket.close(); console.log("WS onError: socket.close"); }
        socket.onclose = function () { clearInterval(timerWS); console.log("WS onClose: clearInterval & start new in 10s"); timerWS = setInterval(() => { connectWS(); }, 10000); }
    } catch (error) {
        console.log("WS catch: nothing (" + error + ")");
    }
}

function colorED110() {
    colorBack = "#000000";
    colorLine = "#000000";
    color1 = "#AA0000";
    color2 = "#BBBBBB";
    color3 = "#BBBBBB";
    color4 = "#0088BB";
    color5 = "#AA0000";
    color6 = "#BBBBBB";
    color7 = "#BBBBBB";
    color8 = "#0088BB";
    color9 = "#0088BB";
    color10 = "#AA0000";
    colorNumbers = "#FFFFFF";
    colorBackground = "#222222";
    colorOuterLine = "#222222";
}

function colorWinmax() {
    colorBack = "#222222";
    colorLine = "#FFFFFF";
    color1 = "#00A000";
    color2 = "#FF0000";
    color3 = "#E7E4C7";
    color4 = "#000000";
    color5 = "#00A000";
    color6 = "#FF0000";
    color7 = "#E7E4C7";
    color8 = "#000000";
    color9 = "#00A000";
    color10 = "#FF0000";
    colorNumbers = "#FFFFFF";
    colorBackground = "#000000";
    colorOuterLine = "#000000";
}

function colorBlueRed() {
    colorBack = "#000000";
    colorLine = "#3C4250";
    color1 = "#3C74D5";
    color2 = "#CE404E";
    color3 = "#DCCFC9";
    color4 = "#3C4250";
    color5 = "#3C74D5";
    color6 = "#CE404E";
    color7 = "#DCCFC9";
    color8 = "#3C4250";
    color9 = "#3C74D5";
    color10 = "#CE404E";
    colorNumbers = "#CAC9CF";
    colorBackground = "#3C4250";
    colorOuterLine = "#3C4250";
}

function hideDart() {
    dartOpacity = dartOpacity - 0.1;
    theDart.style.opacity = dartOpacity;
    if (dartOpacity < 0.1) return;
    dartTimer = setTimeout(hideDart, 30);
}

function dartTo(mult, points) {
    if (points == 0) return;
    clearTimeout(dartTimer);
    dartOpacity = 1.0;
    theDart.style.opacity = dartOpacity;
    if (points == 30) {
        theDart.style.right = radius - theDart.clientWidth + "px";
        theDart.style.top = radius - theDart.clientHeight + "px";
        return;
    }
    if (points == 25) {
        i = linePos[1] * radius / linePos[6] - (linePos[1] * radius / linePos[6] - linePos[0] * radius / linePos[6]) / 2 - 4;
        theDart.style.right = radius - theDart.clientWidth + "px";
        theDart.style.top = radius - theDart.clientHeight - i + "px";
        return;
    }
    var j = 0;
    while (targetsNumber[j] != points) {
        j = j + 1;
    }
    var ang = Math.PI / 2 - (j + 1) * Math.PI / 10;
    //console.log("Angle= " + ang);
    switch (mult) {
        case 1:
            i = linePos[4] * radius / linePos[6] - (linePos[4] * radius / linePos[6] - linePos[3] * radius / linePos[6]) / 2;
            break;
        case 2:
            i = linePos[5] * radius / linePos[6] - (linePos[5] * radius / linePos[6] - linePos[4] * radius / linePos[6]) / 2;
            break;
        case 3:
            i = linePos[3] * radius / linePos[6] - (linePos[3] * radius / linePos[6] - linePos[2] * radius / linePos[6]) / 2;
            break;
    }
    var x = i * Math.cos(ang);
    var y = i * Math.sin(ang);
    //console.log("Dist: " + i);
    //console.log("X: " + x);
    //console.log("Y: " + y);
    theDart.style.right = radius - theDart.clientWidth - x + "px";
    theDart.style.top = radius - theDart.clientHeight - y + "px";
    //theDart.style.right = radius - theDart.clientWidth + "px";
    //theDart.style.top = radius - theDart.clientHeight - i + "px";
}

function drawNumber(x, y, text, size, color = "#FFFFFF", halign = "left", valign = "top") {
    context.fillStyle = color;
    context.font = size + "vw Segment7";
    context.textBaseline = valign;
    context.textAlign = halign;
    context.fillText(text, x, y);
}

function drawText(x, y, text, size, color = "#FFFFFF", halign = "left", valign = "top") {
    context.fillStyle = color;
    context.font = size + "vw Roboto";
    context.textBaseline = valign;
    context.textAlign = halign;
    context.fillText(text, x, y);
}

function drawHand(context, pos, length, width) {
    context.beginPath();
    context.lineWidth = width;
    context.lineCap = "round";
    context.moveTo(0, 0);
    context.rotate(pos);
    context.lineTo(0, -length);
    context.stroke();
    context.rotate(-pos);
}

function drawTargets() {
    var min = Math.min(theWidth, theHeight);
    radius = min / 2.0;
    theDart.style.width = radius / 5;
    //console.log("DH= " + theDart.clientHeight);

    let centerX = null;
    let centerY = null;
    if (theWidth > theHeight) {
        //centerX = radius; // collé à gauche
        centerX = theWidth - radius; // collé à droite
        centerY = theHeight / 2;
    } else {
        centerX = theWidth / 2;
        //centerY = radius; // collé en haut
        centerY = theHeight - radius; // collé en bas
    }
    context.translate(centerX, centerY);
    // background circle
    context.beginPath();
    context.fillStyle = colorBackground;
    context.arc(0, 0, radius, 0, TwoPI);
    context.fill();
    // outer line number
    context.strokeStyle = colorOuterLine;
    context.lineWidth = "2";
    context.arc(0, 0, radius, 0, TwoPI);
    context.stroke();

    var ang;
    var num;
    var currentRadius;
    var delta = 2 * Math.PI / 20;
    //console.log("Delta= " + delta);
    context.moveTo(0, 0);
    // external color
    ang = Math.PI / 20;
    currentRadius = linePos[5] * radius / linePos[6];
    //console.log("R5= " + currentRadius);
    for (num = 1; num < 21; num++) {
        context.beginPath();
        if ((num % 2) == 0) {
            context.fillStyle = color1;
        } else {
            context.fillStyle = color2;
        }
        context.lineTo(currentRadius * Math.cos(ang), currentRadius * Math.sin(ang));
        ang = ang + delta;
        context.lineTo(currentRadius * Math.cos(ang), currentRadius * Math.sin(ang));
        context.lineTo(0, 0);
        context.closePath();
        context.fill();
    }

    // inner line number
    context.beginPath();
    context.strokeStyle = colorLine;
    context.lineWidth = "4";
    context.arc(0, 0, currentRadius - 2, 0, TwoPI);
    context.stroke();

    // inner color
    ang = Math.PI / 20;
    currentRadius = linePos[4] * radius / linePos[6];
    //console.log("R4= " + currentRadius);
    for (num = 1; num < 21; num++) {
        context.beginPath();
        if ((num % 2) == 0) {
            context.fillStyle = color3;
        } else {
            context.fillStyle = color4;
        }
        context.lineTo(currentRadius * Math.cos(ang), currentRadius * Math.sin(ang));
        ang = ang + delta;
        context.lineTo(currentRadius * Math.cos(ang), currentRadius * Math.sin(ang));
        context.lineTo(0, 0);
        context.closePath();
        context.fill();
    }

    // inner line number
    context.beginPath();
    context.strokeStyle = colorLine;
    context.lineWidth = "4";
    context.arc(0, 0, currentRadius - 2, 0, TwoPI);
    context.stroke();

    // inner color
    ang = Math.PI / 20;
    currentRadius = linePos[3] * radius / linePos[6];
    //console.log("R3= " + currentRadius);
    for (num = 1; num < 21; num++) {
        context.beginPath();
        if ((num % 2) == 0) {
            context.fillStyle = color5;
        } else {
            context.fillStyle = color6;
        }
        context.lineTo(currentRadius * Math.cos(ang), currentRadius * Math.sin(ang));
        ang = ang + delta;
        context.lineTo(currentRadius * Math.cos(ang), currentRadius * Math.sin(ang));
        context.lineTo(0, 0);
        context.closePath();
        context.fill();
    }
    // inner line number
    context.beginPath();
    context.strokeStyle = colorLine;
    context.lineWidth = "4";
    context.arc(0, 0, currentRadius - 2, 0, TwoPI);
    context.stroke();

    // inner color
    ang = Math.PI / 20;
    currentRadius = linePos[2] * radius / linePos[6];
    //console.log("R2= " + currentRadius);
    for (num = 1; num < 21; num++) {
        context.beginPath();
        if ((num % 2) == 0) {
            context.fillStyle = color7;
        } else {
            context.fillStyle = color8;
        }
        context.lineTo(currentRadius * Math.cos(ang), currentRadius * Math.sin(ang));
        ang = ang + delta;
        context.lineTo(currentRadius * Math.cos(ang), currentRadius * Math.sin(ang));
        context.lineTo(0, 0);
        context.closePath();
        context.fill();
    }
    // inner line number
    context.beginPath();
    context.strokeStyle = colorLine;
    context.lineWidth = "4";
    context.arc(0, 0, currentRadius - 2, 0, TwoPI);
    context.stroke();



    context.fillStyle = colorNumbers;
    context.font = radius / 10 + "px Roboto";
    context.textBaseline = "middle";
    context.textAlign = "center";
    currentRadius = radius * 0.935;
    for (num = 1; num < 21; num++) {
        ang = num * Math.PI / 10;
        context.rotate(ang);
        context.translate(0, -currentRadius);
        context.rotate(-ang);
        context.fillText(targetsNumber[num - 1].toString(), 0, 0);
        context.rotate(ang);
        context.translate(0, currentRadius);
        context.rotate(-ang);
        var part1 = ang - (delta / 2);
        drawHand(context, part1, linePos[5] * radius / linePos[6] - 3, radius * 0.01);
    }


    // middle circle
    currentRadius = linePos[1] * radius / linePos[6];
    //console.log("R1= " + currentRadius);
    context.beginPath();
    context.fillStyle = color9;
    context.arc(0, 0, currentRadius, 0, TwoPI);
    context.fill();
    // inner line
    context.beginPath();
    context.strokeStyle = colorLine;
    context.lineWidth = "4";
    context.arc(0, 0, currentRadius - 2, 0, TwoPI);
    context.stroke();

    // center circle
    currentRadius = linePos[0] * radius / linePos[6];
    //console.log("R0= " + currentRadius);
    context.beginPath();
    context.fillStyle = color10;
    context.arc(0, 0, currentRadius, 0, TwoPI);
    context.fill();
    // inner line
    context.beginPath();
    context.strokeStyle = colorLine;
    context.lineWidth = "4";
    context.arc(0, 0, currentRadius - 2, 0, TwoPI);
    context.stroke();

    context.translate(-centerX, -centerY);
}

function pad2(number) {
    if (number > 9) return number.toString();
    if (number == 0) return "--";
    return " " + number.toString();
}

function pad3(number) {
    if (number > 99) return number.toString();
    if (number > 9) return " " + number.toString();
    return "  " + number.toString();
}

function drawPlay() {
    drawText((theWidth - 2 * radius) / 2, 6, "* 301 - 20 *", 5, "#FFFFFF", "center");
    var s = " Player";
    if (playersName.length > 1) s = " Players";
    drawText(0, theHeight, playersName.length + s, 1, "#FFFFFF", "left", "bottom");

    if (currentPlayer == null) return;
    var y = 100 + 40 * playersScore[currentPlayer].length;
    var score = 301;
    var tempsScore = score;
    //for (var i = 0; i < playersScore.length; i++) {
    var i = currentPlayer;
    {
        for (var j = 0; j < playersScore[i].length; j++) {
            drawNumber(50, y, (j + 1).toString(), 3, "#AAFFFF", "right");
            tempsScore = 0;
            if (playersScore[i][j].length > 0) {
                var typeDart = "S"; var mult = 1;
                if (playersScore[i][j][0] == 2) {
                    typeDart = "D"; mult = 2;
                }
                if (playersScore[i][j][0] == 3) {
                    typeDart = "T"; mult = 3;
                }
                tempsScore = mult * playersScore[i][j][1];
                drawText(80, y, typeDart, 3, "#AAFFFF", "left");
                drawNumber(110, y, pad2(playersScore[i][j][1]), 3, "#AAFFFF", "left");
                if (playersScore[i][j].length > 2) {
                    typeDart = "S"; mult = 1;
                    if (playersScore[i][j][2] == 2) {
                        typeDart = "D"; mult = 2;
                    }
                    if (playersScore[i][j][2] == 3) {
                        typeDart = "T"; mult = 3;
                    }
                    tempsScore = tempsScore + mult * playersScore[i][j][3];
                    drawText(180, y, typeDart, 3, "#AAFFFF", "left");
                    drawNumber(210, y, pad2(playersScore[i][j][3]), 3, "#AAFFFF", "left");
                    if (playersScore[i][j].length > 4) {
                        typeDart = "S"; mult = 1;
                        if (playersScore[i][j][4] == 2) {
                            typeDart = "D"; mult = 2;
                        }
                        if (playersScore[i][j][4] == 3) {
                            typeDart = "T"; mult = 3;
                        }
                        tempsScore = tempsScore + mult * playersScore[i][j][5];
                        drawText(280, y, typeDart, 3, "#AAFFFF", "left");
                        drawNumber(310, y, pad2(playersScore[i][j][5]), 3, "#AAFFFF", "left");
                    }
                }
            }
            if ((score - tempsScore) < 0) {
                // not good
                drawNumber(380, y, "---", 3, "#AAFFFF", "left");
            } else {
                score = score - tempsScore;
                drawNumber(380, y, pad3(score), 3, "#AAFFFF", "left");
                if ((score - tempsScore) == 0) {
                    // winner
                }
            }
            y = y - 40;
        } // for j
    } // for i
    if (currentPlayer != null) {
        drawText((theWidth - 2 * radius) / 2 + 30, 70, playersName[currentPlayer] + ": ", 4.5, "#AAFFFF", "right");
        drawNumber((theWidth - 2 * radius) / 2 + 30, 70, score.toString(), 4.5, "#AAFFFF", "left");
    }
}

function drawScreen() {
    context.fillStyle = colorBack;
    context.fillRect(0, 0, theWidth, theHeight);
    drawTargets();
    drawPlay();
    //dartTo(1, 5);
}

function getSize() {
    theWidth = window.innerWidth;
    theHeight = window.innerHeight;
    theCanvas.width = theWidth;
    theCanvas.height = theHeight;
    drawScreen();
}

function initAll() {
    theDart = document.getElementById("theDart");
    theDart.style.opacity = 0;
    chooseGame = document.getElementById("chooseGame");
    idGameOptions = document.getElementById("idGameOptions");
    gameOptions = document.getElementById("gameOptions");
    cancelOptions = document.getElementById("cancelOptions");
    validOptions = document.getElementById("validOptions");
    idOptionsIcon = document.getElementById("idOptionsIcon");
    idPlayers = document.getElementById("idPlayers");
    cancelPlayers = document.getElementById("cancelPlayers");
    validPlayers = document.getElementById("validPlayers");
    idPlayersIcon = document.getElementById("idPlayersIcon");
    idGamePlayers = document.getElementById("idGamePlayers");
    selectedPlayers = document.getElementById("selectedPlayers");
    availablePlayers = document.getElementById("availablePlayers");
    theCanvas = document.getElementById("theCanvas");
    context = theCanvas.getContext("2d");
    colorED110();
    //colorWinmax();
    //colorBlueRed();
    getSize();
    window.onresize = getSize;
    if ('WebSocket' in window) {
        timerWS = setInterval(() => { connectWS(); }, 1000);
    } else {
        console.error('This Browser does NOT supported WebSocket :(');
    }
    //setInterval(() => { getSize(); }, 1000);
    setPage(PAGES.PLAYERS);
    document.getElementById("idGameUp").onclick = chooseGameUp;
    document.getElementById("idGame01").onclick = chooseGame01;
    document.getElementById("idGameCricket").onclick = chooseGameCricket;
    cancelOptions.onclick = cancel;
    cancelPlayers.onclick = cancel;
    validOptions.onclick = valid;
    validPlayers.onclick = valid;
}

function sendPlayers(isValid = false) {
    var obj = new Object();
    obj.name = "Players";
    obj.players = [];
    for (var i = 0; i < selected.length; i++) {
        obj.players.push(selected[i]);
    }
    obj.valid = isValid;
    socket.send(JSON.stringify(obj));
}

function addScreenPlayer(id) {
    selected.push(available[id]);
    available.splice(id, 1);
    populateSelected();
    populateAvailable();
    sendPlayers();
}

function removeScreenPlayer(id) {
    available.push(selected[id]);
    selected.splice(id, 1);
    populateSelected();
    populateAvailable();
    sendPlayers();
}

function populateSelected() {
    selected.sort();
    selectedPlayers.innerHTML = "";
    var s = '';
    for (var i = 0; i < selected.length; i++) {
        s = s + '<span class="removePlayer" onClick="removeScreenPlayer(' + i + '); ">' + selected[i] + '</span> ';
    }
    selectedPlayers.innerHTML = s;
}

function populateAvailable() {
    available.sort();
    availablePlayers.innerHTML = "";
    var s = '';
    for (var i = 0; i < available.length; i++) {
        s = s + '<span class="addPlayer" onClick="addScreenPlayer(' + i + '); ">' + available[i] + '</span> ';
    }
    availablePlayers.innerHTML = s;
}

function valid() {
    if (currentPage == PAGES.PLAYERS) {
        if (selected.length == 0) return;
        sendPlayers(true);
        setPage(PAGES.CHOOSE);
        return;
    }
    if (currentPage == PAGES.OPTIONS) {
        setPage(PAGES.GAME);
        return;
    }
}

function cancel() {
    switch (currentPage) {
        case PAGES.CHOOSE:
            setPage(PAGES.PLAYERS);
            break;
        case PAGES.OPTIONS:
            setPage(PAGES.CHOOSE);
            break;
        default:
            break;
    }
}

function chooseGameUp() {
    initGame(GAME.UP);
    idGameOptions.innerHTML = "COUNT-UP : choix des options";
    idGamePlayers.innerHTML = "COUNT-UP : choix des joueurs";
    idOptionsIcon.src = "dart_game_up.png";
    idPlayersIcon.src = "dart_game_up.png";
    setPage(PAGES.GAME);
}

function chooseGame01() {
    initGame(GAME.G301_20);
    idGameOptions.innerHTML = "ZERO ONE : choix des options";
    idGamePlayers.innerHTML = "ZERO ONE : choix des joueurs";
    idOptionsIcon.src = "dart_game_01.png";
    idPlayersIcon.src = "dart_game_01.png";
    setPage(PAGES.OPTIONS);
}

function chooseGameCricket() {
    initGame(GAME.CRICKET);
    idGameOptions.innerHTML = "CRICKET : choix des options";
    idGamePlayers.innerHTML = "CRICKET : choix des joueurs";
    idOptionsIcon.src = "dart_game_cricket.png";
    idPlayersIcon.src = "dart_game_cricket.png";
    setPage(PAGES.OPTIONS);
}

// remove an element from the screen
function hideElement(element) {
    element.style.visibility = "hidden";
    element.style.display = "none";
}

// add an element to the screen
function showElement(element) {
    element.style.visibility = "visible";
    element.style.display = "block";
}

function setPage(newPage) {
    currentPage = newPage;
    switch (newPage) {
        case PAGES.CHOOSE:
            showElement(chooseGame);
            hideElement(gameOptions);
            hideElement(idPlayers);
            hideElement(theCanvas);
            break;
        case PAGES.OPTIONS:
            hideElement(chooseGame);
            showElement(gameOptions);
            hideElement(idPlayers);
            hideElement(theCanvas);
            break;
        case PAGES.PLAYERS:
            hideElement(chooseGame);
            hideElement(gameOptions);
            showElement(idPlayers);
            hideElement(theCanvas);
            populateSelected();
            populateAvailable();
            break;
        case PAGES.GAME:
            hideElement(chooseGame);
            hideElement(gameOptions);
            hideElement(idPlayers);
            showElement(theCanvas);
            for (var i = 0; i < selected.length; i++)
                addPlayer(selected[i]);
            startGame();
            drawScreen();
            //document.fonts.ready.then(function () { drawScreen(); });
            break;
    }
}