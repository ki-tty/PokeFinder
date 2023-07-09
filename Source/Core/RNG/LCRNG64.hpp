/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LCRNG64_HPP
#define LCRNG64_HPP

#include <Core/Global.hpp>

struct JumpTable64
{
    struct
    {
        u64 mult;
        u64 add;
    } jump[32];
};

extern const JumpTable64 BWRNGTable;
extern const JumpTable64 BWRNGRTable;

/**
 * @brief Provides random numbers via the LCRNG algorithm. Most commonly used ones are defined at the bottom of the file.
 *
 * @tparam add LCRNG64 addition value
 * @tparam mult LCRNG64 multiplication value
 */
template <u64 add, u64 mult>
class LCRNG64
{
public:
    /**
     * @brief Construct a new LCRNG64 object
     *
     * @param seed Starting PRNG state
     */
    LCRNG64(u64 seed) : seed(seed)
    {
    }

    /**
     * @brief Construct a new LCRNG64 object
     *
     * @param rng LCRNG object to copy
     */
    LCRNG64(const LCRNG64 &rng) : seed(rng.seed)
    {
    }

    /**
     * @brief Construct a new LCRNG64 object
     *
     * @param seed Starting PRNG state
     * @param advances Initial number of advances
     */
    LCRNG64(u64 seed, u32 advances) : seed(seed)
    {
        jump(advances);
    }

    /**
     * @brief Construct a new LCRNG64 object
     *
     * @param rng LCRNG object to copy
     * @param advances Initial number of advances
     */
    LCRNG64(const LCRNG64 &rng, u32 advances) : seed(rng.seed)
    {
        jump(advances);
    }

    /**
     * @brief Advances the RNG by \p advances amount
     * This function is called when the number of advances is predetermined and allows the compiler to optimize to the final mult/add
     *
     * @param advances Number of advances
     * @param count Pointer to keep track of advance count
     *
     * @return PRNG value after the advances
     */
    u64 advance(u32 advances, u32 *count = nullptr)
    {
        for (u64 advance = 0; advance < advances; advance++)
        {
            next(count);
        }
        return seed;
    }

    /**
     * @brief Returns the adder of the LCRNG
     *
     * @return LCRNG adder value
     */
    constexpr static u64 getAdd()
    {
        return add;
    }

    /**
     * @brief Returns the multipler of the LCRNG
     *
     * @return LCRNG multipler value
     */
    constexpr static u64 getMult()
    {
        return mult;
    }

    /**
     * @brief Returns the current PRNG state
     *
     * @return PRNG value
     */
    u64 getSeed() const
    {
        return seed;
    }

    /**
     * @brief Jumps the RNG by \p advances amount
     * This function uses a jump ahead table to advance any amount in just O(32)
     *
     * @param advances Number of advances
     *
     * @return PRNG value after the advances
     */
    u64 jump(u32 advances)
    {
        const JumpTable64 *table;
        if constexpr (add == 0x269ec3) // BWRNG
        {
            table = &BWRNGTable;
        }
        else // BWRNG(R)
        {
            static_assert(add == 0x9b1ae6e9a384e6f9, "Unsupported LCRNG64");
            table = &BWRNGRTable;
        }

        for (int i = 0; advances; advances >>= 1, i++)
        {
            if (advances & 1)
            {
                seed = seed * table->jump[i].mult + table->jump[i].add;
            }
        }

        return seed;
    }

    /**
     * @brief Gets the next 64bit PRNG state
     *
     * @param count Pointer to keep track of advance count
     *
     * @return PRNG value
     */
    u64 next(u32 *count = nullptr)
    {
        if (count)
        {
            (*count)++;
        }
        return seed = seed * mult + add;
    }

    /**
     * @brief Gets the next 32bit PRNG state
     *
     * @param count Pointer to keep track of advance count
     *
     * @return PRNG value
     */
    u32 nextUInt(u32 *count = nullptr)
    {
        return next(count) >> 32;
    }

    /**
     * @brief Gets the next 32bit PRNG state bounded by the \p max value
     *
     * @param max Max bounding value
     * @param count Pointer to keep track of advance count
     *
     * @return PRNG value
     */
    u32 nextUInt(u32 max, u32 *count = nullptr)
    {
        return ((next(count) >> 32) * max) >> 32;
    }

private:
    u64 seed;
};

using BWRNG = LCRNG64<0x269ec3, 0x5d588b656c078965>;
using BWRNGR = LCRNG64<0x9b1ae6e9a384e6f9, 0xdedcedae9638806d>;

#endif // LCRNG64_HPP
