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

#ifndef LIBREPCB_PROJECT_BOARDPLANEFRAGMENTSBUILDER_H
#define LIBREPCB_PROJECT_BOARDPLANEFRAGMENTSBUILDER_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <librepcb/common/geometry/region.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {
namespace project {

class BI_Plane;
class BI_FootprintPad;

/*****************************************************************************************
 *  Class BoardPlaneFragmentsBuilder
 ****************************************************************************************/

/**
 * @brief The BoardPlaneFragmentsBuilder class
 */
class BoardPlaneFragmentsBuilder final
{
    public:

        // Constructors / Destructor
        BoardPlaneFragmentsBuilder() = delete;
        BoardPlaneFragmentsBuilder(const BoardPlaneFragmentsBuilder& other) = delete;
        BoardPlaneFragmentsBuilder(BI_Plane& plane) noexcept;
        ~BoardPlaneFragmentsBuilder() noexcept;

        // General Methods
        QList<Region> calculateRegions() const noexcept;

        // Operator Overloadings
        BoardPlaneFragmentsBuilder& operator=(const BoardPlaneFragmentsBuilder& rhs) = delete;


    private: // Methods
        ClipperLib::Paths createPadCutOuts(const BI_FootprintPad& pad) const noexcept;


    private: // Data
        BI_Plane& mPlane;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project
} // namespace librepcb

#endif // LIBREPCB_PROJECT_BOARDPLANEFRAGMENTSBUILDER_H
