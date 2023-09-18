# Console online/offline tic-tac-toe client.

### Features

- Choose any grid size between 3x3 and 26x26.
- Choose the length of a winning line.
- Choose to play X or O.
- 3 game modes:
  - singleplayer with 5 difficulty levels,
  - local multiplayer (play on the same pc),
  - online multiplayer.
- Easy play again.
- Score tracking.

### Build notes

You need to add these header-only libraries to your include path:

- https://github.com/yhirose/cpp-httplib.
- https://github.com/nlohmann/json.

Also install OpenSSL (CMake should find and link automatically).

### Server

You can find code for the server here:
https://github.com/valentine-ch/Tic-Tac-Toe-Server-FastAPI