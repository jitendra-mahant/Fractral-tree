#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

// -----------------------------
// Base Class: Graphics Window
// -----------------------------
class GraphicsWindow {
protected:
    sf::RenderWindow window;

public:
    GraphicsWindow(int width = 800, int height = 600, string title = "🌳 Fractal Tree Generator") {
        window.create(sf::VideoMode(width, height), title);
        window.setFramerateLimit(60);
    }

    virtual ~GraphicsWindow() {
        window.close();
    }
};

// -----------------------------
// Derived Class: Fractal Tree
// -----------------------------
class FractalTree : public GraphicsWindow {
private:
    float delay; // delay for animation

    // Recursive function to draw animated tree
    void drawAnimatedTree(sf::RenderWindow &win, float x1, float y1, float angle, int depth, int maxDepth) {
        if (depth == 0)
            return;

        float branchLength = depth * 10.0f;
        float x2 = x1 + cos(angle * M_PI / 180.0f) * branchLength;
        float y2 = y1 - sin(angle * M_PI / 180.0f) * branchLength;

        // Gradient color based on depth
        sf::Color branchColor(
            50 + (maxDepth - depth) * 15,
            200 - (maxDepth - depth) * 15,
            50 + (depth * 10)
        );

        // Draw branch
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), branchColor),
            sf::Vertex(sf::Vector2f(x2, y2), branchColor)
        };
        win.draw(line, 2, sf::Lines);
        win.display();

        // Delay for animation
        std::this_thread::sleep_for(std::chrono::milliseconds((int)delay));

        // Recursive calls
        drawAnimatedTree(win, x2, y2, angle - 25, depth - 1, maxDepth);
        drawAnimatedTree(win, x2, y2, angle + 25, depth - 1, maxDepth);
    }

public:
    FractalTree(float animDelay = 50.0f) : GraphicsWindow(), delay(animDelay) {}

    void drawAnimated(int depth) {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Black);

            float startX = 400;
            float startY = 550;

            // Gentle sway motion over time
            float sway = sin((float)clock() / 300.0f) * 10.0f;

            drawAnimatedTree(window, startX, startY, 90 + sway, depth, depth);

            window.display();

            // Clear and redraw for looping animation
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }
    }
};

// -----------------------------
// Main Function
// -----------------------------
int main() {
    int depth;
    cout << "Enter recursion depth (8–12 for best visuals): ";
    cin >> depth;

    cout << "Enter animation speed in ms (e.g., 30 for fast, 80 for slow): ";
    float speed;
    cin >> speed;

    FractalTree tree(speed);
    tree.drawAnimated(depth);

    return 0;
}

