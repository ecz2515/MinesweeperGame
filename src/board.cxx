/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#include "board.hxx"
#include <algorithm>
#include "position_set.hxx"

using namespace ge211;

Board::Board(Dimensions dims)
        : dims_(dims)
{
    if (dims_.width < 2 || dims_.height < 2) {
        throw Client_logic_error("Board::Board: dims too small");
    }

    if (dims_.width > Position_set::coord_limit ||
        dims_.height > Position_set::coord_limit) {
        throw Client_logic_error("Board::Board: dims too large");
    }
}

Board::Dimensions
Board::dimensions() const
{
    return dims_;
}

void
Board::addPset(Position pos, std::string pset)
{
    if (pset == "bombs_")
    {
        bombs_[pos] = true;
    }
    else if (pset == "seen_")
    {
        seen_[pos] = true;
    }
    else if (pset == "safe_")
    {
        safe_[pos] = true;
    }
    else if (pset == "flags_")
    {
        flags_[pos] = true;
    }
    else if (pset == "unknown_")
    {
        unknown_[pos] = true;
    }
}

void
Board::removePset(Position pos, std::string pset)
{
    if (pset == "bombs_")
    {
        bombs_[pos] = false;
    }
    else if (pset == "seen_")
    {
        seen_[pos] = false;
    }
    else if (pset == "safe_")
    {
        safe_[pos] = false;
    }
    else if (pset == "flags_")
    {
        flags_[pos] = false;
    }
    else if (pset == "unknown_")
    {
        unknown_[pos] = false;
    }
}

Position_set
Board::getPset(std::string set) const
{
    if (set == "bombs_")
    {
        return bombs_;
    }
    else if (set == "seen_")
    {
        return seen_;
    }
    else if (set == "safe_")
    {
        return flags_;
    }
    else if (set == "unknown_")
    {
        return unknown_;
    }
    else
    {
        Position_set s;
        return s;
    }
}

bool
Board::isBomb(Position pos) const
{
    if (bombs_[pos])
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
Board::isSeen(Position pos) const
{
    if (seen_[pos])
    {
        return true;
    }
    else
    {
        return false;
    }
}

void
Board::addBombsToSeen()
{
    for (Position b : bombs_)
    {
        seen_[b] = true;
    }
}

void
Board::addAdjacent(Position pos, int i)
{
    adjacent_.insert({pos, i});
}

bool
Board::good_position(Position pos) const
{
    return 0 <= pos.x && pos.x < dims_.width &&
           0 <= pos.y && pos.y < dims_.height;
}

Type
Board::operator[](Position pos) const
{
    bounds_check_(pos);
    return get_(pos);
}

Board::reference
Board::operator[](Position pos)
{
    bounds_check_(pos);
    return reference(*this, pos);
}

size_t
Board::numTypes(Type type) const
{
    switch (type)
    {
    case Type::bomb:
        return bombs_.size();
    case Type::seen:
        return seen_.size();
    case Type::flag:
        return flags_.size();
    case Type::unknown:
        return unknown_.size();
    default:
        return dims_.width * dims_.height -
               bombs_.size() - seen_.size();
    }
}

// /// Not needed
// Board::Rectangle
// Board::center_positions() const
// {
//     return Rectangle::from_top_left({dims_.width / 2 - 1,
//                                      dims_.height / 2 - 1},
//                                     {2, 2});
// }

static std::vector<Board::Dimensions>
build_directions()
{
    std::vector<Board::Dimensions> result;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx || dy) {
                result.push_back({dx, dy});
            }
        }
    }

    return result;
}

std::vector<Board::Dimensions> const&
Board::all_directions()
{
    static std::vector<Dimensions> result = build_directions();
    return result;
}

Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_);
}


/// is it necessary? no.
bool
operator==(Board b1, Board b2)
{
    return b1.dims_ == b2.dims_ &&
           b1.bombs_ == b2.bombs_ &&
           b1.safe_ == b2.safe_ &&
           b1.flags_ == b2.flags_ &&
           b1.unknown_ == b2.unknown_;
}


/// edited to return bomb, safe for player board?
/// should this return seen and unknown?
/// Might have bug (might not need the else statement)
Type
Board::get_(Position pos) const
{
    if (bombs_[pos]) {
        return Type::bomb;
    } else if (safe_[pos]) {
        return Type::safe;
    } else
    {
        return Type::bug;
    }
}

void
Board::set_(Position pos, Type type)
{
    switch (type) {
    case Type::bomb:
        bombs_[pos] = true;
        safe_[pos] = false;
        break;

    default:
        bombs_[pos] = false;
        safe_[pos] = true;
    }
}


///Sets all positions to bombs or safe. defaults to everything safe
void
Board::set_all(Position_set pos_set, Type type)
{
    switch (type) {
    case Type::bomb:
        bombs_ |= pos_set;
        safe_ &= ~pos_set;
        break;

    case Type::safe:
        safe_ |= pos_set;
        bombs_ &= ~pos_set;
        break;

    /// Unknown might not be necessary
    case Type::unknown:
        unknown_ |= pos_set;
        safe_ &= ~pos_set;
        bombs_ &= ~pos_set;

    default:
        unknown_ |= pos_set;
        safe_ |= ~pos_set;
        bombs_ &= ~pos_set;
    }
}

void
Board::bounds_check_(Position pos) const
{
    if (!good_position(pos)) {
        throw Client_logic_error("Board: position out of bounds");
    }
}

bool
operator!=(Board b1, Board b2)
{
    return !(b1 == b2);
}

std::ostream&
operator<<(std::ostream& os, Board const& board)
{
    Board::Dimensions dims = board.dimensions();

    for (int y = 0; y < dims.height; ++y) {
        for (int x = 0; x < dims.width; ++x) {
            os << board[{x, y}];
        }
        os << "\n";
    }

    return os;
}

Board::reference::reference(Board& board, Position pos) noexcept
        : board_(board),
          pos_(pos)
{ }

Board::reference&
Board::reference::operator=(reference const& that) noexcept
{
    *this = Type(that);
    return *this;
}

Board::reference&
Board::reference::operator=(Type type) noexcept
{
    board_.set_(pos_, type);
    return *this;
}

Board::reference::operator Type() const noexcept
{
    return board_.get_(pos_);
}

Board::multi_reference
Board::at_set(Position_set pos_set)
{
    return multi_reference(*this, pos_set);
}

Board::multi_reference::multi_reference(
        Board& board,
        Position_set pos_set) noexcept
        : board_(board),
          pos_set_(pos_set)
{ }

Board::multi_reference&
Board::multi_reference::operator=(Type type) noexcept
{
    board_.set_all(pos_set_, type);
    return *this;
}