#include <SFML/Graphics.hpp>

int main() {
    // Explicitly create VideoMode with unsigned integers
    sf::RenderWindow window(sf::VideoMode(800u, 600u), "SFML Test");
    sf::CircleShape shape(50.f);
    shape.setFillColor(sf::Color::Green);

    // Event handling fix
    while (window.isOpen()) {
        sf::Event event{}; // Initialize event properly
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}