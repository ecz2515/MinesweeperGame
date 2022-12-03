#include "controller.hxx"

#include <stdexcept>
#include <string>

/// Definitions for setting board and difficulty level

int
main(int argc, char *argv[]) {

    try {
        ///
        /// Don't change this command-line processing, as the grader
        /// will expect to be able to start your program this way.
        ///
        int width;
        int height;
        int bombs;
        switch (argc) {
            case 1:
                width = 8;
                height = 8;
                bombs = 10;
                break;
            case 3:
                /// If the user puts any other number. default to easy mode.
                if (std::stoi(argv[1]) == 1)
                {
                    width = 24;
                    height = 24;
                    bombs = 99;
                }
                else
                {
                    width = 8;
                    height = 8;
                    bombs = 10;
                }
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " [WIDTH HEIGHT]\n";
                return 1;
        }

        // You can change how you start your game class if you want
        // (but you probably don't need to):
        Controller(width, height).run();

    } catch (std::exception const& e) {
        // This prints out error messages if, say, the command-line argument
        // cannot be parsed as `int`s.
        std::cerr << argv[0] << ": " << e.what() << "\n";
        return 1;
    }

    return 0;
}
