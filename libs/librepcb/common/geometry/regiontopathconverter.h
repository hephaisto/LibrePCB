/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBREPCB_REGIONTOPATHCONVERTER_H
#define LIBREPCB_REGIONTOPATHCONVERTER_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "region.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Class RegionToPathConverter
 ****************************************************************************************/

/**
 * @brief The RegionToPathConverter class
 */
class RegionToPathConverter final
{
    public:

        // Constructors / Destructor
        RegionToPathConverter() = delete;
        RegionToPathConverter(const RegionToPathConverter& other) = delete;
        explicit RegionToPathConverter(const Region& region) noexcept : mRegion(region) {}
        ~RegionToPathConverter() noexcept {}

        // General Methods
        Path convert() const noexcept;

        // Operator Overloadings
        RegionToPathConverter& operator=(const RegionToPathConverter& rhs) noexcept;


    private: // Methods
        static void addCutInToPath(Path& outline, const Path& hole) noexcept;
        static int getOrCreateConnectionPoints(Path& outline, const Path& hole) noexcept;
        static QList<Vertex> reorderedHoleVertices(const Path& hole) noexcept;
        static Point getLeftMostPoint(const Path& path) noexcept;


    private: // Data
        const Region& mRegion;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_REGIONTOPATHCONVERTER_H
