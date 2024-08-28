#include "Projectile.h"
#include "../entity/data/DataTables.h"
#include "../util/FileSystem.h"
#include "../util/Utility.h"
#include "../util/Category.h"

#include<vector>

namespace
{
    const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
    : Entity(1)
    , mType(type)
    , mSprite(textures.get(Table[type].texture))
{
    centerOrigin(mSprite);
}

unsigned int Projectile::getCategory() const
{
    //if (mType == HandgunBullet || mType == ShotgunBullet) // originalmente en el diseño, este if validaba que el tipo de projectil sea aliado o enemigo ( en un juego de aviones)
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
