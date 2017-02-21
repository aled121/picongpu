/* Copyright 2017 Rene Widera
 *
 * This file is part of PIConGPU.
 *
 * PIConGPU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PIConGPU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PIConGPU.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "simulation_defines.hpp"

#include "traits/HasIdentifier.hpp"

#include <memory>


namespace picongpu
{
namespace radiation
{

    /** read the `radiationMask` of a species */
    template< bool hasFilter >
    struct ExecuteParticleFilter
    {
        /** get the attribute value of `radiationMask`
         *
         * @param particle particle to be used
         * @return value of the attribute `radiationMask`
         */
        template< typename T_Species >
        void operator()( std::shared_ptr<T_Species> species, const uint32_t currentStep )
        {
            auto radiationEnableFunctor = picongpu::radiation::RadiationParticleFilter( currentStep );
            species->manipulateAllParticles(
                currentStep,
                radiationEnableFunctor
            );
        }
    };

    /** specialization
     *
     * specialization for the case that the species not owns the attribute
     * `radiationMask`
     */
    template< >
    struct ExecuteParticleFilter< false >
    {
        /** get the attribute value of `radiationMask`
         *
         * @param particle to be used
         * @return always true
         */
        template< typename T_Species >
        void operator()( const std::shared_ptr<T_Species>, const uint32_t currentStep )
        {
        }
    };

    /** execute the particle filter on a species
     *
     * There is no need to check if a particle filter is defined
     *
     * @tparam T_Species species type
     * @param species species to be filtered
     */
    template< typename T_Species >
    void executeParticleFilter( std::shared_ptr<T_Species>& species, const uint32_t currentStep )
    {
        constexpr bool hasRadiationFilter = ::PMacc::traits::HasIdentifier<
          typename T_Species::FrameType,
          radiationMask
        >::type::value;

        return ExecuteParticleFilter< hasRadiationFilter >{}( species, currentStep );
    }

} // namespace radiation
} // namespace picongpu
