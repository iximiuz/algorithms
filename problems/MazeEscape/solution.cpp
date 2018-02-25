#include<cassert>
#include<fstream>
#include<iostream>
#include<memory>
#include<string>
#include<sstream>
#include<queue>
#include<unordered_map>

using namespace std;

enum Direction { NORTH, OST, SOUTH, WEST, _DIRECTIONS_COUNT };
enum Movement  { FORWARD, RIGHT, BACKWARD, LEFT };
enum Angle { angle0 = 0, angle90 = 90, angle180 = 180, angle270 = 270 };

struct DMeta {
    Direction d;
    int dx;
    int dy;
    Angle angle;

    DMeta(Direction d, int dx, int dy, Angle angle)
        : d(d), dx(dx), dy(dy), angle(angle) {}
};

const DMeta DMETA[Direction::_DIRECTIONS_COUNT] = {
    DMeta(Direction::NORTH, 0, -1, Angle::angle0),
    DMeta(Direction::OST, 1, 0, Angle::angle90),
    DMeta(Direction::SOUTH, 0, 1, Angle::angle180),
    DMeta(Direction::WEST, -1, 0, Angle::angle270),
};

Direction intToDirection(int d) {
    switch (d) {
    case 0: return Direction::NORTH;
    case 1: return Direction::OST;
    case 2: return Direction::SOUTH;
    case 3: return Direction::WEST;
    }
    throw runtime_error("Unknown direction code");
}

Direction directionByAngle(Angle angle) {
    for (const auto &dm : DMETA) {
        if (angle == dm.angle) {
            return dm.d;
        }
    }
    throw runtime_error("Unknown angle");
}

Direction oppositeDirection(Direction d) {
    return directionByAngle(Angle((DMETA[d].angle + 180) % 360));
}

Movement movementByAngle(Angle angle) {
    return Movement(angle / 90);
}

Angle angleByMovement(Movement m) {
    return Angle(90 * m);
}

string movementToString(Movement m) {
    switch (m) {
    case Movement::FORWARD: return "UP";
    case Movement::RIGHT: return "RIGHT";
    case Movement::BACKWARD: return "DOWN";
    case Movement::LEFT: return "LEFT";
    default: throw runtime_error("Unknown movement code");
    }
}

typedef char Cell;

const Cell CELL_UNKNOWN = '.';
const Cell CELL_WALL = '#';
const Cell CELL_WALL_VISITED = 'x';
const Cell CELL_PATH = '-';
const Cell CELL_EXIT = 'e';

bool isPathCell(const Cell c) {
    return CELL_PATH == c || (0 <= c - '0' && c - '0' <= 9) || (0 <= c - 'A' && c - 'A' <= 5);
}

bool isWallCell(const Cell c) {
    return CELL_WALL == c || CELL_WALL_VISITED == c;
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
        Cell c = get(x, y);
        assert(isPathCell(c));

        if (c == CELL_PATH) {
            c = '0';
        }

        int code = stoi(string(1, tolower(c)), nullptr, 16) | (1 << d);
        stringstream ss;
        ss << uppercase << hex << code;
        set(x, y, ss.str()[0]);
    }

    bool isVisitedFrom(int x, int y, Direction d) const {
        Cell c = get(x, y);
        if (c == CELL_PATH) {
            c = '0';
        }
        return stoi(string(1, tolower(c)), nullptr, 16) & (1 << d);
    }

    void setVisitedWall(int x, int y) {
        set(x, y, CELL_WALL_VISITED);
    }

    void merge(int n, int w, const Map &area) {
        for (int x = area.west(); x < area.west() + int(area.width()); x++) {
            for (int y = area.north(); y < area.north() + int(area.height()); y++) {
                if (CELL_UNKNOWN == get(w + x - area.west(), n + y - area.north())) {
                    set(w + x - area.west(), n + y - area.north(), area.get(x, y));
                }
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
           if (
                   'x' != visited.get(neighbour->x, neighbour->y) 
                   && (isPathCell(map.get(neighbour->x, neighbour->y)) 
                      || CELL_EXIT == map.get(neighbour->x, neighbour->y))
                   && neighbour->deep <= deep
              ) {
               fringe.push(neighbour);
            } 
        }
    }

    return false;
}

struct NeighbourCell {
    const Cell val;
    const int x;
    const int y;
    const int dx;
    const int dy;

    NeighbourCell(Cell val, int x, int y, int dx, int dy)
        : val(val), x(x), y(y), dx(dx), dy(dy) {
        assert(abs(dx) + abs(dy) == 1);
    }
};

class Player {
public:
    int x; 
    int y;
    Direction d;
    const Map &map;

    Player(const Map &map) : x(0), y(0), d(Direction::NORTH), map(map) {}

    NeighbourCell cellAt(Movement m) const {
        Angle relAngle = angleByMovement(m);
        auto direction = directionByAngle(Angle((DMETA[d].angle + relAngle) % 360));
        auto dm = DMETA[direction];
        return NeighbourCell(map.get(x + dm.dx, y + dm.dy), x + dm.dx, y + dm.dy, dm.dx, dm.dy);
    }

    bool move(Movement m, int &dx, int &dy) {
        auto cell = cellAt(m);
        if (!isPathCell(cell.val)) {
            return false;
        }

        dx = cell.dx;
        dy = cell.dy;
        move(cell.dx, cell.dy);

        return true;
    }

    Movement move(const int dx, const int dy) {
        auto nextD = moveToDirection_(dx, dy);
        auto rv = movementTo_(nextD);

        x += dx;
        y += dy;
        d = nextD;

        return rv;
    }

private:
    Movement movementTo_(Direction to) const {
        const size_t relAngle = (360 + DMETA[to].angle - DMETA[d].angle) % 360;
        return movementByAngle(Angle(relAngle));
    }

    Direction moveToDirection_(int dx, int dy) {
        assert(abs(dx) + abs(dy) == 1);

        for (const auto &dm : DMETA) {
            if (dm.dx == dx && dm.dy == dy) {
                return dm.d;
            }
        }
        throw runtime_error("Unreachable");
    }
};

enum class SolverState { INITIAL, WALL, LOOKUP };

SolverState deserializeSolverState(int state) {
    switch (state) {
    case 0: return SolverState::INITIAL;
    case 1: return SolverState::WALL;
    case 2: return SolverState::LOOKUP;
    }
    throw runtime_error("Unkown player state");    
}

class Solver {
public:
    SolverState state;
    size_t step;

    Solver() : state(SolverState::INITIAL), step(0) {}

    Movement move(Player &player, Map &map) {
        step++;

        map.setVisitedFrom(player.x, player.y, oppositeDirection(player.d)); 
        if (setVisitedWallsIfAny_(player, map)) {
            state = SolverState::WALL;
        }

        int dx, dy;

        // Look for exit in <= 2 steps
        if (find(CELL_EXIT, map, player.x, player.y, 2, dx, dy)) {
            return player.move(dx, dy);
        }

        // Follow unfinished wall if exists
        if (SolverState::WALL == state && findMoveAlongUnfinishedWall_(player, map, dx, dy)) {
            return player.move(dx, dy);
        }

        state = SolverState::LOOKUP;
    
        // Check neighbour walls (depth = 1)
        if (findNeighbourWall_(player, dx, dy)) {
            return player.move(dx, dy);
        }

        // TODO: look for unvisited walls adjacent to path cell on the map and go to the nearest if any

        // TODO: look for unvisited cells on the map and go to the nearest if any

        // TODO: look for unvisited cells outside the map and go to the nearest

        // TODO: remove it
        if (find(CELL_PATH, map, player.x, player.y, 100, dx, dy)) {
            return player.move(dx, dy);
        }

        throw runtime_error("Unreachable");
    }

private:
    bool setVisitedWallsIfAny_(Player &player, Map &map) {
        bool found = false;
        do {
            auto rightNeighbour = player.cellAt(Movement::LEFT);
            if (CELL_WALL != rightNeighbour.val) {
                break;
            }
            map.setVisitedWall(rightNeighbour.x, rightNeighbour.y);
            found = true;

            auto topNeighbour = player.cellAt(Movement::FORWARD);
            if (CELL_WALL != topNeighbour.val) {
                break;
            }
            map.setVisitedWall(topNeighbour.x, topNeighbour.y);

            auto leftNeighbour = player.cellAt(Movement::RIGHT);
            if (CELL_WALL != leftNeighbour.val) {
                break;
            }
            map.setVisitedWall(leftNeighbour.x, leftNeighbour.y);
        } while (false);

        return found;
    }

    bool findNeighbourWall_(const Player &player, int &dx, int &dy) {
        assert(SolverState::LOOKUP == state);
        return probeNeighbourCell_(player, Movement::FORWARD, dx, dy) 
            || probeNeighbourCell_(player, Movement::LEFT, dx, dy)
            || probeNeighbourCell_(player, Movement::BACKWARD, dx, dy)
            || probeNeighbourCell_(player, Movement::RIGHT, dx, dy);
    }

    bool probeNeighbourCell_(Player player, Movement m, int &dx, int &dy) {
        return player.move(m, dx, dy) && CELL_WALL == player.cellAt(Movement::RIGHT).val;
    }

    bool findMoveAlongUnfinishedWall_(Player player, const Map &map, int &dx, int &dy) const {
        assert(SolverState::WALL == state);

        if (player.move(Movement::LEFT, dx, dy)) {
            return isWallCell(player.cellAt(Movement::LEFT).val) 
                && !map.isVisitedFrom(player.x, player.y, oppositeDirection(player.d));
        }

        if (player.move(Movement::FORWARD, dx, dy)) {
            if (isWallCell(player.cellAt(Movement::LEFT).val)) {
                return !map.isVisitedFrom(player.x, player.y, oppositeDirection(player.d));
            }

            int stub;
            return player.move(Movement::LEFT, stub, stub) 
                && !map.isVisitedFrom(player.x, player.y, oppositeDirection(player.d));
        }

        if (player.move(Movement::RIGHT, dx, dy)) {
            return isWallCell(player.cellAt(Movement::LEFT).val) 
                && !map.isVisitedFrom(player.x, player.y, oppositeDirection(player.d));
        }

        if (player.move(Movement::BACKWARD, dx, dy)) {
            if (isWallCell(player.cellAt(Movement::LEFT).val)) {
                return !map.isVisitedFrom(player.x, player.y, oppositeDirection(player.d));
            }

            int stub;
            return player.move(Movement::LEFT, stub, stub) 
                && !map.isVisitedFrom(player.x, player.y, oppositeDirection(player.d));
        }

        throw runtime_error("Unreachable");
    }
};

const char *filename = "state.dat";

void serialize(const Solver &solver, const Player &player, const Map &map) {
    ofstream file(filename, ofstream::trunc);
    if (!file) {
        throw runtime_error("Cannot open file for writing");
    }

    file << int(solver.state) << " " << solver.step << endl;
    file << player.x << " " << player.y << " " << player.d << endl;
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
 
void deserialize(Solver &solver, Player &player, Map &map) {
    ifstream file(filename);
    if (!file) {
        return;
    }
    file.exceptions(ifstream::badbit | ifstream::failbit);

    int solverState, d;
    file >> solverState >> solver.step >> player.x >> player.y >> d;
    solver.state = deserializeSolverState(solverState);
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
    case Direction::NORTH: return angle0;
    case Direction::OST: return angle90;
    case Direction::SOUTH: return angle180;
    case Direction::WEST: return angle270;
    default: throw runtime_error("Unreachable");
    }
}

int main() {
    int playerId; // ignored
    cin >> playerId;
    cin.ignore();

    Map map; 
    Player player(map);
    Solver solver;

    // read map & last player pos & direction from the file (if any)
    deserialize(solver, player, map);

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
    if (Angle::angle0 != angle) {
        area = area.rotate(angle);
    }

    // map += area
    map.merge(player.y - 1, player.x - 1, area);

    // do move 
    //  - change player pos and direction
    //  - mark current cell as visited
    //  - cout movement 
    cout << movementToString(solver.move(player, map)) << endl;

    // save to the file
    serialize(solver, player, map);
}
