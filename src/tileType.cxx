/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#include "tileType.hxx"

// Type
// other_player(Type player)
// {
//     switch (player) {
//     case Type::light:
//         return Type::dark;
//     case Type::dark:
//         return Type::light;
//     default:
//         return player;
//     }
// }

std::ostream&
operator<<(std::ostream& os, Type p)
{
    switch (p)
    {
    case Type::bomb:
        return os << "B";
    case Type::seen:
        return os << "S";
    case Type::unknown:
        return os << "U";
    case Type::flag:
        return os << "F";
    default:
        return os << "_";
    }
}

