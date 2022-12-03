#include "model.hxx"

using namespace ge211;

using Position = ge211::Posn<int>;

Model::Model(int size)
        : Model(size, size)
{}

Model::Model(int width, int height)
        : board_({width, height})
{
    // sets up all the bombs, and initialises the game by playing a move at a
    // safe position. Therefore, the first uncovered tile isn't a bomb. When
    // the game starts, the player hasn't won yet, and hasn't died yet.
    win_ = false;
    died_ = false;
    setup_bombs_();
    play_move(start_pos_());
}

Model::Rectangle
Model::board() //const
{
    return board_.all_positions();
}

Type
Model::operator[](Position pos) const
{
    return board_[pos];
}

/// MINESWEEPER FUNCTIONS START HERE

// Goes one step into direction 'dir' to check if there is a bomb.
// Returns a position_set of bombs. The position set would be empty if there
// are no bombs in that direction.
Position_set
Model::dir_bombs_(Position start, Dimensions dir) const
{
        // initialise position set
        Position_set res;

        // check for a bomb in direction of 'dir'
        start += dir;

        // if the tile in the 'dir' direction is off the board, or if it is a
        // safe position, return an empty position set, meaning there is no
        // bomb in that direction.
        if (!board_.good_position(start) or
            board_[start] == Type::safe)
        {
            return res;
        }

        // if the tile in the 'dir' direction has a bomb, add the position of
        // that bomb to the position set.
        if (board_.good_position(start) and
        board_[start] == Type::bomb)
        {
            res[start] = true;
            return res;
        }
        return res;
}

// Returns a union-ed set of positions using dir_bombs_ as a helper
// function for checking in each direction. Returns a position set of
// all adjacent bombs for that one position. We can iterate through this
// position set to count the total bombs adjacent to a tile.
Position_set
Model::all_adjacent_bombs_(Position pos) const
{
    // creates a position set for the result
    Position_set pset = Position_set();

    // iterate through all eight directions
    for (auto d: board_.all_directions())
    {
        // call dir_bombs helper function to check if that direction has a bomb
        Position_set curr = dir_bombs_(pos, d);

        // if the resulting position set isn't empty (meaning there's a
        // bomb), add the position of the bomb to the position set
        if (!curr.empty())
        {
            for (Position p: curr)
            {
                {
                    pset[p] = true;
                }
            }
        }
    }
    return pset;
}

// Takes the position set of all bombs from all_adjacent_bombs. This counts
// the number of bombs adjacent to a tile.
int
Model::count_bombs_(Position pos) const
{

    int count = 0;
    Position_set pset = all_adjacent_bombs_(pos);
    for (Position p : pset)
    {
        count++;
    }
    return count;
}

/// Returns position set for positions without any bombs adjacent to it.
// have to iterate through this position set to make the const solution board
Position_set
Model::no_bombs_() const
{
    Position_set pset;
    for (Position p : board_.all_positions())
    {
        if (count_bombs_(p) == 0 and !board_.getPset("bombs_")[p])
        {
            pset[p] = true;
        }
    }
    return pset;
}

// This returns a position set of randomised bomb locations.
Position_set
Model::bomb_positions_() const
{
    // initialise a position set
    Position_set res;

    // create a random integer object between 0 and 7
    Random_source<int> cord(0, 7);

    // initialise a counter to know when to stop adding bombs. Once there is
    // this number of bombs on the board, stop adding bombs.
    int count = 0;

    while (true)
    {
        if (count == 10)
        {
            return res;
        }
        // get the random integers
        int xcord = cord.next();
        int ycord = cord.next();

        // create a position with those random integers
        Position p(xcord, ycord);

        // if a random position generated is already in the position set (i.e.
        // it already has a bomb there), redo it and generate another position.
        if (!res[p])
        {
            res[p] = true;
            count++;
        }
    }
}

// the function above returned a position set of where all the bombs should be.
// This function marks those positions as bombs.
void
Model::setup_bombs_()
{
    // generate bomb positions
    Position_set bset = bomb_positions_();

    for (Position pos : bset)
    {
        // set this as a public variable in board so that we can update bomb
        // locations here
        board_.addPset(pos, "bombs_");
    }
}

// this function returns an initial position to start the game. This tile is
// guaranteed to be safe, so it's ensured that the first uncovered tile in the
// game is not a bomb.
Position
Model::start_pos_() const
{
    // calls our no_bombs_ helper function. This gives us a tile that has no
    // bombs to it at all.
    Position_set safe = no_bombs_();

    // counter of total safe positions
    int totalSafePos = 0;

    // creating a position var that will be returned if the random generator
    // doesn't work
    Posn<int> first(0,0);

    // counts how many safe positions there are
    for (Position p: safe) {
        totalSafePos++;

        // edge case: if there are no safe positions, return the first position
        // this should never run, because there is always a safe position.
        first = p;
    }

    // random object that generates a number between 0 and the number of safe
    // positions.
    Random_source<int> cord(0, totalSafePos);

    // returning
    int curr = 0;
    int rand = cord.next();
    for (Position p : safe)
    {
        curr++;
        if (curr == rand)
        {
            return p;
        }
    }
    return first;
}

/// should start game with one of the positions in pset that no_bombs_ returns


/// need two more functions to add to state of board: set up safe
/// tiles psets
/// for adjacent tiles, just need to add the number on top. that should be on
/// view


// play the move by adding to seen and removing from unknown set. if it's a
// bomb, die and end game. when uncovering the next move. check how many
// bombs are nearby for that position.
// need to store this number to display it on that position somehow
void
Model::play_move(Position pos)
{
    // add the played position to the position set of seen tiles
    board_.addPset(pos, "seen_");

    // remove the played position from position set of unknown tiles
    board_.removePset(pos, "unknown");

    // if the played position is a bomb, the player dies. Trigger the end of
    // the game
    if (board_.isBomb(pos))
    {
        died_ = true;
        set_game_over_();
    }

    // when there are no more unknown safe positions, the player has won
    else if ((board_.numTypes(Type::unknown) - 10) == 0)
    {
        win_ = true;
    }

    else
    {
        // count the number of bombs for the uncovered position
        int numBombs = count_bombs_(pos);

        // need to add this number to the adjacent position set
        board_.addAdjacent(pos, numBombs);

        //if playing this move causes unknown_ set to be empty
        if ((board_.numTypes(Type::unknown) - 10) == 0)
        {
            win_ = true;
        }
    }
}

// this function is called when the game is over
void
Model::set_game_over_()
{
    // when the player dies, display all the bombs
    board_.addBombsToSeen();
}

Board
Model::returnBoard()
{
    return board_;
}

// END OF MINESWEEPER FUNCTIONS
// COMMENTED-OUT FUNCTIONS FROM REVERSI ARE STILL BELOW. SCROLL DOWN!












/// No need
// /// const Move*
// Model::find_move(Position pos) const
// {
//     auto i = next_moves_.find(pos);
//
//     if (i == next_moves_.end()) {
//         // Nothing was found, so return NULL (nullptr in C++)
//         return nullptr;
//     } else {
//         // Dereferences the iterator to get the value then returns a pointer
//         // to that value. This is safe as the pointer is to a value living
//         // within the `next_moves_` structure.
//         return &(*i);
//     }
// }

/// NO need play move
// void
// Model::play_move(Position pos)
// {
//     if (is_game_over()) {
//         throw Client_logic_error("Model::play_move: game over");
//     }
//
//     const Move* movep = find_move(pos);
//     if (movep == nullptr) { // check if there was no such move
//         throw Client_logic_error("Model::play_move: no such move");
//     }
//
//     // `next_moves_`, etc.
//
//     really_play_move_(*movep);
// }

//
// BELOW ARE HELPER FUNCTIONS
// Our tests will run on the public functions, not the helper functions
// However, you are required to implement the following functions to help the
// public functions
//

// Position_set
// Model::find_flips_(Position current, Dimensions dir) const
// {
//     // TODO: HELPER FUNCTION
//     // gets current position as start. then go into given direction checking
//     // to see if there is opposing pieces across. then stop if there a
//     // friendly piece on other side.
//     // returns empty if there isnt a friendly piece at end after opposing
//     // piece, or if there arent any opposing pieces.
//
//     // while loop to check if going that direction results in opposing
//     // player piece.
//     //CREATE a set
//
//     Position_set res;
//
//     while(true)
//     {
//         current += dir;
//
//         if (!board_.good_position(current) or
//             board_[current] == Player::neither)
//         {
//             return {};
//         }
//
//         if (board_[current] == turn_)
//         {
//             return res;
//         }
//         res[current] = true;
//     }
// }

// Position_set
// Model::evaluate_position_(Position pos) const
// {
//     // TODO: HELPER FUNCTION
//
//     // creates a position set for the result
//     Position_set pset = Position_set();
//
//     // iterate through all eight directions union each
//     // result of find_flips into empty position set
//     if (board_[pos] == Player::neither) {
//         for (auto d: board_.all_directions()) {
//             pset.operator|=(find_flips_(pos, d));
//         }
//         if (pset.empty()) {
//             pset.clear();
//         } else {
//             pset[pos] = true;
//         }
//     }
//     return pset;
// }

//     // TODO: HELPER FUNCTION

//
// void
// Model::compute_next_moves_()
// {
//     // TODO: HELPER FUNCTION
//     //clear out all moves from movemap
//     next_moves_.clear();
//     //need to return all current moves available
//
//     //need to check if any of the 4 center positions
//     for (auto pos : board_.center_positions())
//     {
//         if (board_[pos] == Player::neither)
//         {
//             next_moves_[pos] = {pos};
//         }
//     }
//     //if next_moves is not empty, then still in opening phase and return
//     // these moves
//     if (!next_moves_.empty())
//     {
//         return;
//     }
//     //iterate through each position of the board if not in opening
//     else
//     {
//         for (Position pos : board_.all_positions())
//         {
//             // get the all the gains from that position
//             Position_set currPos = evaluate_position_(pos);
//
//             //if there are gains, that means its a valid move
//             if (!currPos.empty())
//             {
//                 //add that pos to the next_moves_ map as the key
//                 //set the gains of that pos as the value
//                 next_moves_[pos] = currPos;
//             }
//         }
//     }
// }
//
// bool
// Model::advance_turn_()
// {
//     // TODO: HELPER FUNCTION
//     //change the current turn to the other player
//     turn_ = other_player(turn_);
//     //compute next moves
//     compute_next_moves_();
//     //if next_moves_ is empty.  there arent any valid moves, so return false
//     // to whether current player has a valid move
//     if (next_moves_.empty())
//     {
//         return false;
//     }
//
//     //if next_moves_ is not empty, return true that there are valid moves for
//     // the next player
//     return true;
// }
//
// void
// Model::set_game_over_()
// {
//     // TODO: HELPER FUNCTION
//     turn_ = Player::neither;
//     int darkPoints = board_.count_player(Player::dark);
//     int lightPoints = board_.count_player(Player::light);
//     if (darkPoints > lightPoints)
//     {
//         winner_ = Player::dark;
//     }
//     else
//     {
//         winner_ = Player::light;
//     }
// }
//
// void
// Model::really_play_move_(Move move)
// {
//     // TODO: HELPER FUNCTION
//     //move = (position, position_set of gains)
//     //set position on board
//     //advance turn either player can still play (HAVE AVAILABLE MOVES)
//     //advance turn twice if next player cant play
//
//     //get the move and then use that to update board.
//     set that position to turn
//     board_.set_all(move.second, turn_);
//     //check if next player can play
//     bool check = advance_turn_();
//     if (!check)
//     {
//         //if cant play, then advance turn one more time
//         and check if he can play
//         check = advance_turn_();
//         if (!check)
//         {
//             //if not, both players cant play and its gg
//             set_game_over_();
//         }
//     }
// }
