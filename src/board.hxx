/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#pragma once

#include "tileType.hxx"
#include "position_set.hxx"

#include <ge211.hxx>

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

/// Represents the state of the board.
class Board
{
public:
    //
    // HELPFUL TYPE ALIASES
    //

    /// Board dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// Board positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// Board rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    // Defined and documented below.
    class reference;

    unordered_map<Position, int> adjacent_;



private:
    //
    // PRIVATE DATA MEMBERS
    //

    Dimensions dims_;
    // /// should update seen every 'turn'?
    Position_set bombs_;
    /// should update seen every 'turn'?
    Position_set seen_;
    Position_set safe_;
    Position_set flags_;
    Position_set unknown_;


    // INVARIANT: (light_ & dark_).empty()

public:
    //
    // PUBLIC CONSTRUCTOR & FUNCTION MEMBERS
    //

    ///Function to add a position to one of board's position sets
    void addPset(Position pos, std::string pset);

    ///Function to remove a position from one of board's position sets
    void removePset(Position pos, std::string pset);

    ///Function to return pset. Will return an empty pset if there is no pset
    /// of that string
    Position_set getPset(std::string set) const;

    ///Function that returns bool if bomb present in that position
    bool isBomb(Position pos) const;

    ///Function that returns bool if position is seen or unknown
    bool isSeen(Position pos) const;

    ///Function that adds adjacent[pos] = #ofbombs
    void addAdjacent(Position pos, int i);

    ///Function that adds bombs positions to seen_
    ///Needed for rendering all bombs when dead
    void addBombsToSeen();



    /// Constructs a board with the given dimensions.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if either dimension is less
    ///    than 2 or greater than 8.
    explicit Board(Dimensions dims);

    /// Returns the same `Dimensions` value passed to the
    /// constructor.
    Dimensions dimensions() const;

    /// Returns whether the given position is in bounds.
    bool good_position(Position) const;

    /// Returns the `Player` stored at `pos`.
    ///
    /// ## Errors
    ///
    ///  - throws `ge211::Client_logic_error` if `!good_position(pos)`.
    Type operator[](Position pos) const;

    //
    // PUBLIC CONSTRUCTOR & FUNCTION MEMBERS
    //

    /// Returns a reference to the `Player` stored at `pos`. This can
    /// be assigned to update the board:
    ///
    /// ```
    /// // Light player plays at (3, 4)
    /// board[{3, 4}] = Player::light;
    /// ```
    ///
    /// ## Errors
    ///
    ///  - throws `ge211::Client_logic_error` if `!good_position(pos)`.
    reference operator[](Position pos);

    /// Stores the given player in all the positions in the given set.
    /// For example,
    ///
    /// ```
    /// // Sets three positions to dark:
    /// Position_set positions{{0, 0}, {1, 1}, {2, 2}};
    /// board.set_all(positions, Player::dark);
    /// ```
    ///
    /// ## Errors
    ///
    ///  - behavior is undefined if any positions in the `Position_set`
    ///    are out of bounds.
    void set_all(Position_set, Type);

    /// Returns the number of tiles of that type
    size_t numTypes(Type) const;

    /// Returns a rectangle containing all the positions of the board. This
    /// can be used to iterate over the positions:
    ///
    /// ```
    /// for (Position pos : a_board.all_positions()) {
    ///     ... a_board[pos] ...;
    /// }
    /// ```
    ///
    /// Note that `Rectangle`s are considered to be closed on the top
    /// and left, but open on the bottom and right. The iterator will visit
    /// the correct positions for the board.
    Rectangle all_positions() const;

    /// Returns a rectangle containing the four center positions which
    /// much be occupied for play to move beyond them. This can be used
    /// to iterate over those positions.
    // Rectangle center_positions() const;

    /// Returns a reference to a `std::vector` containing all eight "unit
    /// direction vectors". In Python notation, these are:
    ///
    /// ```python
    /// { Dims(dx, dy)
    ///       for dx in [-1, 0, 1]
    ///           for dy in [-1, 0, 1]
    ///               if dx or dy }
    /// ```
    static std::vector<Dimensions> const& all_directions();

    /// Equality for boards.
    friend bool operator==(Board, Board);

    /// Defined and documented below.
    class multi_reference;

    /// Returns an object that allows assigning to all the positions in
    /// `set`. See below for the documentation of
    /// `Board::multi_reference::operator=(Player)`.
    ///
    /// ## Errors
    ///
    ///  - behavior is undefined if any positions in the `Position_set`
    ///    are out of bounds.
    multi_reference at_set(Position_set set);

private:
    //
    // PRIVATE HELPER FUNCTION MEMBERS
    //

    Type get_(Position where) const;
    void set_(Position where, Type who);
    void bounds_check_(Position where) const;

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif
};


//
// FREE FUNCTIONS FOR WORKING WITH THE CLASS
//

/// Inequality for boards.
bool
operator!=(Board const&, Board const&);

/// Board printing, suitable for debugging.
std::ostream&
operator<<(std::ostream&, Board const&);


//
// HELPER CLASSES
//

/// Class returned by `operator[](Position)` that simulates
/// an assignable reference to a `Posn<int>`. This is what allows
/// you to write
///
///     board[pos] = player;
///
/// to place `player` at `pos`.
///
/// The definition of the class follows this definition of the
/// `Board` class.
class Board::reference
{
    Board& board_;
    Position pos_;

public:
    /// Assigns the value of `that` to the object of `this`.
    reference& operator=(reference const&) noexcept;

    /// Assigns to the object of the reference.
    reference& operator=(Type) noexcept;

    /// Returns the value of the reference.
    operator Type() const noexcept;

private:
    friend class Board;

    reference(Board&, Position) noexcept;
};


/// Class returned by `at_set(Position_set)` that allows assigning
/// one player to all the positions in the given `Position_set`.
class Board::multi_reference
{
    Board& board_;
    Position_set pos_set_;

public:
    /// Assigns the given player to all the positions of this
    /// multi-reference, which are all the positions in the set that
    /// was given to `Board::at_set(Position_set)`. Thus, you can
    /// mass-assign a player to a set of positions like so:
    ///
    ///     // Sets three positions to dark:
    ///     Position_set positions{{0, 0}, {1, 1}, {2, 2}};
    ///     board.at_set(positions) = Player::dark;
    ///
    multi_reference& operator=(Type) noexcept;

private:
    friend class Board;

    multi_reference(Board&, Position_set) noexcept;
};

