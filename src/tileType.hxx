/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#pragma once

#include <iostream>

// A player or lack thereof.
enum class Type
{
    bomb,
    seen,
    flag,
    safe,
    unknown,
    bug
};

// Returns the other player.
Type other_player(Type);

// Prints a player in a manner suitable for debugging.
std::ostream& operator<<(std::ostream&, Type);
