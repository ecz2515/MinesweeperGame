#include "model.hxx"
#include <catch.hxx>

using namespace ge211;

/// MINESWEEPER TESTS START HERE

/// This tests if the game can be run completely. We declare a position set
/// of bombs, and then eventually play a move at one of bombs. The game should
/// realise that the player has died, as well as display the locations of all
/// the bombs. (See line 44 and 45)
TEST_CASE("set_game_complete")
{
    Model m(8, 8);
    // get board, then get pset bombs
    Position_set bombs = m.returnBoard().getPset("bombs_");

    Posn<int> bomb(0,0);
    int count = 0;
    for (Posn<int> b : bombs)
    {
        count++;
        if (count == 1)
        {
             bomb = b;
        }
    }
    m.play_move(bomb);

    int c = 0;

    Position_set seen = m.returnBoard().getPset("seen_");
    for (Posn<int> s : seen)
    {
        for (Posn<int> b : bombs)
        {
            if (s == b)
            {
                c++;
            }
        }
    }

    CHECK(m.died_ == true);
    CHECK(c == 10);
}

/// This tests if the game begins properly by revealing one safe tile. We
/// declare position sets of bomb tiles and seen tiles, and test if the tile
/// we reveal is in the set of bombs or not. If it's not, then the tile is
/// safe. See the CHECK_FALSE at line 70.
TEST_CASE("find_safe_pos")
{
    Model m(8,8);
    Position_set seen = m.returnBoard().getPset("seen_");
    Position_set bombs = m.returnBoard().getPset("bombs_");

    bool check = false;

    for (Posn<int> b : bombs)
    {
        for (Posn<int> s : seen)
        {
            if (s == b)
            {
                check = true;
            }
        }
    }
    CHECK_FALSE(check);
}

/// This tests if the game can play a move correctly. We have a set of seen
/// positions, and then play a move somewhere on the board. The game should
/// add that played position to the set of seen tiles (see line 89).
TEST_CASE("play_move")
{
    Model m(8, 8);
    m.play_move({1, 1});
    Position_set seen = m.returnBoard().getPset("seen_");
    bool check = false;
    for (Posn<int> s : seen)
    {
        if (s == Posn<int>(1, 1))
        {
            check = true;
        }
    }
    CHECK(check);
}

/// This tests the game can generate 10 UNIQUE random positions of bombs.
/// See line 109.
TEST_CASE("random_positions_for_bombs")
{
    Model m(8,8);

    Position_set bombs = m.returnBoard().getPset("bombs_");

    int count = 0;

    Position_set seen;

    for (Posn<int> b : bombs)
    {
        seen[b] = true;
        count++;
    }
    CHECK(seen.size() == 10);
}