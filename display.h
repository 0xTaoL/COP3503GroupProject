#include <ncurses>
#include <string>

class display {
    unsigned char size[2];

public:
    display(std::string, unsigned char);
    void update();
};
