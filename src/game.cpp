#include <QDebug>
#include "game.h"

/**
 * Constructor
 *
 * @param parent a pointer to the parent
 */
Game::Game(player_t & p1, player_t & p2, QObject *parent) : QObject(parent)
{
    // Get the instance of the main application
    app = QCoreApplication::instance();

    // create essential objects

    board   = new Board(this);
    player1 = createPlayer(p1);
    player2 = createPlayer(p2);

    turn = player2;

    //piece   = new Piece(this);
}

/**
 * Main game function. Creates the objects of Player, Board, Piece.
 * Controls the game state.
 */
void Game::run() {
    Piece* piece = turn->choosePiece();

    while (!checkVictory() && piece != NULL) {
        turn = getOpponent(turn);
        piece = turn->move(piece);
        board->printMatrix();
        board->printStock();
    }

    if (checkVictory()) { // someone has won
        qDebug() <<  turn->getName() << " has won!";
    } else { // draw
        qDebug() << "It is a draw!";
    }

    quit();
}

bool Game::checkVictory()
{
    QList<Piece *> fields;

    // check diagonals
    for (unsigned i = 0; i < MATRIX_SIZE; ++i)
        fields.append(board->getMatrix()[i][i]);

    if (Piece::checkVictoryFields(fields))
        return true;

    fields.clear();

    for (unsigned i = 0; i < MATRIX_SIZE; ++i)
        fields.append(board->getMatrix()[i][MATRIX_SIZE - 1 - i]);

    if (Piece::checkVictoryFields(fields))
        return true;

    // check top-down
    for (unsigned i = 0; i < MATRIX_SIZE; ++i) {
        fields.clear();
        for (unsigned j = 0; j < MATRIX_SIZE; ++j)
            fields.append(board->getMatrix()[i][j]);

        if (Piece::checkVictoryFields(fields))
            return true;

    }


    // check left-right
    for (unsigned i = 0; i < MATRIX_SIZE; ++i) {
        fields.clear();
        for (unsigned j = 0; j < MATRIX_SIZE; ++j)
            fields.append(board->getMatrix()[j][i]);

        if (Piece::checkVictoryFields(fields))
            return true;
    }
    return false;
}

/**
 * Quits the application
 */
void Game::quit() {
    emit finished();
}


/**
 * @brief Game::getOpponent
 * @param player
 * @return returns opponent (second player) of given player
 */
Player *Game::getOpponent(Player *player)
{
    if (player == player1)
        return player2;
    if (player == player2)
        return player1;
    else
        throw "getOpponent: invalid given player";  // TODO
}

/**
 * @brief Game::createPlayer creates a player according to given structure
 * @param player
 * @return
 */
Player *Game::createPlayer(struct player_t &player)
{
    Player *retval = NULL;
    switch (player.type) {
    case RANDOM:
        retval = new PlayerRandom("Random", board, this);
        break;
    case NOVICE:
        retval = new PlayerNovice("Novice", board, this);
        break;
    // TODO
    default:
        throw "NOT IMPLEMENTED";
        break;
    }

    return retval;
}

/**
 * Last cleaning before quitting.
 */
void Game::aboutToQuitApp() {
    // cleaning
}
