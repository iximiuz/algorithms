#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#define BG_RED(s) "\033[1;41m" + s + "\033[0m"
#define BG_WHITE(s) "\033[1;47m" + s + "\033[0m"
#define FG_RED(s) std::string("\033[1;31m") + s + std::string("\033[0m")
#define FG_GREEN(s) std::string("\033[1;32m") + s + std::string("\033[0m")
#define FG_YELLOW(s) std::string("\033[1;33m") + s + std::string("\033[0m")

struct Cell {
    int value;
    bool unveiled;

    Cell(int value = 0, bool unveiled = false): value(value), unveiled(unveiled) {}
};

template<typename T>
class Matrix {
private:
    int rows_;
    int cols_;
    std::vector<T> data_;

public:
    Matrix(int rows = 0, int cols = 0) {
        resize(rows, cols);        
    }

    void resize(int rows, int cols) {
        rows_ = rows;
        cols_ = cols;
        data_.resize(rows * cols);
    }

    T& at(int row, int col) {
        return data_.at(cols_ * row + col);
    }

    int rows() const {
        return rows_;
    }

    int cols() const {
        return cols_;
    }
};

class State {
public:
    typedef std::shared_ptr<State> PState;

    virtual void render() = 0;
    virtual PState handleUserInput() = 0;
    virtual ~State() {}
};

class StateFinal: public State {
private:
    Matrix<Cell> field_;
    bool win_;

public:
    StateFinal(Matrix<Cell> field, bool win): field_(field), win_(win) {}

    void render() override {
        for (int r = 0; r < field_.rows(); r++) {
            for (int c = 0; c < field_.cols(); c++) {
                auto cell = field_.at(r, c);
                auto symb = std::to_string(cell.value);
                std::cout << (9 == cell.value ? BG_RED(symb) : symb) 
                          << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl 
                  << "              " << (win_? FG_GREEN("Win!") : FG_RED("Game Over")) 
                  << "              " << std::endl << std::endl
                  << "       <press enter to continue>       " << std::endl;
    }

    PState handleUserInput() override;
};

class StateMain: public State, public std::enable_shared_from_this<StateMain> {
private:
    Matrix<Cell> field_;
    int cursorRow_;
    int cursorCol_;
    int nUnveiled_;

public:
    StateMain(int rows, int cols, int nMines): cursorRow_(0), cursorCol_(0), nUnveiled_(0) {
        field_.resize(rows, cols);

        int value = 9;
        if (nMines > rows * cols / 2) {
            for (int r = 0; r < field_.rows(); r++) {
                for (int c = 0; c < field_.cols(); c++) {
                    field_.at(r, c).value = 9;
                }
            }

            value = 0;
        }

        while (nMines) {
            int r = rand() % field_.rows();
            int c = rand() % field_.cols();
            auto &cell = field_.at(r, c);

            if (cell.value != value) {
                cell.value = value;
                nMines--;
                std::cout << "SET v=" << value << " row=" << r << " col=" << c << std::endl;
            }
        }

        for (int r = 0; r < field_.rows(); r++) {
            for (int c = 0; c < field_.cols(); c++) {
                auto &cell = field_.at(r, c);
                if (cell.value != 9) {
                    cell.value = calcAdjacentMinesCount(r, c);
                }
            }
        }
    }

    void render() override {
        for (int r = 0; r < field_.rows(); r++) {
            for (int c = 0; c < field_.cols(); c++) {
                auto &cell = field_.at(r, c);
                auto symb = cell.unveiled ? std::to_string(cell.value) : "-";
                if (r == cursorRow_ && c == cursorCol_) {
                    symb = BG_WHITE(symb);
                }
                std::cout << symb << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl << "Your command: ";
    }

    PState handleUserInput() override {
        std::string command;
        std::getline(std::cin, command);
    
        switch (tolower(command[0])) {
        case 'w':
            cursorRow_ = std::max(0, cursorRow_ - 1);
            break;

        case 'a':
            cursorCol_ = std::max(0, cursorCol_ - 1);
            break;

        case 's':
            cursorRow_ = std::min(field_.rows() - 1, cursorRow_ + 1);
            break;

        case 'd':
            cursorCol_ = std::min(field_.cols() - 1, cursorCol_ + 1);
            break;

        case ' ':
            return handleUnveil();
            break;
        }
        
        return shared_from_this();
    }

private:
    int calcAdjacentMinesCount(int row, int col) {
        int count = 0;

        for (int dr = -1; dr < 2; dr++) {
            for (int dc = -1; dc < 2; dc++) {
                if (0 == dr && 0 == dc) {
                    continue;
                }

                int r = row + dr;
                int c = col + dc;

                if (0 <= r && r < field_.rows() && 0 <= c && c < field_.cols()) {
                    count += field_.at(r, c).value == 9;
                }
            }
        }
        
        return count;
    }

    PState handleUnveil() {
        auto &cell = field_.at(cursorRow_, cursorCol_);
        if (cell.unveiled) {
            return shared_from_this();
        }

        if (9 == cell.value) {
            return std::make_shared<StateFinal>(field_, false);
        }

        unveilCell(cursorRow_, cursorCol_);

        if (nUnveiled_ == field_.cols() * field_.rows()) {
            return std::make_shared<StateFinal>(field_, true);
        } 

        return shared_from_this();
    }

    void unveilCell(int row, int col) {
        auto &cell = field_.at(row, col);
        assert(!cell.unveiled);

        cell.unveiled = true;
        nUnveiled_++;

        if (0 != cell.value) {
            return;
        }

        for (int dr = -1; dr < 2; dr++) {
            for (int dc = -1; dc < 2; dc++) {
                if (0 == dr && 0 == dc) {
                    continue;
                }
                
                int r = row + dr;
                int c = col + dc;

                if (0 <= c && c < field_.cols() && 0 <= r && r < field_.rows()) {
                    if (!field_.at(r, c).unveiled) {
                        unveilCell(r, c);
                    }
                }
            }
        }
    }
};

class StateSettings: public State, public std::enable_shared_from_this<StateSettings> {
private:
    int rows_;
    int cols_;
    std::string warn_;

public:
    StateSettings(): rows_(0), cols_(0), warn_("") {}

    void render() override {
        renderWarningIfNeeded();

        if (rows_ <= 0) {
            std::cout << FG_GREEN("Enter number of rows: ") << std::endl;
            return;
        }
        if (cols_ <= 0) {
            std::cout << FG_GREEN("Enter number of columns: ") << std::endl;
            return;
        }
        std::cout << FG_GREEN("Enter number of mines: ") << std::endl;
    }

    PState handleUserInput() override {
        if (rows_ <= 0) {
            return handleInputRows();
        }
        if (cols_ <= 0) {
            return handleInputCols();
        }
        return handleInputMines();
    }

private:
    void renderWarningIfNeeded() {
        if (!warn_.empty()) {
            std::cout << FG_RED(warn_) << std::endl;
            warn_ = "";
        }
    }

    PState handleInputRows() {
        rows_ = handleInputNumber();
        return shared_from_this();
    }

    PState handleInputCols() {
        cols_ = handleInputNumber();
        return shared_from_this();
    }

    PState handleInputMines() {
        int mines = handleInputNumber();
        if (0 < mines && mines <= rows_ * cols_) {
            return std::make_shared<StateMain>(rows_, cols_, mines);
        }

        warn_ = "WARN: value must be positive integer less than " + 
                std::to_string(rows_ * cols_ + 1);
        return shared_from_this();
    }

    int handleInputNumber() {
        int v = 0;
        if (!(std::cin >> v && v > 0)) {
            warn_ = "WARN: value must be positive integer";
            std::cin.clear();
            std::cin.ignore();
        }
        return v;
    }
};

State::PState StateFinal::handleUserInput() {
    std::string l;
    std::getline(std::cin, l);
    return std::make_shared<StateSettings>();
}

class StateWelcome: public State, public std::enable_shared_from_this<StateWelcome> {
public:
    void render() override {
        std::cout << "                   Control:                   " << std::endl
                  << " W - move cursor up                           " << std::endl
                  << " A - move cursor left                         " << std::endl
                  << " S - move cursor down                         " << std::endl
                  << " D - move cursor right                        " << std::endl
                  << " <space> - unveil cell                        " << std::endl
                  << "                                              " << std::endl
                  << FG_GREEN("        <press enter to continue>        ") << std::endl;

    }

    PState handleUserInput() override {
        std::string l;
        std::getline(std::cin, l);
        return std::make_shared<StateSettings>();
    }
};

class Game {
private:
    State::PState state_;

public:
    Game() {
        state_ = std::make_shared<StateWelcome>();
    }

    void render() {
        std::system("clear");
        std::cout << FG_YELLOW("Minesweeper 0.1.0 <iximiuz@gmail.com> (c) 2018") 
                  << std::endl << std::endl;
        state_->render();
    }

    void handleUserInput() {
        state_ = state_->handleUserInput();
    }
};

int main() {
    srand(time(nullptr));
    Game g;
    while (true) {
        g.render();
        g.handleUserInput();
    }
}

