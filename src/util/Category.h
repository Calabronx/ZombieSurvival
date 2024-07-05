#pragma once
namespace Category
{
    enum Type
    {
        None            = 0,
        Scene           = 1 << 0,
        PlayerSurvivor  = 1 << 1,
        Zombie          = 1 << 3
    };
}