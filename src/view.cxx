#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 35;

// Colors are red-green-blue(-alpha), each component
// from 0 to 255.

// create colors for sprites
static Color const flagColor = Color::medium_red();
static Color const bombColor = Color::black();
static Color const gray {100, 100, 100};
static Color const unknown {200, 200, 200};
static Color const seenColor = gray;
static Color const unknownColor = unknown;
static Color const red = {255, 0, 0};

// make a size for each board unit.
static ge211::Dims<int> bdim {33,33};

// edited with new objects.
View::View(Model& model)
        : model_(model),
          seen_(bdim, seenColor),
          unknown_(bdim, unknownColor),
          flag_(15, flagColor),
          bomb_(15, bombColor),
          sans18("sans.ttf", 18),
          count()
          // is this only one count or can this be used for many
          // positions?

        // You may want to add sprite initialization here
{}

// 1. draw the board
// 2. draw all unknown positions as unknown sprites
// 3. draw all seen positions as their actual sprites
// 4. check if they are bombs, and the number of adjacent bombs
// 5. draw all flags
void View::draw(Sprite_set& set, ge211::Posn<int> mouse_pos)
{
    // loop through every position in the board
    for (Position bpos : model_.board()) {

        // get screen position from that board position
        Position screenPos = board_to_screen(bpos);

        // adjust tile position by moving it right and down to center
        Position tilePos = screenPos + Dimensions {2, 2};

        // get piece position setting it to the tilePos and then moving it right
        // and down to center
        Position bombPos = tilePos + Dimensions {1, 2}; //Dimensions
        Position countPos = tilePos + Dimensions {11, 0}; //Dimensions

        // return the board
        Board board = model_.returnBoard();

        // if the current position is seen, render it
        if (board.getPset("seen_")[bpos]) {
            set.add_sprite(seen_, tilePos, 0);

            // render all the bombs if there is a bomb in that position, and
            // that position is seen
            if (board.getPset("bombs_")[bpos])
            {
                set.add_sprite(bomb_, bombPos, 1);
            }
        }

        // render unknown if the position is not seen (unknown)
        else {
            set.add_sprite(unknown_, tilePos, 0);
        }

        // if the current position has been flagged, render a flag
        if (board.getPset("flags_")[bpos]) {
            cout << "position has been flagged";
            set.add_sprite(flag_, tilePos, 1);
        }
        // else
        // {
        //     set.add_sprite(un, tilePos, 1);
        // }

        // render and display the number of adjacent bombs to the position
        if (board.adjacent_.count(bpos) == 1)
        {
            if (board.adjacent_.at(bpos) > 0) {
                // create a text_sprite
                ge211::Text_sprite::Builder currCount(sans18);

                // get the number of adjacent bombs
                int counter = board.adjacent_[bpos];
                std::string c = std::to_string(counter);

                // set the number of bombs to text sprite
                // cout << c;
                currCount.color(red) << c;
                count[bpos.x][bpos.y].reconfigure(currCount);
                // reconfigure the count to the updated counter
                // newC.reconfigure(currCount);
                set.add_sprite(count[bpos.x][bpos.y], countPos, 2);
            }
        }
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Minesweeper";
}

//takes a board position ({1,1} and converts this position to physical pixel
// position)
View::Position
View::board_to_screen(Model::Position pos) const
{
    return {grid_size * pos.x, grid_size * pos.y};
}

// takes pixel position and converts to board position
Model::Position
View::screen_to_board(View::Position pos) const
{
    return {pos.x / grid_size, pos.y / grid_size};
}



// Position_set const&
// View::gains_(View::Position mouse_pos) const
// {
//     static Position_set emptySet;
//     //returns the current flips that position being hovered will gain.
//     //get the move object where the player is hovering over
//     {
//         Move const *movep = model_.find_move(mouse_pos);
//         //if there's a move, then return all the possible moves, or else
//         // return empty position set
//         if (movep) {
//             return movep->second;
//         } else {
//             return emptySet;
//         }
//     }
// }

/// Need to edit this to setup bombs, set up adjacents and etc..?
/// if not adjacents, then just check each tile every time.
// Arms-length collaboration with Evan Chen on how to add each
// sprite to the sprite set. (What to check for first and why, and why
// checking "isgameover" late causes the bug of winning pieces disappearing.
// void
// View::add_type_sprite_(
//         Sprite_set& set,
//         Type type,
//         Position pos,
//         int z) const
// {
//     {
//         if (type == Type::)
//         {
//             return;
//         }
//         //case for if the game is over, need to replace all loser pieces with
//         // the loser sprite
//         if (model_.is_game_over() and player != model_.winner())
//         {
//             set.add_sprite(loserSprite_, pos, z);
//         }
//         //for every position need to check if its light or dark to add to
//         // sprite set
//         else if (player == Player::light)
//         {
//             set.add_sprite(lightPiece_, pos, z);
//         }
//         else if (player == Player::dark)
//         {
//             set.add_sprite(darkPiece_, pos, z);
//         }
//
//
//     }
// }