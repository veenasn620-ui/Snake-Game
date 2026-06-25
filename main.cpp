#include <SFML/Graphics.hpp>
#include <deque>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int HUD_HEIGHT = 60;
const int CELL_SIZE = 20;

const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = (WINDOW_HEIGHT - HUD_HEIGHT) / CELL_SIZE;

const std::string HIGH_SCORE_FILE = "highscore.txt";

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

enum class GameState {
    Menu,
    Playing,
    Paused,
    GameOver
};

std::vector<std::string> getPattern(char c) {
    c = static_cast<char>(std::toupper(c));

    switch (c) {
        case '0': return {"11111","10001","10011","10101","11001","10001","11111"};
        case '1': return {"00100","01100","00100","00100","00100","00100","01110"};
        case '2': return {"11110","00001","00001","11110","10000","10000","11111"};
        case '3': return {"11110","00001","00001","01110","00001","00001","11110"};
        case '4': return {"10010","10010","10010","11111","00010","00010","00010"};
        case '5': return {"11111","10000","10000","11110","00001","00001","11110"};
        case '6': return {"01111","10000","10000","11110","10001","10001","01110"};
        case '7': return {"11111","00001","00010","00100","01000","01000","01000"};
        case '8': return {"01110","10001","10001","01110","10001","10001","01110"};
        case '9': return {"01110","10001","10001","01111","00001","00001","11110"};

        case 'A': return {"01110","10001","10001","11111","10001","10001","10001"};
        case 'B': return {"11110","10001","10001","11110","10001","10001","11110"};
        case 'C': return {"01111","10000","10000","10000","10000","10000","01111"};
        case 'D': return {"11110","10001","10001","10001","10001","10001","11110"};
        case 'E': return {"11111","10000","10000","11110","10000","10000","11111"};
        case 'F': return {"11111","10000","10000","11110","10000","10000","10000"};
        case 'G': return {"01111","10000","10000","10011","10001","10001","01111"};
        case 'H': return {"10001","10001","10001","11111","10001","10001","10001"};
        case 'I': return {"11111","00100","00100","00100","00100","00100","11111"};
        case 'J': return {"00111","00010","00010","00010","10010","10010","01100"};
        case 'K': return {"10001","10010","10100","11000","10100","10010","10001"};
        case 'L': return {"10000","10000","10000","10000","10000","10000","11111"};
        case 'M': return {"10001","11011","10101","10101","10001","10001","10001"};
        case 'N': return {"10001","11001","10101","10011","10001","10001","10001"};
        case 'O': return {"01110","10001","10001","10001","10001","10001","01110"};
        case 'P': return {"11110","10001","10001","11110","10000","10000","10000"};
        case 'Q': return {"01110","10001","10001","10001","10101","10010","01101"};
        case 'R': return {"11110","10001","10001","11110","10100","10010","10001"};
        case 'S': return {"01111","10000","10000","01110","00001","00001","11110"};
        case 'T': return {"11111","00100","00100","00100","00100","00100","00100"};
        case 'U': return {"10001","10001","10001","10001","10001","10001","01110"};
        case 'V': return {"10001","10001","10001","10001","10001","01010","00100"};
        case 'W': return {"10001","10001","10001","10101","10101","10101","01010"};
        case 'X': return {"10001","10001","01010","00100","01010","10001","10001"};
        case 'Y': return {"10001","10001","01010","00100","00100","00100","00100"};
        case 'Z': return {"11111","00001","00010","00100","01000","10000","11111"};

        case ':': return {"00000","00100","00100","00000","00100","00100","00000"};
        case '-': return {"00000","00000","00000","11111","00000","00000","00000"};
        case ' ': return {"00000","00000","00000","00000","00000","00000","00000"};

        default: return {"00000","00000","00000","00000","00000","00000","00000"};
    }
}

void drawText(
    sf::RenderWindow& window,
    const std::string& text,
    float x,
    float y,
    float scale,
    sf::Color color
) {
    sf::RectangleShape pixel(sf::Vector2f(scale, scale));
    pixel.setFillColor(color);

    float currentX = x;

    for (char c : text) {
        std::vector<std::string> pattern = getPattern(c);

        for (size_t row = 0; row < pattern.size(); row++) {
            for (size_t col = 0; col < pattern[row].size(); col++) {
                if (pattern[row][col] == '1') {
                    pixel.setPosition(
                        currentX + static_cast<float>(col) * scale,
                        y + static_cast<float>(row) * scale
                    );
                    window.draw(pixel);
                }
            }
        }

        currentX += 6 * scale;
    }
}

class Snake {
private:
    std::deque<sf::Vector2i> body;
    Direction direction;
    Direction nextDirection;

public:
    Snake() {
        reset();
    }

    void reset() {
        body.clear();

        int startX = GRID_WIDTH / 2;
        int startY = GRID_HEIGHT / 2;

        body.push_back(sf::Vector2i(startX, startY));
        body.push_back(sf::Vector2i(startX - 1, startY));
        body.push_back(sf::Vector2i(startX - 2, startY));

        direction = Direction::Right;
        nextDirection = Direction::Right;
    }

    void setDirection(Direction newDirection) {
        if (direction == Direction::Up && newDirection == Direction::Down) return;
        if (direction == Direction::Down && newDirection == Direction::Up) return;
        if (direction == Direction::Left && newDirection == Direction::Right) return;
        if (direction == Direction::Right && newDirection == Direction::Left) return;

        nextDirection = newDirection;
    }

    sf::Vector2i getHead() const {
        return body.front();
    }

    sf::Vector2i getNextHead() const {
        sf::Vector2i next = getHead();

        Direction moveDirection = nextDirection;

        if (moveDirection == Direction::Up) {
            next.y--;
        } else if (moveDirection == Direction::Down) {
            next.y++;
        } else if (moveDirection == Direction::Left) {
            next.x--;
        } else if (moveDirection == Direction::Right) {
            next.x++;
        }

        return next;
    }

    void move(bool grow) {
        direction = nextDirection;
        body.push_front(getNextHead());

        if (!grow) {
            body.pop_back();
        }
    }

    bool hitsSelf() const {
        sf::Vector2i head = body.front();

        for (size_t i = 1; i < body.size(); i++) {
            if (body[i] == head) {
                return true;
            }
        }

        return false;
    }

    bool occupies(const sf::Vector2i& position) const {
        for (const sf::Vector2i& part : body) {
            if (part == position) {
                return true;
            }
        }

        return false;
    }

    void render(sf::RenderWindow& window) const {
        for (size_t i = 0; i < body.size(); i++) {
            sf::RectangleShape segment(
                sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2)
            );

            if (i == 0) {
                segment.setFillColor(sf::Color(80, 255, 120));
            } else {
                segment.setFillColor(sf::Color(40, 180, 80));
            }

            segment.setPosition(
                static_cast<float>(body[i].x * CELL_SIZE + 1),
                static_cast<float>(HUD_HEIGHT + body[i].y * CELL_SIZE + 1)
            );

            window.draw(segment);
        }
    }
};

class Food {
private:
    sf::Vector2i position;

public:
    Food() : position(5, 5) {}

    sf::Vector2i getPosition() const {
        return position;
    }

    void respawn(const Snake& snake, std::mt19937& rng) {
        std::uniform_int_distribution<int> xDist(0, GRID_WIDTH - 1);
        std::uniform_int_distribution<int> yDist(0, GRID_HEIGHT - 1);

        int attempts = 0;

        do {
            position.x = xDist(rng);
            position.y = yDist(rng);
            attempts++;
        } while (snake.occupies(position) && attempts < 1000);
    }

    void render(sf::RenderWindow& window) const {
        sf::CircleShape foodShape(CELL_SIZE / 2.0f - 2);
        foodShape.setFillColor(sf::Color(255, 80, 80));

        foodShape.setPosition(
            static_cast<float>(position.x * CELL_SIZE + 2),
            static_cast<float>(HUD_HEIGHT + position.y * CELL_SIZE + 2)
        );

        window.draw(foodShape);
    }
};

class Game {
private:
    sf::RenderWindow window;
    Snake snake;
    Food food;

    GameState state;

    int score;
    int highScore;

    float moveTimer;
    float moveDelay;

    bool previousP;
    bool previousR;
    bool previousSpace;
    bool previousEnter;

    std::mt19937 rng;

public:
    Game()
        : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "C++ Snake Game"),
          state(GameState::Menu),
          score(0),
          highScore(0),
          moveTimer(0.0f),
          moveDelay(0.14f),
          previousP(false),
          previousR(false),
          previousSpace(false),
          previousEnter(false),
          rng(std::random_device{}()) {
        init();
    }

    void init() {
        if (!window.isOpen()) {
            std::cerr << "Error: Could not create game window." << std::endl;
            return;
        }

        window.setFramerateLimit(60);
        highScore = loadHighScore();
        resetGame();
        state = GameState::Menu;
    }

    int loadHighScore() {
        std::ifstream file(HIGH_SCORE_FILE);

        int value = 0;

        if (file >> value) {
            return value;
        }

        return 0;
    }

    void saveHighScore() {
        std::ofstream file(HIGH_SCORE_FILE);

        if (file) {
            file << highScore << std::endl;
        }
    }

    void resetGame() {
        snake.reset();
        food.respawn(snake, rng);
        score = 0;
        moveTimer = 0.0f;
        moveDelay = 0.14f;
    }

    void run() {
        sf::Clock clock;

        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();

            handleInput();
            update(deltaTime);
            render();
        }
    }

    void handleInput() {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        bool enterPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
        bool pPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
        bool rPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::R);

        if (state == GameState::Menu) {
            if ((spacePressed && !previousSpace) ||
                (enterPressed && !previousEnter)) {
                resetGame();
                state = GameState::Playing;
            }
        }

        if (state == GameState::Playing) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                snake.setDirection(Direction::Up);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                snake.setDirection(Direction::Down);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                snake.setDirection(Direction::Left);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                snake.setDirection(Direction::Right);
            }

            if (pPressed && !previousP) {
                state = GameState::Paused;
            }
        }
        else if (state == GameState::Paused) {
            if (pPressed && !previousP) {
                state = GameState::Playing;
            }
        }

        if (state == GameState::GameOver) {
            if (rPressed && !previousR) {
                resetGame();
                state = GameState::Playing;
            }

            if (spacePressed && !previousSpace) {
                state = GameState::Menu;
            }
        }

        previousSpace = spacePressed;
        previousEnter = enterPressed;
        previousP = pPressed;
        previousR = rPressed;
    }

    void update(float deltaTime) {
        if (state != GameState::Playing) {
            return;
        }

        moveTimer += deltaTime;

        if (moveTimer < moveDelay) {
            return;
        }

        moveTimer = 0.0f;

        sf::Vector2i nextHead = snake.getNextHead();

        bool hitWall =
            nextHead.x < 0 ||
            nextHead.x >= GRID_WIDTH ||
            nextHead.y < 0 ||
            nextHead.y >= GRID_HEIGHT;

        if (hitWall) {
            endGame();
            return;
        }

        bool eatingFood = nextHead == food.getPosition();

        snake.move(eatingFood);

        if (snake.hitsSelf()) {
            endGame();
            return;
        }

        if (eatingFood) {
            score += 10;

            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }

            food.respawn(snake, rng);

            if (moveDelay > 0.055f) {
                moveDelay -= 0.004f;
            }
        }

        std::stringstream title;
        title << "C++ Snake Game | Score: " << score
              << " | High Score: " << highScore;

        window.setTitle(title.str());
    }

    void endGame() {
        state = GameState::GameOver;

        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }
    }

    void renderPlayArea() {
        sf::RectangleShape hudBackground(
            sf::Vector2f(WINDOW_WIDTH, HUD_HEIGHT)
        );
        hudBackground.setFillColor(sf::Color(25, 25, 35));
        window.draw(hudBackground);

        sf::RectangleShape playArea(
            sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - HUD_HEIGHT)
        );
        playArea.setPosition(0, HUD_HEIGHT);
        playArea.setFillColor(sf::Color(15, 15, 20));
        window.draw(playArea);

        sf::RectangleShape border(
            sf::Vector2f(WINDOW_WIDTH - 2, WINDOW_HEIGHT - HUD_HEIGHT - 2)
        );
        border.setPosition(1, HUD_HEIGHT + 1);
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineThickness(2);
        border.setOutlineColor(sf::Color(90, 90, 100));
        window.draw(border);
    }

    void renderHud() {
        drawText(
            window,
            "SCORE:" + std::to_string(score),
            20,
            18,
            4,
            sf::Color(240, 240, 240)
        );

        drawText(
            window,
            "HIGH:" + std::to_string(highScore),
            500,
            18,
            4,
            sf::Color(255, 220, 90)
        );
    }

    void renderMenu() {
        drawText(window, "SNAKE GAME", 190, 180, 8, sf::Color(80, 255, 120));
        drawText(window, "PRESS SPACE TO START", 145, 300, 4, sf::Color(230, 230, 230));
        drawText(window, "ARROWS OR WASD TO MOVE", 135, 350, 4, sf::Color(180, 180, 180));
        drawText(window, "P TO PAUSE", 265, 400, 4, sf::Color(180, 180, 180));
        drawText(window, "ESC TO QUIT", 260, 450, 4, sf::Color(180, 180, 180));
    }

    void renderPaused() {
        sf::RectangleShape overlay(
            sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)
        );
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window.draw(overlay);

        drawText(window, "PAUSED", 280, 250, 8, sf::Color(255, 220, 90));
        drawText(window, "PRESS P TO RESUME", 180, 350, 4, sf::Color(230, 230, 230));
    }

    void renderGameOver() {
        sf::RectangleShape overlay(
            sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)
        );
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        drawText(window, "GAME OVER", 210, 220, 8, sf::Color(255, 80, 80));
        drawText(window, "SCORE:" + std::to_string(score), 285, 320, 5, sf::Color(240, 240, 240));
        drawText(window, "PRESS R TO RESTART", 170, 390, 4, sf::Color(230, 230, 230));
        drawText(window, "SPACE FOR MENU", 225, 440, 4, sf::Color(180, 180, 180));
    }

    void render() {
        window.clear(sf::Color(10, 10, 15));

        if (state == GameState::Menu) {
            renderMenu();
        } else {
            renderPlayArea();
            renderHud();
            food.render(window);
            snake.render(window);

            if (state == GameState::Paused) {
                renderPaused();
            }

            if (state == GameState::GameOver) {
                renderGameOver();
            }
        }

        window.display();
    }
};

int main() {
    Game game;
    game.run();

    return 0;
}