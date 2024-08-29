#include "GameOverState.h"

#include "../../util/FileSystem.h"
#include "../../util/Utility.h"
#include "../../input/InputHandler.h"


GameOverState::GameOverState(StateStack& stack, Context context)
    : State(stack, context)
    , mGameOverText()
    , mElapsedTime(sf::Time::Zero)
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f windowSize(context.window->getSize());

    mGameOverText.setFont(font);
    if (context.inputPlayer->getPlayerStatus() == InputHandler::Dead)
        mGameOverText.setString("You are dead");
    else
        mGameOverText.setString("You have survived!");

    mGameOverText.setCharacterSize(70);
    centerOrigin(mGameOverText);
    mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt)
{
    mElapsedTime += dt;
    if (mElapsedTime > sf::seconds(3))
    {
        requestStateClear();
        requestStackPush(States::Menu);
    }
    return false;
}

bool GameOverState::handleEvent(const sf::Event& event)
{
    return false;
}
