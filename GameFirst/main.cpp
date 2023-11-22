#include <SFML/Graphics.hpp>

enum class Player { None, X, O, R };

class TicTacToe {
public:
    TicTacToe() : actionsNumber(0), currentPlayer(Player::X), winner(Player::None)
    {
        resetBoard();
        initWindow(); // window sets
    }

    void run()
    {
        while (window.isOpen())
        {
            processEvents();
            update();
            render();
        }
    }

private:
    const int windowSize = 600;
    const int gridSize = 10;
    const int enoughToWin = 5;
    const int cellSize = windowSize / gridSize;
    int actionsNumber = 0;

    Player currentPlayer;
    Player winner;
    std::vector<std::vector<Player>> board;

    sf::RenderWindow window;

    void resetBoard() /* vector^2 : (None) */ {
        board = std::vector<std::vector<Player>>(gridSize, std::vector<Player>(gridSize, Player::None));
    }

    void initWindow() {
        window.create(sf::VideoMode(windowSize, windowSize), "Tic-Tac-Toe-Too");
        window.setFramerateLimit(60);
    }

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y); // created
                }
            }
        }
    }

    void handleMouseClick(int mouseX, int mouseY) {
        if (winner != Player::None || actionsNumber >= gridSize * gridSize) {
            resetBoard();
            actionsNumber = 0;
            winner = Player::None;
            return;
        }

        int row = mouseY / cellSize;
        int col = mouseX / cellSize;

        if (row >= 0 && row < gridSize && col >= 0 && col < gridSize && board[row][col] == Player::None) {
            board[row][col] = currentPlayer;
            checkForWinner(row, col); //*
            currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
        }

        actionsNumber++;
    }

    void checkForWinner(int lastMoveRow, int lastMoveCol) {
        
        // Check row
        for (int i = 0; i < enoughToWin + 1; i++) // for each set of n cells in row
        {
            for (int j = i; j < i + enoughToWin; j++) // check if enough in row are the same
            {
                if (board[lastMoveRow][j - 1] == board[lastMoveRow][j]) {
                    winner = board[lastMoveRow][j];
                    return;
                }
            }
        }
        // Check column
        for (int i = 0; i < enoughToWin + 1; i++) // for each set of n cells in column
        {
            for (int j = i; j < i + enoughToWin; j++) // check if enough in column are the same
            {
                if (board[j - 1][lastMoveCol] == board[j][lastMoveCol]) {
                    winner = board[j][lastMoveCol];
                    return;
                }
            }
        }
        const int startCol = lastMoveCol < enoughToWin ? 0 : lastMoveCol - enoughToWin;
        const int startRow = lastMoveRow < enoughToWin ? 0 : lastMoveRow - enoughToWin;
        const int endCol = lastMoveCol + enoughToWin  > gridSize ? gridSize : lastMoveCol + enoughToWin;
        const int endRow = lastMoveRow + enoughToWin > gridSize ? gridSize : lastMoveRow + enoughToWin;
        const int difRows = endRow - startRow;
        const int difCols = endCol - startCol;
        const int difference = difRows < difCols ? difRows - enoughToWin : difCols - enoughToWin;
        if (difCols >= enoughToWin && difRows >= enoughToWin && lastMoveRow + lastMoveCol == gridSize - 1)
        {
            // Check diagonals: top-left -> bottom-right
            for (int i = 0; i < difference; i++) // for each set of n cells in diagonal
            {
                for (int j = 0; j < enoughToWin - 1; j++) // check if enough in column are the same
                {
                    if (board[startRow + i + j][startCol + i + j] == board[j][lastMoveCol]) {
                        winner = board[j][lastMoveCol];
                        return;
                    }
                }
            }

        }
        // Check diagonals
        if ((lastMoveRow == lastMoveCol || lastMoveRow + lastMoveCol == gridSize - 1) &&
            ((board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
                (board[0][2] == board[1][1] && board[1][1] == board[2][0]))) {
            winner = board[1][1];
        }
    }

    void update() {
        // Update game logic here if needed
    }

    void render() {
        window.clear();

        // Draw grid
        for (int i = 1; i < gridSize; ++i) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(i * cellSize, 0)),
                sf::Vertex(sf::Vector2f(i * cellSize, windowSize)),
                sf::Vertex(sf::Vector2f(0, i * cellSize)),
                sf::Vertex(sf::Vector2f(windowSize, i * cellSize))
            };
            window.draw(line, 4, sf::Lines);
        }

        // Draw X and O
        for (int row = 0; row < gridSize; ++row) {
            for (int col = 0; col < gridSize; ++col) {
                if (board[row][col] == Player::X) {
                    drawX(row, col);
                }
                else if (board[row][col] == Player::O) {
                    drawO(row, col);
                }
            }
        }

        // Draw winner message
        if (winner != Player::None || actionsNumber >= 9) {
            static sf::Font font;
            bool bFontLoaded = false;
            if (!bFontLoaded)
            {
                font.loadFromFile("C:\\Users\\User\\OneDrive\\Робочий стіл\\OOP\\Tanks-1\\Arial.ttf"); // Change the font file path as needed
                bFontLoaded = true;
            }

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            sf::String Answer = (winner == Player::X ? "Player X" : "Player O") + std::string(" wins!\nClick to restart.");
            if (winner == Player::None)
            {
                Answer = "Draw!\nClick to restart.";
            }

            text.setString(Answer);
            text.setPosition(10, 10);
            window.clear();
            window.draw(text);
        }

        window.display();
    }

    void drawX(int row, int col) {
        sf::Vertex line1[] = {
            sf::Vertex(sf::Vector2f(col * cellSize + 10, row * cellSize + 10)),
            sf::Vertex(sf::Vector2f((col + 1) * cellSize - 10, (row + 1) * cellSize - 10))
        };
        sf::Vertex line2[] = {
            sf::Vertex(sf::Vector2f((col + 1) * cellSize - 10, row * cellSize + 10)),
            sf::Vertex(sf::Vector2f(col * cellSize + 10, (row + 1) * cellSize - 10))
        };

        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);
    }

    void drawO(int row, int col) {
        sf::CircleShape circle(cellSize / 2 - 10);
        circle.setPosition(col * cellSize + 10, row * cellSize + 10);
        circle.setOutlineThickness(10);
        circle.setOutlineColor(sf::Color::White);

        window.draw(circle);
    }
};

int main() {
    TicTacToe game;
    game.run();

    return 0;
}