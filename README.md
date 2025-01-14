# ChessPlusPLus

## Description

ChessPlusPlus is a Chess game that was coded entirely from scratch in C++.

It includes the standard rules of chess and also the ability to combine the movepools of pieces.

It can be played in 2-player mode, or versus the built-in engine that plays at about 1500 elo.

## Instructions
<ol>
<li>Upon launching, you will be prompted on if you would like to enable fairy chess.</li><li>
Any response other than “Y” will leave fairy chess disabled.</li><li>
You will then be asked whether you would like to play against the engine.</li><li>
Any response other than “Y” will leave the game in 2 player mode (No engine).</li><li>
If fairy chess is enabled, the program will then prompt you on which pieces* to combine for fairy chess. *You can only tell the program to combine B,N,R,Q,K, any other piece will be treated as not combined. If the piece added does not add new moves (Adding bishop to bishop), it will be treated as not combined.</li><li>
If the engine is enabled, the program will ask you whether you would like to play with the Black or White pieces. Any response other than “B” will result in you playing as White.</li><li>
Once the game has begun, each time it is a player’s turn to move they will be prompted “Enter move”. This move is to be entered in correct Algebraic notation, though sometimes minor errors will still be accepted.</li><li>
Pieces on the board are represented as follows:
Uppercase letters are white pieces, lowercase letters are black pieces
N means knight, K means king, Q means queen, R means rook, B means bishop, P means pawn
Beside board, if fairy chess is enabled, the program outputs a list of all of the fairy combinations. Combinations that do not add new moves are not output (Ex. Queen + Bishop).</li>
</ol>

Commands can be entered in place of moves:
<ul>
<li>/resign (self explanatory)</li><li>
/undo (reverts the board back a number of moves as specified)</li><li>
/on (turns on the engine)</li><li>
/off (turns off the engine)</li><li>
/onemove (the engine generates one move)</li><li>
/setdepth (sets the maximum depth of the engine)</li><li>
/setcutoff (sets the cutoff which the engine uses internally to
           decide whether or not to continue calculating a line)</li><li>
/printall (print the position stored inside of all pieces for debugging)</li>
</ul>

## What is Meant by Fairy Chess?
In this game, fairy chess pieces are combinations of 2 preexisting chess pieces. When fairy chess is Enabled, Rooks, Bishop, Knights, Queens will become fairy pieces.
