#include "InputHandler.h"
#include <SFML/System/Vector2.hpp> 
#include "../entity/Character.h"
#include "../util/Category.h"

#include <iostream>

struct SurvivorMover
{
    SurvivorMover(float vx, float vy)
    : velocity(vx, vy)
    {
    }

    void operator() (Character& character, sf::Time) const
    {
        character.move(velocity);
    }

    sf::Vector2f velocity;
};

void InputHandler::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
    {
        Command output;
        output.category = Category::PlayerSurvivor;
        output.action = [](SceneNode& s, sf::Time)
        {
            std::cout << s.getPosition().x << "," << s.getPosition().y << "\n";
        };
        commands.push(output);
    }
}

void InputHandler::handleRealTimeInput(CommandQueue& commands)
{
    const float playerSpeed = 30.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        Command moveLeft;
        moveLeft.category = Category::PlayerSurvivor;
        moveLeft.action = derivedAction<Character>(SurvivorMover(-playerSpeed, 0.f));
        commands.push(moveLeft);
    }
}
