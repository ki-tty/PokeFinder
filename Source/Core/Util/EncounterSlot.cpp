/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "EncounterSlot.hpp"
#include <Core/Enum/Encounter.hpp>
#include <array>
#include <cstddef>

/**
 * @brief Calculates the encounter slot from the \p ranges using the \p compare value
 *
 * @tparam size Number of entries in \p ranges
 * @tparam greater Whether to compare >= or <
 * @param rand PRNG call
 * @param ranges Table PRNG range values
 *
 * @return Encounter slot
 */
template <size_t size, bool greater = false>
static u8 calcSlot(u8 rand, const std::array<u8, size> &ranges)
{
    for (size_t i = 0; i < size; i++)
    {
        if constexpr (greater)
        {
            if (rand >= ranges[i])
            {
                return i;
            }
        }
        else
        {
            if (rand < ranges[i])
            {
                return i;
            }
        }
    }
    return 255;
}

namespace EncounterSlot
{
    u8 hSlot(u8 rand, Encounter encounter)
    {
        switch (encounter)
        {
        case Encounter::OldRod:
            return calcSlot<2>(rand, std::array<u8, 2> { 70, 100 });
        case Encounter::GoodRod:
            return calcSlot<3>(rand, std::array<u8, 3> { 60, 80, 100 });
        case Encounter::SuperRod:
            return calcSlot<5>(rand, std::array<u8, 5> { 40, 80, 95, 99, 100 });
        case Encounter::Surfing:
        case Encounter::RockSmash:
            return calcSlot<5>(rand, std::array<u8, 5> { 60, 90, 95, 99, 100 });
        default:
            return calcSlot<12>(rand, std::array<u8, 12> { 20, 40, 50, 60, 70, 80, 85, 90, 94, 98, 99, 100 });
        }
    }

    u8 jSlot(u8 rand, Encounter encounter)
    {
        switch (encounter)
        {
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            return calcSlot<5>(rand, std::array<u8, 5> { 40, 80, 95, 99, 100 });
        case Encounter::OldRod:
        case Encounter::Surfing:
            return calcSlot<5>(rand, std::array<u8, 5> { 60, 90, 95, 99, 100 });
        default:
            return calcSlot<12>(rand, std::array<u8, 12> { 20, 40, 50, 60, 70, 80, 85, 90, 94, 98, 99, 100 });
        }
    }

    u8 kSlot(u8 rand, Encounter encounter)
    {
        switch (encounter)
        {
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            return calcSlot<5>(rand, std::array<u8, 5> { 40, 70, 85, 95, 100 });
        case Encounter::Surfing:
            return calcSlot<5>(rand, std::array<u8, 5> { 60, 90, 95, 99, 100 });
        case Encounter::BugCatchingContest:
            return calcSlot<10, true>(rand, std::array<u8, 10> { 80, 60, 50, 40, 30, 20, 15, 10, 5, 0 });
        case Encounter::Headbutt:
        case Encounter::HeadbuttAlt:
        case Encounter::HeadbuttSpecial:
            return calcSlot<6>(rand, std::array<u8, 6> { 50, 65, 80, 90, 95, 100 });
        case Encounter::RockSmash:
            return calcSlot<2>(rand, std::array<u8, 2> { 80, 100 });
        default:
            return calcSlot<12>(rand, std::array<u8, 12> { 20, 40, 50, 60, 70, 80, 85, 90, 94, 98, 99, 100 });
        }
    }

    u8 bdspSlot(u8 rand, Encounter encounter)
    {
        switch (encounter)
        {
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            return calcSlot<5>(rand, std::array<u8, 5> { 40, 80, 95, 99, 100 });
        case Encounter::OldRod:
        case Encounter::Surfing:
            return calcSlot<5>(rand, std::array<u8, 5> { 60, 90, 95, 99, 100 });
        default:
            return calcSlot<12>(rand, std::array<u8, 12> { 20, 40, 50, 60, 70, 80, 85, 90, 94, 98, 99, 100 });
        }
    }
}
