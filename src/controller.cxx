#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          mouse_pos{0,0}
{ }

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_pos);
}

//Custom function ------------------------------------
//on mouse move, have the piece follow the mouse.
void
Controller::on_mouse_move(ge211::Posn<int> position)
{
    // update the position of the mouse
    mouse_pos = position;
}

// when player clicks, get the mouse position to the position clicked on the
// board. Find the corresponding tile, and play_move_ there.
void
Controller::on_mouse_down(ge211::Mouse_button button, View::Position spos)
{
    // get the board position from mouse position
    Model::Position boardpos = view_.screen_to_board(spos);

    // play the move when:
        // 1. the player hasn't died
        // 2. the player left clicks
        // 3. the status of the tile is not known
    if (!model_.died_
        and button == ge211::Mouse_button::left)
        // and (view_.model_.returnBoard().getPset("unknown_")[boardpos]))
    {

        model_.play_move(boardpos);
    }

    // if user right clicks and player not died, check if there is a flag in
    // that position
    if (!model_.died_ and button == ge211::Mouse_button::right and
    !model_.win_)
    {
        cout << "left mouse ";
        cout << model_.returnBoard().getPset("flags_");
        cout << model_.returnBoard().getPset("flags_")[boardpos];
        // if there is already a flag there, remove the flag
        if (model_.returnBoard().getPset("flags_")[boardpos] == 1)
        {
            std::cout << "removing flag from pset";
            model_.returnBoard().removePset(boardpos, "flags_");
        }

        // else, add the hovered position to the flags_ pset in board.
        else
        {
            std::cout << "add flag to pset";
            model_.returnBoard().addPset(boardpos, "flags_");
        }
    }
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}