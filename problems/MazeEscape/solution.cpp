#include<cassert>
#include<fstream>
#include<iostream>
#include<memory>
#include<string>
#include<queue>
#include<unordered_map>

using namespace std;

enum Direction { up, right, down, left, _last };
enum Angle { angle0 = 0, angle90 = 90, angle180 = 180, angle270 = 270 };

struct DMeta {
    Direction d;
    int dx;
    int dy;
    Angle angle;

    DMeta(Direction d, int dx, int dy, Angle angle)
        : d(d), dx(dx), dy(dy), angle(angle) {}
};

const DMeta DMETA[Direction::_last] = {
    DMeta(Direction::up, 0, -1, Angle::angle0),
    DMeta(Direction::right, 1, 0, Angle::angle90),
    DMeta(Direction::down, 0, 1, Angle::angle180),
    DMeta(Direction::left, -1, 0, Angle::angle270),
};

Direction intToDirection(int d) {
    switch (d) {
    case 0: return Direction::up;
    case 1: return Direction::right;
    case 2: return Direction::down;
    case 3: return Direction::left;
    }
    throw runtime_error("Unknown direction code");
}

Direction moveToDirection(int dx, int dy) {
    assert(abs(dx) + abs(dy) == 1);

    for (const auto &dm : DMETA) {
        if (dm.dx == dx && dm.dy == dy) {
            return dm.d;
        }
    }
    throw runtime_error("Unreachable");
}

// Direction absoluteDirection(Direction absFrom, Direction relTo) {
//     const size_t relAngle = (360 + DMETA[absTo].angle - DMETA[absFrom].angle) % 360;
//     return angleToDirection(relAngle);
// }

Direction angleToDirection(Angle angle) {
    for (const auto &dm : DMETA) {
        if (angle == dm.angle) {
            return dm.d;
        }
    }
    throw runtime_error("Unknown angle");
}

Direction relativeDirection(Direction absFrom, Direction absTo) {
    const size_t relAngle = (360 + DMETA[absTo].angle - DMETA[absFrom].angle) % 360;
    for (const auto &dm : DMETA) {
        if (relAngle == dm.angle) {
            return dm.d;
        }
    }
    throw runtime_error("Unreachable");
}

Direction oppositeDirection(Direction d) {
    return angleToDirection(Angle((DMETA[d].angle + 180) % 360));
}

string directionToString(Direction d) {
    switch (d) {
    case Direction::up: return "UP";
    case Direction::right: return "RIGHT";
    case Direction::down: return "DOWN";
    case Direction::left: return "LEFT";
    default: throw runtime_error("Unknown direction code");
    }
}

typedef char Cell;

const Cell CELL_UNKNOWN = '.';
const Cell CELL_WALL = '#';
const Cell CELL_PATH = '-';
const Cell CELL_EXIT = 'e';

bool isPathCell(const Cell c) {
    return CELL_PATH == c || (0 <= c - '0' && c - '0' <= 9) || (0 <= c - 'A' && c - 'A' <= 5);
}

class Map {
private:
    typedef unordered_map<int, unordered_map<int, Cell>> Grid;
    
    int north_;
    int west_;
    size_t width_;
    size_t height_;
    Grid grid_;

public:
    Map(int north = 0, int west = 0, size_t width = 0, size_t height = 0)
        : north_(north), west_(west), width_(width), height_(height) {}
    
    const int &north() const {
        return north_;
    }

    const int &west() const {
        return west_;
    }

    const size_t &width() const {
        return width_;
    }

    const size_t &height() const {
        return height_;
    }

    const Cell &get(int x, int y) const {
        auto col = grid_.find(x);
        if (grid_.end() == col) {
            return CELL_UNKNOWN;
        }

        auto cell = col->second.find(y); 
        if (col->second.end() == cell) {
            return CELL_UNKNOWN;
        }

        return cell->second;
    }

    void set(int x, int y, Cell c) {
        if (west_ > x) {
            width_ += size_t(west_ - x);
            west_ = x;
        } else if (x >= west_ + int(width_)) {
            width_ += size_t(x - (west_ + int(width_) - 1));
        }
        if (north_ > y) {
            height_ += size_t(north_ - y);
            north_ = y;
        } else if (y >= north_ + int(height_)) {
            height_ += size_t(y - (north_ + int(height_) - 1));
        }
        grid_[x][y] = c;
    }

    void setVisitedFrom(int x, int y, Direction d) {

    }

    void merge(int n, int w, const Map &area) {
        for (int x = area.west(); x < area.west() + int(area.width()); x++) {
            for (int y = area.north(); y < area.north() + int(area.height()); y++) {
                set(w + x - area.west(), n + y - area.north(), area.get(x, y));
            }
        }
    }

    Map rotate(Angle angle) const {
        assert(width() == height());

        Map rotated;

        const int NO = north();
        const int WE = west();
        const int WI = int(width());
        const int HE = int(height());

        switch (angle) {
        case Angle::angle90:
            for (int x = WE; x < WE + WI; x++) {
                for (int y = NO; y < NO + HE; y++) {
                    rotated.set(x, y, get(y, WE + WI - 1 - (x - WE)));
                }
            }
            break;

        case Angle::angle180:
            for (int x = WE; x < WE + WI; x++) {
                for (int y = NO; y < NO + HE; y++) {
                    rotated.set(x, y, get(WE + WI - 1 - (x - WE), NO + HE - 1 - (y - NO)));
                }
            }
            break;

        case Angle::angle270:
            for (int x = WE; x < WE + WI; x++) {
                for (int y = NO; y < NO + HE; y++) {
                    rotated.set(x, y, get(NO + HE - 1 - (y - NO), x));
                }
            }
            break;

        default:
            throw runtime_error("Unkown rotation angle");
        }

        return rotated;    
    }

    string toString() const {
        string rv = "north=" + to_string(north()) + 
                    " west=" + to_string(west()) +
                    " width=" + to_string(width()) + 
                    " height=" + to_string(height()) + "\n";

        for (int y = north(); y < north() + int(height()); y++) {
            for (int x = west(); x < west() + int(width()); x++) {
                rv += get(x, y);
            }
            rv += '\n';
        }
        return rv;
    }
};

bool find(const Cell c, const Map &map, const int x, const int y, const size_t deep, int &dx, int &dy) {
    assert(isPathCell(map.get(x, y)));

    struct Node {
        typedef shared_ptr<Node> PNode;

        const int x;
        const int y;
        const size_t deep;
        const shared_ptr<Node> parent;

        Node(int x, int y, PNode parent = PNode(NULL))
            : x(x), y(y), deep(parent ? parent->deep + 1 : 0), parent(parent) {}
    };
    typedef Node::PNode PNode;

    Map visited;
    queue<PNode> fringe;
    fringe.push(PNode(new Node(x, y)));

    while (fringe.size()) {
        auto node = fringe.front();
        fringe.pop();
        if (c == map.get(node->x, node->y)) {
            do {
                dx = node->x - x;
                dy = node->y - y;
            } while ((node = node->parent) && node->deep);
            return true;
        }

        visited.set(x, y, 'x');

        for (const auto &dm : DMETA) {
           PNode neighbour(new Node(node->x + dm.dx, node->y + dm.dy, node));
           if ('x' != visited.get(neighbour->x, neighbour->y) && neighbour->deep <= deep) {
                fringe.push(neighbour);
            } 
        }
    }

    return false;
}

enum class PlayerState { initial, wall, lookup };

PlayerState deserializePlayerState(int ps) {
    switch (ps) {
    case 0: return PlayerState::initial;
    case 1: return PlayerState::wall;
    case 2: return PlayerState::lookup;
    }
    throw runtime_error("Unkown player state");    
}

class Player {
public:
    PlayerState state;
    size_t step;
    int x; 
    int y;
    Direction d;

    Player(): state(PlayerState::initial), step(0), x(0), y(0), d(Direction::up) {}

    Direction move(Map &map) {
        map.setVisitedFrom(x, y, oppositeDirection(d)); 

        int dx, dy;

        // Look for exit in <= 2 steps
        if (find(CELL_EXIT, map, x, y, 2, dx, dy)) {
            return doMove_(dx, dy);
        }

        // Follow unfinished wall if exists
        if (findMoveAlongUnfinishedWall_(dx, dy)) {
            return doMove_(dx, dy);
        }

        // TODO: look for unvisited walls adjacent to path cell on the map and go to the nearest if any

        // TODO: look for unvisited cells on the map and go to the nearest if any

        // TODO: look for unvisited cells outside the map and go to the nearest

        // TODO: remove me
        if (Direction::up == d) {
            return doMove_(1, 0);
        } 
        if (Direction::right == d) {
            return doMove_(0, 1);
        } 
        if (Direction::down == d) {
            return doMove_(-1, 0);
        }
        if (Direction::left == d) {
            return doMove_(0, -1);
        }
        throw runtime_error("Unreachable");
    }

private:
    bool findMoveAlongUnfinishedWall_(int &dx, int &dy) const {
        // for (const auto &dm : DMETA) {
        //     if (CELL_WALL == map.get(x + dm.dx, y + dm.dy)) {
        //         return moveAlongUnvisitedWall_(dm);
        //     }
        // }
        return false;
    }

    Direction doMove_(const int dx, const int dy) {
        auto nextD = moveToDirection(dx, dy);
        auto rv = relativeDirection(d, nextD);

        x += dx;
        y += dy;
        d = nextD;
        step++;

        return rv;
    }
};

const char *filename = "state.dat";

void serialize(const Player &player, const Map &map) {
    ofstream file(filename, ofstream::trunc);
    if (!file) {
        throw runtime_error("Cannot open file for writing");
    }

    file << int(player.state) << " " << player.step
         << " " << player.x << " " << player.y 
         << " " << player.d << endl;
    file << map.north() << " " << map.west() << " " 
         << map.width() << " " << map.height() << endl;

    for (int y = map.north(); y < map.north() + int(map.height()); y++) {
        string row(map.width(), '?');
        for (int x = map.west(); x < map.west() + int(map.width()); x++) {
            row[size_t(x - map.west())] = map.get(x, y);
        }
        file << row.c_str() << endl;
    }

    file.flush();
}
 
void deserialize(Player &player, Map &map) {
    ifstream file(filename);
    if (!file) {
        return;
    }
    file.exceptions(ifstream::badbit | ifstream::failbit);

    int ps, d;
    file >> ps >> player.step >> player.x >> player.y >> d;
    player.state = deserializePlayerState(ps);
    player.d = intToDirection(d);
   
    int north, west; 
    size_t width, height; 
    file >> north >> west >> width >> height;
    file.ignore();

    for (int y = north; y < north + int(height); y++) {
        string line;
        getline(file, line);
        if (line.size() != width) {
            throw runtime_error("Malformed data");
        }
        for (int x = west; x < west + int(width); x++) {
            Cell c = line[size_t(x - west)];
            if (CELL_UNKNOWN != c) {
                map.set(x, y, c);
            }
        }
    }

    assert(map.north() == north);
    assert(map.west() == west);
    assert(map.width() == width);
    assert(map.height() == height);
}

Angle calcNormalizedAngle(Direction d) {
    switch (d) {
    case Direction::up: return angle0;
    case Direction::right: return angle90;
    case Direction::down: return angle180;
    case Direction::left: return angle270;
    default: throw runtime_error("Unreachable");
    }
}

int main() {
    int playerId; // ignored
    cin >> playerId;
    cin.ignore();

    Player player;
    Map map; 

    // read map & last player pos & direction from the file (if any)
    deserialize(player, map);

    // read area from stdin
    Map area(-1, -1, 3, 3);
    for (int y = area.north(); y < area.north() + int(area.height()); y++) {
        for (int x = area.west(); x < area.west() + int(area.width()); x++) {
            area.set(x, y, cin.get());
        }
        cin.ignore();
    }

    // rotate area (normalize)
    auto angle = calcNormalizedAngle(player.d);
    if (angle) {
        area = area.rotate(angle);
    }

    // map += area
    map.merge(player.y - 1, player.x - 1, area);

    // move lookup:
    //   - have a wall to follow? Continue folowwing
    //   - know about unfollowed wall - go to the closest point of this wall
    //   - go to the "best" unvisited (but known) point:
    //     - point inside known rect
    //     - closest point outside

    // do move 
    //  - change player pos and direction
    //  - cout relative direction
    cout << directionToString(player.move(map)) << endl;

    // save to the file
    serialize(player, map);
}

