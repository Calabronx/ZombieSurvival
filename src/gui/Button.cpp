#include "Button.h"
#include "../util/Utility.h"

GUI::Button::Button(const FontHolder& fonts, const TextureHolder& textures)
    : mCallback()
    , mNormalTexture(textures.get(Textures::ButtonNormal))
    , mSelectedTexture(textures.get(Textures::ButtonSelected))
    , mPressedTexture(textures.get(Textures::ButtonPressed))
    , mSprite()
    , mText("", fonts.get(Fonts::Main), 16)
    , mIsToggle(false)
{
    mSprite.setTexture(mNormalTexture);

    sf::FloatRect bounds = mSprite.getLocalBounds();
    mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

void GUI::Button::setCallback(Callback callback)
{
    mCallback = std::move(callback);
}

void GUI::Button::setText(const std::string& text)
{
    mText.setString(text);
    centerOrigin(mText);
}

void GUI::Button::setToggle(bool flag)
{
    mIsToggle = flag;
}

bool GUI::Button::isSelectable() const
{
    return true;
}

void GUI::Button::select()
{
    Component::select();
    mSprite.setTexture(mSelectedTexture);
}

void GUI::Button::deselect()
{
    Component::deselect();
    mSprite.setTexture(mNormalTexture);
}

void GUI::Button::activate()
{
    Component::activate();
    if (mIsToggle)
        mSprite.setTexture(mPressedTexture);
    if (mCallback)
        mCallback();
    if (!mIsToggle)
        deactivate();
}

void GUI::Button::deactivate()
{
    Component::deactivate();
    if (mIsToggle)
    {
        if (isSelected())
            mSprite.setTexture(mSelectedTexture);
        else
            mSprite.setTexture(mNormalTexture);
    }
}

void GUI::Button::handleEvent(const sf::Event& event)
{
}

void GUI::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
}
