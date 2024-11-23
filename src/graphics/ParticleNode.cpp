#include "ParticleNode.h"
#include "../entity/data/DataTables.h"
#include "../util/Category.h"


namespace {
    const std::vector<ParticleData> Table = initializeParticleData();
}

ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& textures)
    : SceneNode()
    , mParticles()
    , mTexture(textures.get(Textures::Particle))
    , mType(type)
    , mVertexArray(sf::Quads)
    , mNeedsVertexUpdate(true)
{
}

void ParticleNode::addParticle(sf::Vector2f position)
{
    Particle particle;
    particle.position = position;
    particle.color = Table[mType].color;
    particle.lifeTime = Table[mType].lifetime;

    mParticles.push_back(particle);
}

Particle::Type ParticleNode::getParticleType() const
{
    return mType;
}

sf::Time ParticleNode::getEmmisionTime() const
{
    return Table[mType].lifetime;
}

unsigned int ParticleNode::getCategory() const
{
    return Category::ParticleSystem;
}

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    // remove expired particles at the beginning
    while (!mParticles.empty() && mParticles.front().lifeTime <= sf::Time::Zero)
        mParticles.pop_front();

    // decrease lifetime of existing particles
    for (Particle& particle : mParticles)
        particle.lifeTime -= dt;

    mNeedsVertexUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mNeedsVertexUpdate)
    {
        computeVertices();
        mNeedsVertexUpdate = false;
    }

    states.texture = &mTexture;

    target.draw(mVertexArray, states);

}

void ParticleNode::addVertex(float worldX, float worldY, float textCoordX, float textCoordY, const sf::Color& color) const
{
    sf::Vertex vertex;
    vertex.position = sf::Vector2f(worldX, worldY);
    vertex.texCoords = sf::Vector2f(textCoordX, textCoordY);
    vertex.color = color;

    mVertexArray.append(vertex);
}

void ParticleNode::computeVertices() const
{
    sf::Vector2f size(mTexture.getSize());
    sf::Vector2f half = size / 2.f;

    mVertexArray.clear();

    for (const Particle& particle : mParticles)
    {
        sf::Vector2f pos = particle.position;
        sf::Color color = particle.color;
        float ratio = particle.lifeTime.asSeconds() / Table[mType].lifetime.asSeconds();
        color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

        addVertex(pos.x - half.x, pos.y - half.y, 0.f,      0.f, color);
        addVertex(pos.x + half.x, pos.y - half.y, size.x,   0.f, color);
        addVertex(pos.x + half.x, pos.y + half.y, size.x,   size.y, color);
        addVertex(pos.x - half.x, pos.y + half.y, 0.f,      size.y, color);

    }
}
