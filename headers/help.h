#ifndef HELP_H
#define HELP_H

#include <string_view>

constexpr std::string_view helpText{R"(
GAME RULES (IN CASE YOU REALLY DON'T KNOW)
- The game is played on a grid of predetermined size, typically square-shaped.
- Two players take turns marking empty squares with their designated symbols (X and O).
- X always starts first.
- The objective is to get a specified number of your symbols in a row, column, or diagonal, known as the winning line.
- The traditional version of the game is played on a 3x3 grid with a winning line of 3, but these numbers may be different.
- The game starts with an empty grid and continues until one player achieves the winning condition or all squares are filled.
- If all squares are filled without a winner, the game ends in a draw or a tie.
- A winning move can be made in any direction: across rows, down columns, or along diagonals.
- Once a winning condition is met, the game ends immediately, and that player is declared the winner.

MENU NAVIGATION
- The first thing you see here is the main menu which allows you to choose the game mode, open help or exit.
- Use character codes to access specific features:
    s - start a single player game.
    l - start a local multiplayer game (both player play on the same pc).
    o - enter online menu.
    h - show help menu (this is how you got here ¯\_(ツ)_/¯).
    e - exit the game.
- If you enter a character that is not s, l, o, h or e the following message will be printed: "Invalid input, try again".
- Then you will be able to reenter you input correctly.
- Each game mode has its specific submenus that work in a similar way.
- Some menus will allow you to go back to previous one by entering e.
- In most cases input is case-insensitive (except names and passwords).

GRID PROPERTIES & CELL INPUT
- Each new game has its own grid properties that include its size and winning line.
- You can manually choose those parameters but the way depends on the game mode.
- In this implementation the grid is always square because author thinks it should be always square.
- The minimal size of a grid is 3x3 because the game on a smaller grids doesn't make sence.
- The maximum size of a grid is 26x26 because you run out of letters (and console space as well probably).
- The length of a winning line can be anything between 3 and a chosen grid sise.
- If the grid size is 3, the winnig line will be automatically set to 3.
- Grid is represented in the console like this:
    1   2   3 
  +---+---+---+
a |   |   |   | a
  +---+---+---+
b |   |   |   | b
  +---+---+---+
c |   |   |   | c
  +---+---+---+
    1   2   3 
- Each cell in a grid has a unique coordinate (for example "a2", "b1", "c3" etc).
- When it's your turn, you will be asked to enter a cell.
- To enter a cell use its coordinate in the grid (letter first, no quotes).
- Exapmles of cells entered correctly: b2, C1.
- Exapmles of cells entered incorrectly: 1a, "b3".
    
SCORE & PLAY AGAIN
- Each time a game ends, a score will printed to the console and you will be asked if you want to play again.
- If you decide to play again a new game will be started with the same parameters.
- The score represents how hamy times each side has won and how namy draws there were.
- The score keeps track only the result of the current sequence of games as long as you choose to play again.
- It is not saved any where and when you press 'n', it is lost forever.

SINGLE PLAYER MODE
- In this mode you can play against the computer.
- Before the game starts, it will ask you for the grid size and a winnig line length.
- Then it fill ask you to choose you token (X or O). Remember that X always starts first.
- Finally you have to choose a diffuculty level.
- The game currently has 5 difficulty levels for single player with level 1 being the easiest and level 5 being the hardest.
- After you entered all game parameters, the game will start.
- Computer will make its moves automatically when it's its turn.
- When the game is finished, the score will be printed and you will be able to choose if you want to play again.

LOCAL MULTIPLAYER MODE
- This mode allows two players to play on the same computer.
- Before the game starts, it will ask you for the grid size and a winnig line length.
- Then it will ask you to enter players' names.
- The order you enter these names DOES matter as the player whose name you enter first will play as X and move first.
- After you entered all game parameters, the game will start.
- Before each move the game will show the name of the player whose turn is it.
- When the game is finished, the score will be printed and you will be able to choose if you want to play again.

ONLINE MULTIPLAYER MODE
- This mode allows you to play with other players online.
- To play online you will need to enter server adress.
- Yes, you need to know server adress or host your own. The client will automatically verify the adress you entered.
- You need an account to play online. This is necessary for other players to see your username.
- To create an account you need only username and password.
- You will be asked to enter l to log in to existing account, c to create a new account or d to delete account from the server.
- You can also press e to exit if you don't want to create an account.
- When you're logged in, you can either start waiting for invitation of look for players who are currently waiting.
- If you choose to invite other player, you will need to enter the name of the user you want to invite.
- The list of waiting users will be printed to the console and updated automatically.
- When you choose the user you want to invite, you will need to choose the game parameters.
- After invitation is sent, wait until the the invited user accepts or declines invtation.
- You can also cancel your invitation by entering c to the console.
- If you choose to wait for invitation, the list of your incoming invitaions will be printed to the console and updated automatically.
- You will need to enter the number of invitation you want to respond to and then a to accept or d to decline.
- In either case, you can enter e to exit to the previous menu.
- When invitation is accepted the game will start automatically on both clients.
- When the game is finished, the score will be printed and you will be able to choose if you want to play again.
- Note that both players have to accept play again for new game to start.
- You can cancel play again by entering c to the console.

FAQ (NO ONE REALLY ASKED THEM BUT IT MAY BE HELPFUL)
Q: How can I get a server adress?
A: If someone sent you this client, you can ask them if they know one. You can also host your own server.

Q: If I want to host my own server, where can I get the source code?
A: Here: https://github.com/valentine-ch/Tic-Tac-Toe-Server-FastAPI

Q: Why server adress can't be validated?
A: Make sure you have Internet connection and enter the adress correctly.
If it still cannot be validated, the server may be temporarily or permanantly shut unavailable or not compatible with with version of client.

Q. Is my data encrypted?
A. Yes, all the data is sent using a secure https protocol. The way it is stored and processed on the server depends on server implementation.

Q: What information does this client send to the server?
A: This client does not collect any information on your pc. Only your usrname, password and your game-related input is sent.

Q: Can all servers be trusted?
A: No. The source code for the server is open and threoretically bad actors can create malicious servers.
Make sure you can trust the server or its owner.
)"};

#endif