#ifndef BOARD_H
#define BOARD_H
#include "bishop.h"
#include "boardHelper.h"
#include "fairy.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"
#include <vector>
using namespace std;

class Board {
public:
  Board(bool type = 0) {
    turn = false; // white's turn
    if (type == 0) {
      isFairy = false;
      square[0][0] = new Rook('a', 1, false);
      square[0][7] = new Rook('h', 1, false);
      square[7][0] = new Rook('a', 8, true);
      square[7][7] = new Rook('h', 8, true);
      square[0][1] = new Knight('b', 1, false);
      square[0][6] = new Knight('g', 1, false);
      square[7][1] = new Knight('b', 8, true);
      square[7][6] = new Knight('g', 8, true);
      square[0][2] = new Bishop('c', 1, false);
      square[0][5] = new Bishop('f', 1, false);
      square[7][2] = new Bishop('c', 8, true);
      square[7][5] = new Bishop('f', 8, true);
      square[0][4] = new King('e', 1, false);
      square[0][3] = new Queen('d', 1, false);
      square[7][4] = new King('e', 8, true);
      square[7][3] = new Queen('d', 8, true);
      for (int i = 0; i < 8; i++) {
        // sets empty squares to unspecialized pieces
        square[1][i] = new Pawn((char)(i + 97), 2, false);
        square[6][i] = new Pawn((char)(i + 97), 7, true);
      }
      for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
          square[i][j] = new Piece;
        }
      }
    }
    if (type == 1) {
      // pawns are pawns, they promote to one of the enabled fairy pieces
      // king are kings
      isFairy = true;
      for (int i = 0; i < 8; i++) {
        square[1][i] = new Pawn((char)(i + 97), 2, false);
        square[6][i] = new Pawn((char)(i + 97), 7, true);
      }
      for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
          square[i][j] = new Piece;
        }
      }
      square[0][4] = new King('e', 1, false);
      square[7][4] = new King('e', 8, true);
      char comb;
      cout << "Rook combined with: ";
      cin >> comb;
      if (isMajor(comb) == false) {
        comb = ' ';
      }
      if (comb == 'R') { // a rook plus a rook is just a rook, no reason to test
                         // moves for two
        comb = ' ';
      }
      rComb = comb;
      // if comb turns out to be garbage, the piece just acts like a normal
      // a bad comb is dealt with later
      square[0][0] = new Fairy('a', 1, false, 'R', comb);
      square[0][7] = new Fairy('h', 1, false, 'R', comb);
      square[7][0] = new Fairy('a', 8, true, 'R', comb);
      square[7][7] = new Fairy('h', 8, true, 'R', comb);
      cout << "Knight combined with: ";
      cin >> comb;
      if (isMajor(comb) == false) {
        comb = ' ';
      }
      if (comb == 'N') {
        comb = ' ';
      }
      nComb = comb;
      square[0][1] = new Fairy('b', 1, false, 'N', comb);
      square[0][6] = new Fairy('g', 1, false, 'N', comb);
      square[7][1] = new Fairy('b', 8, true, 'N', comb);
      square[7][6] = new Fairy('g', 8, true, 'N', comb);
      cout << "Bishop combined with: ";
      cin >> comb;
      if (isMajor(comb) == false) {
        comb = ' ';
      }
      if (comb == 'B') {
        comb = ' ';
      }
      bComb = comb;
      square[0][2] = new Fairy('c', 1, false, 'B', comb);
      square[0][5] = new Fairy('f', 1, false, 'B', comb);
      square[7][2] = new Fairy('c', 8, true, 'B', comb);
      square[7][5] = new Fairy('f', 8, true, 'B', comb);
      cout << "Queen combined with: ";
      cin >> comb;
      if (comb != 'N') {
        comb = ' ';
      }
      qComb = comb;
      square[0][3] = new Fairy('d', 1, false, 'Q', comb);
      square[7][3] = new Fairy('d', 8, true, 'Q', comb);
    }
  }

  ~Board() {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        delete square[i][j];
      }
    }
    // square is a static array, so cannot deleted, nothing else can be deleted
  }

  bool endGame() { // should the game end?
    if (isMate() == true || insuffMatl() == true)
      return true;
    else if (fiftyMove==0)
      return true;
    else if (threefold()==true)
      return true;
    else
      return false;
  }

  bool getTurn() { return turn; }

  void changeTurn() {
    turn = flip(turn);
  }

  bool insuffMatl() { // is it draw by insufficient material?
    int white3p = 0;  // number of 3 point pieces white has
    int black3p = 0;
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (square[row][col]->isEmpty() == false) {
          char type = square[row][col]->getType();
          if (type == 'R')
            return false; // any of these pieces allow you to checkmate
          else if (type == 'Q')
            return false;
          else if (type == 'P')
            return false;
          else if (type == 'B') {
            if (square[row][col]->getColour() == false) {
              if (white3p == 0)
                white3p += 2; // a bishop and a knight or a bishop can checkmate
              else
                return false;
            } else {
              if (black3p == 0)
                black3p += 2;
              else
                return false;
            }
          } else if (type == 'N') {
            if (square[row][col]->getColour() == false) {
              if (white3p <= 1)
                white3p++; // you need 3 knights or a knight and a bishop
              else
                return false;
            } else {
              if (black3p <= 1)
                black3p++;
              else
                return false;
            }
          }
        }
      }
    }
    return true; // nothing can checkmate, so the position is a draw
  }

  friend ostream &operator<<(ostream &os, Board &b) {
    char out[26][42]; // these are the dimensions that gave the squarest board
    for (int i = 0; i < 42; i++) {
      out[0][i] = '_', out[25][i] = '_'; // creates the border of the board
    }
    for (int i = 0; i < 25; i++) {
      out[i][0] = '|', out[i][41] = '|'; // creates the border of the board
    }
    if (b.turn == true) { // formatting is slightly different for black vs white
                          // in corners
      out[0][0] = '_';
      out[0][41] = '_';
      out[25][0] = '|';
      out[25][41] = '|';
    }
    for (int i = 0; i < 24; i++) {
      for (int j = 0; j < 40; j++) {
        // each square on the board is visually 5 wide and 3 high
        if ((i / 3 + j / 5) % 2 == 1) // create the checkboard pattern because
                                      // each 1 square change flips %2
          out[i + 1][j + 1] = '#';
        else
          out[i + 1][j + 1] = ' ';
      }
    }
    for (int i = 0; i < 8; i++) {
      // adds the letters and numbers on the side of the board
      out[i * 3 + 2][0] = (char)(i + 49);
      out[0][i * 5 + 3] = (char)(i + 97);
      out[i * 3 + 2][41] = (char)(i + 49);
      out[25][i * 5 + 3] = (char)(i + 97);
    }
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (b.square[i][j]->isEmpty() == false) {
          // only change pattern if there is a piece
          if (b.square[i][j]->getColour() == true)
            out[i * 3 + 2][j * 5 + 3] =
                tolower(b.square[i][j]->getType()); // black=true get lowercase
          else
            out[i * 3 + 2][j * 5 + 3] =
                b.square[i][j]->getType(); // white=false get uppercase
          // get type return the letter corresponding to the piece
        }
      }
    }
    string extraInfo[5];
    if (b.isFairy == true) {
      extraInfo[0] = "    Fairy Chess Combinations:";
      extraInfo[1] = "    Bishop = B";
      if (isMajor(b.bComb) ==
          true) { // only output the combination if it is valid
        extraInfo[1] += " + ";
        extraInfo[1] += b.bComb;
      }
      extraInfo[2] = "    Knight = N";
      if (isMajor(b.nComb) == true) {
        extraInfo[2] += " + ";
        extraInfo[2] += b.nComb;
      }
      extraInfo[3] = "    Rook = R";
      if (isMajor(b.rComb) == true) {
        extraInfo[3] += " + ";
        extraInfo[3] += b.rComb;
      }
      extraInfo[4] = "    Queen = Q";
      if (isMajor(b.qComb) == true) {
        extraInfo[4] += " + ";
        extraInfo[4] += b.qComb;
      }
    }
    if (b.turn == false) { // the board output will be flipped depending on the
                           // colour whose turn it is
      for (int i = 25; i >= 0; i--) {
        for (int j = 0; j < 42; j++) {
          //weird stuff is for colouring purposes
          os << outputSquare(out[i][j]);
        }
        if (25 - i <= 4)
          os << extraInfo[25 - i];
        os << endl;
      }
    } else {
      for (int i = 0; i < 26; i++) {
        for (int j = 41; j >= 0; j--) {
          os << outputSquare(out[i][j]);
        }
        if (i <= 4)
          os << extraInfo[i];
        os << endl;
      }
    }
    return os;
  }

  string legalize(const string &move, bool colour) {
    string legal;
    int i = 0;
    int len = move.length();
    bool cancel = false; // prevents moves to squares past one that is blocked
                         // (further in the chain)
    while (i < len) {
      if (cancel == true) {
        if (move[i + 3] == '<' || move[i + 3] == '|') {
          // these symbols mean the end chain of blocked moves
          cancel = false;
        }
        i += 4;
      } else {
        int col = letInt(move[i + 1]); // column from 0->7 where 'a'=0
        int row = numInt(move[i + 2]); // row from 0->7 where '1'=0
        char cond = move[i + 3];
        if (cond ==
            '>') { // if the square is blocked ignore the rest of the chain
          if (square[row][col]->isEmpty() == true) {
            legal += move.substr(i, 4);
          } else {
            cancel = true;
            if (square[row][col]->getColour() != colour) {
              legal += move.substr(i, 4);
            }
          }
        } else if (cond == '~') { // if the square is blocked ignore the rest of
                                  // the chain,
          // also only legal target is empty
          if (square[row][col]->isEmpty() == true) {
            legal += move.substr(i, 4);
          } else {
            cancel = true;
          }
        } else if (cond == '|') { // only legal if target empty, also end of
                                  // blocked chain for pawn
          if (square[row][col]->isEmpty() == true) {
            legal += move.substr(i, 4);
          }
        } else if (cond == '*') { // only legal if it takes a piece
          if (square[row][col]->isEmpty() == false) {
            if (square[row][col]->getColour() != colour) {
              legal += move.substr(i, 4);
            }
          }
        } else if (cond == '!') { // en passant
          if (lastMove ==
              move[i + 1]) { // lastMove stores the column of the last move if
                             // it is a double pawn move
            // otherwise it stores 'x', if that is the same column as the en
            // passant check, proceed
            if (numInt(move[i + 2]) == 5 - 3 * (int)colour) {
              // check that the planned move is going to the correct roww for EP
              // with that colour
              legal += move.substr(i, 4);
            }
          }
        } else if (cond == 'O') { // castle
          if (turn == colour)
            // only check castle if it is that king's turn
            // it is not important for determing check
            legal += checkCastle();
        } else {
          // no special conditions, just makes sure the square doesn't have a
          // piece of the same colour
          if (square[row][col]->isEmpty() == true) {
            legal += move.substr(i, 4);
          } else if (square[row][col]->getColour() != colour) {
            legal += move.substr(i, 4);
          }
        }
        i += 4;
      }
    }
    return legal;
  }

  string checkCastle() {

    string output; // this will be returned
    int row;
    if (turn == false) // if white's turn, check bottom row for castle
      row = 0;
    else // else, check top row for castle
      row = 7;
    if (squareAttacked(toAN(4, row), flip(turn)) ==
        true) // if king is in check, no castling
      return "";
    if (square[row][0]->getType() == 'R' &&
        square[row][0]->getHasMoved() == false) {
      // checking long castle here
      // rook has to be on corner and cannot move before castling
      bool lcast = true;
      for (int col = 3; col >= 2; col--) {
        // squares king passes through must be empty and not attacked
        if (square[row][col]->isEmpty() == false) {
          lcast = false;
          break;
        }
        // for efficiency sake, the empty square condition is checked first
        string pos = toAN(col, row); // position be safety checked
        bool other = flip(turn);
        bool attk = squareAttacked(pos, other);
        if (attk == true) {
          lcast = false;
          break;
        }
      }
      if (lcast == true)
        // Internal long castle notation
        output += "KO<O";
    }
    if (square[row][7]->getType() == 'R' &&
        square[row][7]->getHasMoved() == false) {
      // short castle check
      bool scast = true;
      for (int col = 5; col <= 6; col++) {
        if (square[row][col]->isEmpty() == false) {
          scast = false;
          break;
        }
        string pos = toAN(col, row);
        bool other = flip(turn);
        bool attk = squareAttacked(pos, other);
        if (attk == true) {
          scast = false;
          break;
        }
      }
      if (scast == true) {
        // internal short castle notation
        output += "KO>O";
      }
    }
    return output;
  }

  string getKingPos(bool colour) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (square[row][col]->getType() == 'K') {
          if (square[row][col]->getColour() == turn) {
            return square[row][col]->getPosition();
          }
        }
      }
    }
    return "ERROR"; // should never be returned
  }

  bool isCheck() {
    string kingpos = getKingPos(turn);
    bool other = flip(turn);
    return squareAttacked(kingpos, other);
  }

   bool squareAttacked(const string &pos,
                      bool colour) { // is this square attacked by this colour?
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (square[i][j]->isEmpty() == false) {
          if (square[i][j]->getColour() == colour) {
            // checks for pieces of the colour
            string move = square[i][j]->allMoves();
            move = legalize(move, colour);
            int found = move.find(
                pos); // is the position contained in the legalized moves
            // npos is returned when find fails, so if the result!=npos, it was
            // found
            if (found != string::npos)
              return true;
          }
        }
      }
    }
    return false;
  }

  char moveType(char type) {
    if (isMajor(type) == true) // i.e. R,N,B,Q,K
      return type;
    if (isLetter(type) ==
        true) // a through h, pawn moves have no letter in front
      return 'P';
    if (type == 'O') // castling is considered a king move internally
      return 'K';
    cout << "Invalid move notation\n"; //
    return 'x';
  }

  bool move(string input) { // takes an input move, tests if it's legal, then
                            // moves if possible
    // false=fail, true=success
    char type = moveType(input[0]);
    if (type == 'x')
      return false;
    int i = input.length() - 1;
    while (isNumber(input[i]) == false &&
           i >= 1) // a number on 0 is an invalid move, so stop at 1
      i--;
    int row_st = 0, row_end = 8, col_st = 0,
        col_end = 8; // start and end row/columns for search later on
    if (i >= 2) {
      // this section deals with notation like Red1 or R1d1 where multiple
      // pieces can move to the square it does so by restricting the
      // rows/columns searched for the desired move later on
      if (isLetter(input[i - 2]) == true) {
        col_st = letInt(input[i - 2]);
        col_end = col_st + 1;
      } else if (input[i - 2] == 'x' && i >= 3) {
        // Accounts for the possibilty that input looks like Rexd1
        if (isLetter(input[i - 3]) == true) {
          col_st = letInt(input[i - 3]);
          col_end = col_st + 1;
        }
      }
      if (isNumber(input[i - 2]) == true) {
        row_st = numInt(input[i - 2]);
        row_end = row_st + 1;
      } else if (input[i - 2] == 'x' && i >= 3) {
        if (isNumber(input[i - 3]) == true) {
          row_st = numInt(input[i - 3]);
          row_end = row_st + 1;
        }
      }
    }
    string target;
    if (i == 0) {
      if (input[0] ==
          'O') { // only castle moves have no number while being valid
        if (input.length() == 5) { // O-O-O length=5
          target = "O<O";
        } else if (input.length() == 3) { // O-O length=3
          target = "O>O";
        } else {
          cout << "Invalid move notation\n";
          return 0;
        }
      } else {
        cout << "Invalid move notation\n";
        return false;
      }
    } else if (isLetter(input[i - 1]) ==
               false) { // a letter must come before the last number
      cout << "Invalid move notation\n";
      return false;
    } else
      target = input.substr(i - 1, 2);
    int row_piece; // row
    int col_piece; // column
    int totalFound = 0;
    bool enPassantHolyHeck = false, twoSpace = false;
    // the name enPassantHolyHeck was inspired by a friend, it just stores
    // whether the move found is en passant twoSpace stores whether it is a
    // double pawn move
    for (int row = row_st; row < row_end; row++) {
      for (int col = col_st; col < col_end; col++) {
        if (square[row][col]->getColour() == turn) {
          if (square[row][col]->getType() == type) {
            string move = legalize(square[row][col]->allMoves(), turn);
            // all legal move the piece can make ignoring king safety
            int found = move.find(target);
            if (found != string::npos) {
              // move[found+2] will store the move condition, i.e. Pe4|,
              // P(i-1),e(i),4(i+1),|(i+2)
              if (move[found + 2] == '!') {
                enPassantHolyHeck = true;
              } else if (move[found + 2] == '|') {
                if (found >= 2) {
                  if (move[found - 2] == '~') { // move[found-2] will contain
                                                // '~' for double pawn moves
                    // extending the example above,
                    // Pe3~Pe4|,P(i-5),e(i-4),4(i-3),~(i-2)
                    twoSpace = true;
                  }
                }
              }
              // if the move is found, store location, +1 piece found
              row_piece = row, col_piece = col;
              totalFound++;
            }
          }
        }
      }
    }
    if (totalFound == 0) { // no pieces have the move
      cout << "Illegal move\n";
      return false;
    }
    if (totalFound > 1) {
      cout << "Multiple pieces have the move, invalid move notation\n";
      return false;
    }
    if (input[0] ==
        'O') { // castling is already tested to not put the king in check
      castle(target[1]);
      lastMove = 'x';
      savePosition();
      fiftyMove--;
      return true;
    }
    int row_tgt = numInt(target[1]), col_tgt = letInt(target[0]);
    bool test =
        testMove(row_piece, col_piece, row_tgt, col_tgt, enPassantHolyHeck);
    // tests if the king is in check after the move
    if (test == true) { // king will be safe, proceed

      // remember to delete pieces

      if (type == 'P') {
        if (target[1] == '1' || target[1] == '8') {
          // this section deals with pawn promotion
          // it is not a seperate function because move() needs to know if it
          // fails, and implementing such a function with that restriction would
          // be overcomplicating things
          char last = input[input.length() - 1];
          // unless it is check or checkmate, the desired piece will be the last
          // character in the move i.e. a8=Q
          if (last == '+' || last == '#')
            last = input[input.length() - 2];
          // if it is check/checkmate it will be the character right before
          if (isFairy == false) { // normal promotion
            if (last == 'R') {
              delete square[row_piece]
                           [col_piece]; // the pawn object is no longer needed
              square[row_piece][col_piece] =
                  new Rook(target[0], row_tgt + 1, turn);
            } else if (last == 'B') {
              delete square[row_piece][col_piece];
              square[row_piece][col_piece] =
                  new Bishop(target[0], row_tgt + 1, turn);
            } else if (last == 'N') {
              delete square[row_piece][col_piece];
              square[row_piece][col_piece] =
                  new Knight(target[0], row_tgt + 1, turn);
            } else if (last == 'Q') {
              delete square[row_piece][col_piece];
              square[row_piece][col_piece] =
                  new Queen(target[0], row_tgt + 1, turn);
            } else {
              cout << "Pawn promotion unspecified, invalid move notation\n";
              return 0;
            }
          } else { // fairy promotion
            if (last == 'R') {
              delete square[row_piece][col_piece];
              square[row_piece][col_piece] =
                  new Fairy(target[0], row_tgt + 1, turn, 'R', rComb);
            } else if (last == 'B') {
              delete square[row_piece][col_piece];
              square[row_piece][col_piece] =
                  new Fairy(target[0], row_tgt + 1, turn, 'B', bComb);
            } else if (last == 'N') {
              delete square[row_piece][col_piece];
              square[row_piece][col_piece] =
                  new Fairy(target[0], row_tgt + 1, turn, 'N', nComb);
            } else if (last == 'Q') {
              delete square[row_piece][col_piece];
              square[row_piece][col_piece] =
                  new Fairy(target[0], row_tgt + 1, turn, 'Q', qComb);
            } else {
              cout << "Pawn promotion unspecified, invalid move notation\n";
              return 0;
            }
          }
        }
      }
      square[row_piece][col_piece]->move(
          toAN(col_tgt, row_tgt)); // tells the piece object where it moved to
      // shuffles around pointers to relflect the new board state
      if (square[row_tgt][col_tgt]->isEmpty()==false) {
        fiftyMove=50;//resets the fifty move rule counter
      }
      else if (square[row_piece][col_piece]->getType()=='P') {
        fiftyMove=50;
      }
      else fiftyMove--;
      delete square[row_tgt][col_tgt]; // that piece object is no longer
                                       // relevant
      square[row_tgt][col_tgt] =
          square[row_piece][col_piece]; // square being moved to is overwritten
      square[row_piece][col_piece] =
          new Piece(); // overwrites square moved off of with empty
      if (enPassantHolyHeck == true) {
        // overwrites the square being passed with empty
        delete square[row_piece][col_tgt];
        square[row_piece][col_tgt] = new Piece();
      }
      if (twoSpace == true)
        lastMove =
            target[0]; // for enpassant, stores the column of a double pawn move
      else
        lastMove = 'x'; // the window for enpassant expires after one turn
      turn = flip(turn);
      savePosition();
      return 1;
    } else { // king is in check after move, don't proceed
      cout << "Illegal Move, king is in check after it\n";
      return 0;
    }
  }

  void castle(char direction) {
    if (direction == '<') {
      square[(int)turn * 7][4]->move(toAN(
          2, (int)turn * 7)); // tells piece objects their new position (king)
      square[(int)turn * 7][0]->move(toAN(3, (int)turn * 7)); //^(rook)
      // shuffles around piece pointers to reflect board state
      delete square[(int)turn * 7][2]; // these empty squares are not needed
      delete square[(int)turn * 7][3];
      square[(int)turn * 7][2] = square[(int)turn * 7][4];
      // if white's turn, then 0*7=0, if black's, then 1*7=7
      square[(int)turn * 7][3] = square[(int)turn * 7][0];
      square[(int)turn * 7][4] = new Piece();
      square[(int)turn * 7][0] = new Piece();
      turn = flip(turn);
    } else if (direction == '>') {

      square[(int)turn * 7][4]->move(toAN(6, (int)turn * 7));
      square[(int)turn * 7][7]->move(toAN(5, (int)turn * 7));
      delete square[(int)turn * 7][6];
      delete square[(int)turn * 7][5];
      square[(int)turn * 7][6] = square[(int)turn * 7][4];
      square[(int)turn * 7][5] = square[(int)turn * 7][7];
      square[(int)turn * 7][4] = new Piece();
      square[(int)turn * 7][7] = new Piece();
      turn = flip(turn);
    } else
      cout << "CASTLE_ERROR\n"; // should never occur
  }

  bool isMate() { // checks for no legal moves
    // rename variables
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (square[row][col]->isEmpty() == false) {
          if (square[row][col]->getColour() == turn) {
            string move = square[row][col]->allMoves();
            move = legalize(move, turn);
            // obtain all legal moves (before king safety) of all pieces of the
            // colour
            for (int k = 0; k < move.length(); k += 4) {
              bool enPassant = false;
              if (move[k + 3] == '!')
                enPassant = true;
              bool test = testMove(row, col, numInt(move[k + 2]),
                                   letInt(move[k + 1]), enPassant);
              if (test == true) // if any moves are possible, it is not
                                // checkmate or stalemate
                return false;
            }
          }
        }
      }
    }
    return true; // no moves are possible, it is mate
  }

  bool testMove(int num0, int let0, int num1, int let1, bool ep) {
    Piece *passant = square[num0][let1]; // store the piece taken by en passant
    if (ep == true)
      square[num0][let1] = new Piece(); // remove it if en passant is done
    Piece *saved = square[num1][let1]; // piece on square moved to, will need to
                                       // be put back on board after the test
    square[num1][let1] =
        square[num0][let0]; // shuffles pointers to set board state
    square[num0][let0] = new Piece();
    square[num1][let1]->setPos(
        let1, num1); // updates position of piece without flipping hasmoved
    bool result = flip(isCheck());
    delete square[num0][let0];               // delete the empty
    square[num1][let1]->setPos(let0, num0);  // sets piece to where it was
    square[num0][let0] = square[num1][let1]; // resets board state to what it
                                             // was
    square[num1][let1] = saved;
    if (ep == true) {
      delete square[num0][let1];
      square[num0][let1] = passant;
    }
    return result;
  }

Piece *getSquare(int row,int col) {
  return square[row][col];
}

virtual void savePosition() {}//this is just here so that move can call it

int getFiftyMove() {return fiftyMove;}

bool threefold() {
  int copy=0;
  bool soFarSoGood;
  for (int i=1;i<=50-fiftyMove;i++) {
    soFarSoGood=true;
    for (int row=0;row<8;row++) {
      for (int col=0;col<8;col++) {
        if (pieces[(l-1)%100][row][col]!=pieces[(l-1-i)%100][row][col])
          {
            soFarSoGood=false;
          }
      }
    }
    if (soFarSoGood==true) copy++;
    if (copy==2) {
      return true;
    }
  }
  return false;
}

protected:
  Piece *square[8][8];
  bool turn; // 0=white,1=black
  char lastMove =
      'x'; // contains file of last move if double pawn, else contains x
  bool isFairy;
  char rComb = 'x'; // fairy chess:piece rook combined with
  char bComb = 'x';
  char nComb = 'x';
  char qComb = 'x';
  int fiftyMove=50;
  char pieces[100][8][8];
  bool colours[100][8][8];
  bool moved[100][8][8];
  char lMove[100];
  int pastDepth[100];
  float pastCutoff[100];
  int l=0;
};
#endif