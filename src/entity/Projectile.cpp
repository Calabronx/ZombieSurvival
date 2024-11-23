#include "Projectile.h"
#include "../entity/data/DataTables.h"
#include "../util/FileSystem.h"
#include "../util/Utility.h"
#include "../util/Category.h"
#include "../graphics/EmitterNode.h"

#include<vector>

namespace
{
    const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
    : Entity(1)
    , mType(type)
    , mSprite(textures.get(Table[type].texture), Table[type].textureRect)
{
    centerOrigin(mSprite);

    /*std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Smoke));
    smoke->setPosition(0.f, getBoundingRect().height / 2.f);
    attachChild(std::move(smoke));
    
    std::unique_ptr<EmitterNode> fire(new EmitterNode(Particle::Fire));
    fire->setPosition(0.f, getBoundingRect().height / 2.f);
    attachChild(std::move(fire));*/


}

unsigned int Projectile::getCategory() const
{
    return Category::Projectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return Table[mType].speed;
}

int Projectile::getDamage()
{
    return Table[mType].damage;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
