#include"popen2.h"
#include<assert.h>
#include<array>
#include<fstream>
#include<iostream>
#include<string>
#include<vector>

using namespace std;

enum Direction { up, right, down, left, _last };

struct DirectionMeta {
    int angle;
    int dx;
    int dy;

    DirectionMeta(int angle, int dx, int dy): angle(angle), dx(dx), dy(dy) {}
};

const DirectionMeta DMETA[Direction::_last] = {
   DirectionMeta(0, 0, -1),   // up
   DirectionMeta(90, 1, 0),   // right
   DirectionMeta(180, 0, 1),  // down
   DirectionMeta(270, -1, 0), // left
};

Direction angleToDirection(int angle) {
    switch (angle) {
    case 0: return Direction::up;
    case 90: return Direction::right;
    case 180: return Direction::down;
    case 270: return Direction::left;
    }
    throw runtime_error("Unknown direction angle");
}

struct Coords {
    size_t x;
    size_t y;
};

class Map {
private:
    typedef vector<vector<char>> Grid;

    size_t width_;
    size_t height_;
    Grid grid_; 

    static void checkRangeX(const Grid &grid, size_t x) {
        if (0 == grid.size()) {
            throw runtime_error("Empty grid");
        }
        if (x >= grid[0].size()) {
            throw runtime_error("Out of range (x=" + to_string(x) + ")");
        }
    }

    static void checkRangeY(const Grid &grid, size_t y) {
        if (y >= grid.size()) {
            throw runtime_error("Out of range (y=" + to_string(y) + ")");
        }
    }

    class ColumnMut {
    private:
        Grid &grid_;
        const size_t x_;

    public:
        ColumnMut(Grid &grid, const size_t x): grid_(grid), x_(x) {
            checkRangeX(grid, x);
        }

        char& operator [](size_t y) {
            checkRangeY(grid_, y);
            return grid_[y][x_];
        }
    };

    class ColumnConst {
    private:
        const Grid &grid_;
        const size_t x_;

    public:
        ColumnConst(const Grid &grid, const size_t x): grid_(grid), x_(x) {
            checkRangeX(grid, x);
        }

        const char& operator [](size_t y) const {
            checkRangeY(grid_, y);
            return grid_[y][x_];
        }
    };

public:
    Map(): width_(0), height_(0) {}

    Map(size_t width, size_t height): width_(width), height_(height) {
        grid_ = Grid();
        grid_.resize(height_);
        for (auto &row : grid_) {
            row.resize(width_);
        }
    }

    size_t height() const {
        return height_;
    }

    size_t width() const {
        return width_;
    }

    ColumnMut operator [](size_t x) {
        return ColumnMut(grid_, x);
    }

    const ColumnConst operator [](size_t x) const {
        return ColumnConst(grid_, x);
    }

    void addRow(const string &row) {
        if (0 == height_) {
            assert(0 == width_);
            width_ = row.size();
        }
        if (row.size() != width_) {
            throw runtime_error("Only rectangular maps are supported"); 
        }
        grid_.push_back(vector<char>(row.begin(), row.end()));
        height_++;
    }

    bool find(const char c, Coords &coords) {
        for (size_t x = 0; x < width_; x++) {
            for (size_t y = 0; y < height_; y++) {
                if (c == (*this)[x][y]) {
                    coords.x = x;
                    coords.y = y;
                    return true;
                }
            }
        }
        return false;
    }

    Map getRegion(size_t north, size_t west, size_t width, size_t height) const {
        Map region(width, height);
        for (size_t x = 0; x < width; x++) {
            for (size_t y = 0; y < height; y++) {
                region[x][y] = (*this)[west + x][north + y];
            } 
        }
        return region;
    }

   /**
    RIGHT
    00 10 20      20 21 22
    01 11 21  ->  10 11 12
    02 12 22      00 01 02

    LEFT
    00 10 20      02 01 00
    01 11 21  ->  12 11 10
    02 12 22      22 21 20

    DOWN
    00 10 20      20 10 00
    01 11 21  ->  21 11 01
    02 12 22      22 12 02
    **/
    Map rotate(Direction d) const {
        auto rotated = getRegion(0, 0, width_, height_);
        if (Direction::right == d) {
            for (size_t x = 0; x < width_; x++) {
                for (size_t y = 0; y < height_; y++) {
                    rotated[x][y] = (*this)[height_ - y - 1][x];
                }
            }
        }
        if (Direction::left == d) {
            for (size_t x = 0; x < width_; x++) {
                for (size_t y = 0; y < height_; y++) {
                    rotated[x][y] = (*this)[y][width_ - x - 1];
                }
            }
        }
        if (Direction::down == d) {
            for (size_t x = 0; x < width_; x++) {
                for (size_t y = 0; y < height_; y++) {
                    rotated[x][y] = (*this)[width_ - x - 1][height_ - y - 1];
                }
            }
        }
        return rotated;
    }

    string toString() const {
        string result;
        for (const auto &row : grid_) {
            result += string(row.begin(), row.end()) + "\n"; 
        }
        return result;
    }
};

struct Player {
    const Map &map;
    size_t x;
    size_t y;
    Direction d;    
    size_t nSteps;

    Player(const Map &map, size_t x, size_t y, Direction d)
        : map(map), x(x), y(y), d(d), nSteps(0) {}

    bool isWinner() const {
        return ('e' == map[x][y]);
    }

    Map visibleArea() {
        assert(0 < y && y < map.height() - 1);
        assert(0 < x && x < map.width() - 1);     
        return map.getRegion(y - 1, x - 1, 3, 3).rotate(d);
    }

    void move(Direction rel) {
        int angle = (DMETA[d].angle + DMETA[rel].angle) % 360;
        Direction nextD = angleToDirection(angle);
        int nextX = int(x) + (DMETA[nextD].dx);
        int nextY = int(y) + (DMETA[nextD].dy);

        if (0 > nextX || size_t(nextX) > map.width() - 1) {
            throw runtime_error("Bad movement - out of range (x)");
        }
        if (0 > nextY || size_t(nextY) > map.height() - 1) {
            throw runtime_error("Bad movement - out of range (y)");
        }
        if ('#' == map[size_t(nextX)][size_t(nextY)]) {
            throw runtime_error("Bad movement - wall");
        }

        x = size_t(nextX);
        y = size_t(nextY);
        d = nextD;
        nSteps++;
    }
};

Direction parseDirection(const char *arg) {
    switch (arg[0]) {
        case 'U':
            return Direction::up;
        case 'D':
            return Direction::down;
        case 'L':
            return Direction::left;
        case 'R':
            return Direction::right;
    } 

    throw runtime_error("Unexpected direction arg");
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
       cout << "Wrong number of arguments" << endl;
       return 1;
    }

    int nIter = stoi(argv[4]);

    Map map;
    auto mapfile = ifstream(argv[1]);
    while (mapfile) {
        string line;
        getline(mapfile, line);
        if (line.size()) {
            map.addRow(line);
        }
    }

    Coords pos;
    if (!map.find('b', pos)) {
        throw runtime_error("Bot position not found");
    }
    map[pos.x][pos.y] = '-';
    Player player(map, pos.x, pos.y, parseDirection(argv[2]));

    const size_t BUF_SIZE = 32;
    array<char, BUF_SIZE> buf;
    while (nIter--) {
        cout << "Player x=" << player.x << " y=" << player.y << " d=" << player.d << endl;
        if (player.isWinner()) {
            cout << "Victory in " << player.nSteps << " steps" << endl;
            break;
        }

        unique_ptr<files_t, int(*)(files_t*)> sio(popen2(argv[3]), pclose2);
        if (!sio) {
            throw runtime_error("popen2() failed");
        }
        
        auto area = player.visibleArea().toString();
        auto input = "1\n" + area;
        cout << "SOLUTION INPUT:" << endl << input << "<<EOF" << endl;
        if (EOF == fputs(input.c_str(), sio->in)) {
            throw runtime_error("fputs() failed");
        }
        if (0 != fflush(sio->in)) {
            throw runtime_error("fflush() failed");
        }

        if (nullptr == fgets(buf.data(), BUF_SIZE, sio->out)) {
            throw runtime_error("fgets() failed");
        }
        cout << "SOLUTION OUTPUT:" << endl << buf.data() << "<<EOF" << endl;

        player.move(parseDirection(buf.data()));
        cout << endl;
    }
}

