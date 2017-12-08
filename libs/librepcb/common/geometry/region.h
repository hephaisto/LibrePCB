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

#ifndef LIBREPCB_REGION_H
#define LIBREPCB_REGION_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "path.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Class Region
 ****************************************************************************************/

/**
 * @brief The Region class
 */
class Region final
{
    public:

        // Constructors / Destructor
        Region() noexcept :
            mOutline(), mHoles() {}
        Region(const Region& other) noexcept :
            mOutline(other.mOutline), mHoles(other.mHoles) {}
        Region(const Path& outline) noexcept :
            mOutline(outline), mHoles() {}
        ~Region() noexcept {}

        // Getters
        Path& getOutline() noexcept {return mOutline;}
        const Path& getOutline() const noexcept {return mOutline;}
        QList<Path>& getHoles() noexcept {return mHoles;}
        const QList<Path>& getHoles() const noexcept {return mHoles;}
        Path toSinglePathWithCutIns() const noexcept;
        ClipperLib::Paths toClipperPaths() const noexcept;
        QPainterPath toQPainterPathPx() const noexcept;

        // General Methods
        void setOutline(const Path& outline) noexcept {mOutline = outline;}
        void addHole(const Path& hole) noexcept {mHoles.push_back(hole);}
        Region& translate(const Point& delta) noexcept;
        Region& rotate(const Angle& angle) noexcept;
        QList<Region> offset(const Length& offset) const noexcept;

        // Operator Overloadings
        Region& operator=(const Region& rhs) noexcept;

        // Static Methods
        static QList<Region> fromClipper(const ClipperLib::PolyNode& node) noexcept;
        static QPainterPath toQPainterPathPx(const QList<Region>& regions) noexcept;


    private: // Data
        Path mOutline;
        QList<Path> mHoles;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_REGION_H
