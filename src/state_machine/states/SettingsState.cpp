#include "SettingsState.h"
#include "../../util/Utility.h"

SettingsState::SettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	addButtonLabel(InputHandler::MoveLeft, 300.f, "Move Left", context);
	addButtonLabel(InputHandler::MoveRight, 350.f, "Move Right", context);
	addButtonLabel(InputHandler::MoveUp, 400.f, "Move Up", context);
	addButtonLabel(InputHandler::MoveDown, 450.f, "Move Down", context);
	addButtonLabel(InputHandler::Fire, 500.f, "Fire", context);
	addButtonLabel(InputHandler::Reload, 550.f, "Reload", context);
	//hay mas acciones para setear

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(80.f, 620.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	mGUIContainer.pack(backButton);

}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time dt)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;
	for (std::size_t action = 0; action < InputHandler::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().inputPlayer->assignKey(static_cast<InputHandler::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}
	if (isKeyBinding)
		updateLabels();
	else
		mGUIContainer.handleEvent(event);
	return false;
}

void SettingsState::updateLabels()
{
	InputHandler& player = *getContext().inputPlayer;
	for (std::size_t i = 0; i < InputHandler::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<InputHandler::Action>(i));
		if (key != sf::Keyboard::Unknown)
			mBindingLabels[i]->setText(toString(key == 0 ? "A" : key == 3 ? "D" : key == 3 ? "D" : key == 22 ? "W" : key == 18 ? "S" : key == 17 ? "R" : ""));
	}
}

void SettingsState::addButtonLabel(InputHandler::Action action, float y, const std::string& text, Context context)
{
	mBindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mBindingButtons[action]->setPosition(80.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabels[action]->setPosition(300.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}
