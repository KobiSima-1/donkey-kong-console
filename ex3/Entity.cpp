#include "Entity.h"

/*move
* move the entity on the board.
* @param tileBelow - the tile below the entity.
* @param board - the game board.
* @return void.
*/
void Entity::move(char tileBelow, Board& board) {

    updateDirection(tileBelow);

    //handle entity move.
    switch (direction) {
    case false: // Move left
        if (isWalkable(board, -1, 0)) {
            pos.move(GameConfig::Direction::LEFT);
        }
        break;
    case true: // Move right
        if (isWalkable(board, 1, 0)) {
            pos.move(GameConfig::Direction::RIGHT);
        }
        break;
    }
}

