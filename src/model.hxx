
// You may add private members if you like, or even remove
// the private helper functions, but you mustn't change the
// existing public members, nor add new ones. If you do, your
// code may not build for automated tests.

#pragma once

#include "tileType.hxx"
#include "board.hxx"

#include <ge211.hxx>

#include <iostream>
#include <vector>

// Represents the state of the Reversi game.
class Model
{
public:
    /***************************************************/
    /*** DON'T CHANGE ANYTHING IN THE PUBLIC SECTION ***/
    /***************************************************/

    bool win_;
    bool died_;
    /// Model dimensions will use `int` coordinates, as board dimensions do.
    using Dimensions = Board::Dimensions;

    /// Model positions will use `int` coordinates, as board positions do.
    using Position = Board::Position;

    /// Model rectangles will use `int` coordinates, as board rectangles do.
    using Rectangle = Board::Rectangle;

    // /// made board public
    // Board board_;
    /// Constructs a model with `size` as both its width and height.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if `size` is less than 2
    ///    or greater than 8.
    explicit Model(int size = 8);

    /// Constructs a model with the given width and height.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if either dimension is less
    ///    than 2 or greater than 8.
    Model(int width, int height);

    /// Returns a rectangle containing all the positions of the board.
    /// This can be used to iterate over the positions.
    Rectangle board();

    // /// Returns whether the game is finished. This is true when neither
    // /// player can move.
    // bool is_game_over() const
    // { return turn() == Player::neither; }

    /// function should add the recently played move into the seen pset
    /// and out of the unknown pset
    /// should probably also check if the position has a bomb and end the
    /// game by revealing all bombs
    void play_move(Position pos);

    ///Returns the board
    Board returnBoard();

    /// No need
    // /// Returns the current turn, or `Player::neither` if the game is
    // /// over.
    // Type turn() const
    // { return turn_; }

    /// NO need
    // /// Returns the winner, or `Player::neither` if there is no winner
    // /// (either because the game isn't over, or because it's a draw).
    // Player winner() const
    // { return winner_; }

    /// Returns the player at the given position, or `Player::neither` if
    /// the position is unoccupied.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if the position is out of
    ///    bounds.
    Type operator[](Position) const;


    /// Returns a pointer to the move that will result if the current
    /// player plays at the given position. If the current player cannot
    /// play at the given position, returns `nullptr`. (Also returns
    /// `nullptr` if the position is out of bounds.)
    ///
    /// Note that the returned pointer must be borrowed from `next_moves_`,
    /// not a pointer to a local variable defined within this function.
    ///
    // const Move* find_move(Position) const;

    /// Attempts to play a move at the given position for the current
    /// player. If successful, advances the state of the game to the
    /// correct player or game over.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if the game is over.
    ///
    ///  - Throws `ge211::Client_logic_error` if the move is not currently
    ///    allowed for the current player.
    ///
    // void play_move(Position);

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif

private:
    //
    // PRIVATE MEMBER VARIABLES
    // (Don't change these!)
    //

    Board board_;

    // INVARIANT:
    //  - `next_moves_` is always current for the state of the game.

    /**********************************************/
    /*** DO NOT CHANGE ANYTHING ABOVE THIS LINE ***/
    /**********************************************/
    //
    // You may add or change anything you like below this point.
    //

    //
    // PRIVATE HELPER FUNCTIONS
    //
    // Implementing these first is a very good idea.
    //

    /// Computes the set of positions to be flipped in direction `dir` if
    /// the current player (`turn_`) were to play at position `start`.
    /// In particular, if there is some `n` such that all of these hold:
    ///
    ///  - board_[start + 1 * dir] == other_player(turn_)
    ///  - board_[start + 2 * dir] == other_player(turn_)
    ///  - . . .
    ///  - board_[start + n * dir] == other_player(turn_)
    ///  - board_[start + (n + 1) * dir] == turn_
    ///
    /// then it returns the `Position_set`
    ///
    ///    {start + 1 * dir, ..., start + n * dir}
    ///
    /// Otherwise, it returns the empty set.
    ///
    /// (Helper for `evaluate_position_`.)
    // Position_set find_flips_(Position start, Dimensions dir) const;

    /// REPLACEMENT FUNCTIONS

    /// Goes one step into direction 'dir' to check if there is a bomb.
    /// Returns a position_set. Empty if no bomb present in that direction.
    Position_set dir_bombs_(Position start, Dimensions dir) const;

    /// Returns a unioned set of positions using find_bombs_ as a helper
    /// function for checking in each direction. Returns a set of positions
    /// that contains all nearby bombs for that one position. We can iterate
    /// thru this set to count bombs
    Position_set all_adjacent_bombs_(Position pos) const;

    /// Iterates thru each position in all_adjacent_bombs's returned position
    /// set. Returns an integer. This integer will be used to mark each
    /// position with adjacent bombs
    int count_bombs_(Position pos) const;

    // /// Returns a position_set for all adjacent clear areas that dont have an
    // /// integer (for how many bombs are adjancent) associated with it
    // Position_set first_reveal_(Position pos) const;

    /// Returns position set for positions without any bombs adjacent to it.
    /// useful for setting first move
    Position_set no_bombs_() const;

    /// Based on the difficulty level indicated, this function returns a
    /// randomised position_set of bomb locations.
    Position_set bomb_positions_() const;

    /// iterate through the returned set to set Bombs[{Position}] = true;
    void setup_bombs_();

    /// get random game start position from no_bombs_ helper function
    Position start_pos_() const;



    /// Sets game over if selected position is a bomb tile
    void set_game_over_();

};

