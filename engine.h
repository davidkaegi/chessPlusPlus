#ifndef ENGINE_H
#define ENGINE_H
#include "board.h"
using namespace std;

class Engine : public Board {
public:
  Engine(bool type = 0) : Board(type) {
    maxDepth = 3;
    depth = 0;
  }

  void setMaxDepth(int max) { maxDepth = max; }

  double evaluate() {
    if (isMate() == true) {
      if (isCheck() == true) {
        if (turn == 0)
          return -100 + depth; // mate in less moves is better, so deeper=less
                               // valuable mate
        else
          return 100 - depth;
      } else
        return 0;
    } else if (insuffMatl() == true)
      return 0;
    else {
      double sum = 0;
      for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
          if (square[row][col]->isEmpty() == false) {
            char type = square[row][col]->getType();
            bool colour = square[row][col]->getColour();
            double change = 0; // values are mostly the pieces' material values
            if (type == 'P') { // incentivize pawn pushing by valuing more if
                               // they're closer to promotion
              if (colour == false)
                change = 0.9 + (double)row / 10.0;
              else
                change = 1.6 - (double)row / 10.0;
            } else if (type == 'B')
              change = 3.01; // bishops are slightly better than knights
            else if (type == 'N')
              change = 3;
            else if (type == 'R')
              change = 5;
            else if (type == 'Q')
              change = 9;
            if (colour == false)
              sum += change;
            else
              sum -= change;
          }
        }
      }
      // this section makes the bot value control over the centre
      for (int row = 3; row <= 4; row++) {
        for (int col = 3; col <= 4; col++) {
          if (square[row][col]->isEmpty() == false) {
            if (square[row][col]->getColour() == false)
              sum += 0.2;
            else
              sum -= 0.2;
          }
        }
      }
      return sum;
    }
  }

  double evalLine(int row0, int col0, int row1, int col1, bool ep) {
    bool promotion = false;
    double result = 42; // the meaning of the universe and an otherwise
                        // impossible error value
    bool hasMoved = square[row0][col0]->getHasMoved();
    Piece *original = square[row0][col0];
    Piece *passant = square[row0][col1]; // store the piece taken by en passant
    if (ep == true)
      square[row0][col1] = new Piece(); // remove it if en passant is done
    Piece *saved = square[row1][col1]; // piece on square moved to, will need to
                                       // be put back on board after the test
    square[row1][col1] = square[row0][col0];
    // shuffles pointers to set board state
    square[row0][col0] = new Piece();
    if (square[row1][col1]->getType() == 'P') {
      if (row1 == 0 || row1 == 7) {
        promotion = true;
        square[row1][col1] =
            new Fairy((char)(col1 + 97), row1 + 1, turn, 'Q', qComb);
        // if fairy is enabled, then it will by combined, otherwise, it is a
        // normal queen
      }
    }
    square[row1][col1]->move(toAN(col1, row1));
    if (isCheck() == false) {
      depth++;
      turn = flip(turn);
      result = engineMove();
      depth--;
      turn = flip(turn);
    }
    original->setPos(col0, row0); // sets piece to where it was
    if (promotion == true) {
      delete square[row1][col1]; // if the pawn queens, then the new queen will
                                 // be deleted
    }
    delete square[row0][col0];
    square[row0][col0] = original; // resets board state to what it
                                   // was
    square[row1][col1] = saved;
    if (ep == true) {
      delete square[row0][col1];
      square[row0][col1] = passant;
    }
    square[row0][col0]->setHasMoved(hasMoved);
    return result;
  }

  double engineMove() { // type double because depth!=0 iterations return the
                        // best evaluation
    if (isMate() == true) {
      if (isCheck() == true) {
        if (turn == false)
          return -100 + depth; // mate in less moves is better, so less value
                               // for a deeper mate
        else
          return 100 - depth;
      } else
        return 0;
    }
    if (insuffMatl() == true)
      return 0;
    if (depth == maxDepth) {
      // if depth gets higher than 3, moves take a long time to calculate
      // standard maxDepth=3, maybe on something more powerful than replit, the
      // bot could become stronger
      return evaluate();
    }
    int rowBest, colBest, rowTgt, colTgt;
    double bestEval, hereEval; // bestEval is the best evaluation at max depth
    // hereEval is the evaluation of that move at this depth, it is used to
    // break ties
    char newLastMove;
    bool ep = false;

    if (turn == false)
      bestEval = -101, hereEval = -101;
    else
      bestEval = 101, hereEval = 101;
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (square[row][col]->isEmpty() == false) {
          if (square[row][col]->getColour() == turn) {
            // only pieces of the colour past this point
            string move = legalize(square[row][col]->allMoves(), turn);
            int l = move.length();
            for (int i = 0; i < l; i += 4) {
              double res;
              int row1 = numInt(move[i + 2]); // row moved to
              int col1 = letInt(move[i + 1]); // column moved to
              char lMove = lastMove;          // stores what it was before
              bool enPassant = false;
              if (move[i + 3] == 'O') {
                castle(move[i + 2]);
                depth++;
                lastMove = 'x';
                res = engineMove();
                // castling is usually a good idea, so I gave the engine an
                // incentive to do so
                if (move[i + 2] == '>')
                  res += 0.5; // I think shortcastle is easier to play than
                              // longcastle
                else
                  res += 0.3; // the engine has limitation, so short is
                              // incentivized more
                depth--;
                turn = flip(turn);
                unCastle(move[i + 2]);
              } else {
                if (move[i + 3] == '!')
                  enPassant = true;
                else if (i >= 1) {
                  if (move[i + 3] == '|' && move[i - 1] == '~')
                    lastMove = move[i + 1];
                  else
                    lastMove = 'x';
                } else
                  lastMove = 'x';
                res = evalLine(row, col, row1, col1, enPassant);
              }
              if (res != 42) { // if res=42, the move would put the king in
                               // check
                if (turn == false) {
                  if (res > bestEval) {
                    bestEval = res, rowBest = row, colBest = col, rowTgt = row1,
                    colTgt = col1, newLastMove = lastMove;
                    ep = enPassant;
                    hereEval = evaluate();
                  } else if (res == bestEval) {
                    if (evaluate() > hereEval) {
                      bestEval = res, rowBest = row, colBest = col,
                      rowTgt = row1, colTgt = col1, newLastMove = lastMove;
                      ep = enPassant;
                      hereEval = evaluate();
                    }
                  }
                } else {
                  if (res < bestEval) {
                    bestEval = res, rowBest = row, colBest = col, rowTgt = row1,
                    colTgt = col1, newLastMove = lastMove;
                    ep = enPassant;
                  }
                }
              }
              lastMove = lMove; // restore balance to the force
            }
          }
        }
      }
    }
    if (depth == 0) {
      if ((char)(colTgt + 97) == 'O') { // this unndoes the letInt function
        cout << "Engine played";
        if ((char)(rowTgt + 49) == '<')
          cout << " Long ";
        else
          cout << " Short ";
        cout << "Castle\n";
        castle((char)(rowTgt + 49));
        lastMove = 'x';
        return 0;
      } else {
        lastMove='x';
        cout << "Engine played " << square[rowBest][colBest]->getType()
             << (char)(colBest + 97) << rowBest + 1 << " to "
             << (char)(colTgt + 97) << rowTgt + 1 << endl;
        if (square[rowBest][colBest]->getType() == 'P') {
          if (rowTgt == 0 || rowTgt == 7) {
            delete square[rowBest][colBest]; // byebye pawn
            square[rowBest][colBest] =
                new Fairy((char)(colTgt + 97), rowTgt + 1, turn, 'Q', qComb);
          }
          else if (abs(rowBest-rowTgt)==2) {
            lastMove=(char)(colBest + 97);
          }
        }
        delete square[rowTgt][colTgt]; // clears the square moved onto
        square[rowTgt][colTgt] = square[rowBest][colBest];
        square[rowTgt][colTgt]->move(toAN(colTgt, rowTgt));
        square[rowBest][colBest] = new Piece();
        if (ep == true) {
          delete square[rowBest][colTgt]; // byebye pawn
          square[rowBest][colTgt] = new Piece();
        }
        turn = flip(turn);
        return 0;
      }
    }
    return bestEval;
  }

  void unCastle(char direction) {    // for the engine
    delete square[(int)turn * 7][4]; // delete the empty space on the king
                                     // square
    if (direction == '<') {
      delete square[(int)turn * 7][0]; // delete empty on the rook square
      // shuffles around piece pointers to reflect board state
      square[(int)turn * 7][4] = square[(int)turn * 7][2];
      // if white's turn, then 0*7=0, if black's, then 1*7=7
      square[(int)turn * 7][0] = square[(int)turn * 7][3];
      square[(int)turn * 7][3] = new Piece();
      square[(int)turn * 7][2] = new Piece();
      square[(int)turn * 7][4]->move(toAN(
          4, (int)turn * 7)); // tells piece objects their new position (king)
      square[(int)turn * 7][0]->move(toAN(0, (int)turn * 7)); //^(rook)
      square[(int)turn * 7][4]->setHasMoved(false);
      square[(int)turn * 7][0]->setHasMoved(false);
    } else if (direction == '>') {
      delete square[(int)turn * 7]
                   [7]; // delete the empty space on the rook square
      square[(int)turn * 7][4] = square[(int)turn * 7][6];
      square[(int)turn * 7][7] = square[(int)turn * 7][5];
      square[(int)turn * 7][6] = new Piece();
      square[(int)turn * 7][5] = new Piece();
      square[(int)turn * 7][4]->move(toAN(4, (int)turn * 7));
      square[(int)turn * 7][7]->move(toAN(7, (int)turn * 7));
      square[(int)turn * 7][4]->setHasMoved(false);
      square[(int)turn * 7][7]->setHasMoved(false);
    } else
      cout << "CASTLE_ERROR\n"; // should never occur
  }

protected:
  int depth;
  int maxDepth;
};

#endif