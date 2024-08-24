#pragma once
namespace Category
{
    enum Type
    {
        None                 = 0,
        SceneLandLayer       = 1 << 0,
        PlayerSurvivor       = 1 << 1,
        Zombie               = 1 << 3,
        Pickup                  = 1 << 4,
        SurvivorProjectile   = 1 << 5,
    };
}