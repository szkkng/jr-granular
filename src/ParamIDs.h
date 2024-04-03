/*
  ==============================================================================

   Copyright 2022 Suzuki Kengo

   JR-Granular is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   JR-Granular is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with JR-Granular. If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/

#pragma once

namespace ParamIDs
{

// Be the same value as the "paramId" listed in the rnbo description.json.
inline constexpr auto mix { "mix" };
inline constexpr auto gain { "gain" };
inline constexpr auto grainPos { "grainPos" };
inline constexpr auto grainSize { "grainSize" };
inline constexpr auto interval { "interval" };
inline constexpr auto pitch { "pitch" };
inline constexpr auto width { "width" };

} // namespace ParamIDs
