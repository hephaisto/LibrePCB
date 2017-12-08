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

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "region.h"
#include "regiontopathconverter.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Getters
 ****************************************************************************************/

Path Region::toSinglePathWithCutIns() const noexcept
{
    RegionToPathConverter conv(*this);
    return conv.convert();
}

ClipperLib::Paths Region::toClipperPaths() const noexcept
{
    ClipperLib::Paths p;
    p.push_back(mOutline.toClipperPath());
    for (const Path& hole : mHoles) {
        p.push_back(hole.toClipperPath());
    }
    return p;
}

QPainterPath Region::toQPainterPathPx() const noexcept
{
    QPainterPath p = mOutline.toQPainterPathPx();
    p.setFillRule(Qt::OddEvenFill); // important to subtract holes!
    for (const Path& hole : mHoles) {
        p.addPath(hole.toQPainterPathPx());
    }
    return p;
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

Region& Region::translate(const Point& delta) noexcept
{
    mOutline.translate(delta);
    for (Path& hole : mHoles) {
        hole.translate(delta);
    }
    return *this;
}

Region& Region::rotate(const Angle& angle) noexcept
{
    mOutline.rotate(angle);
    for (Path& hole : mHoles) {
        hole.rotate(angle);
    }
    return *this;
}

QList<Region> Region::offset(const Length& offset) const noexcept
{
    ClipperLib::ClipperOffset o(2.0, 5000); // max. 5000nm deviation from perfect arc
    o.AddPaths(toClipperPaths(), ClipperLib::jtRound, ClipperLib::etClosedPolygon);

    ClipperLib::PolyTree solution;
    o.Execute(solution, offset.toNm());
    return fromClipper(solution);
}

/*****************************************************************************************
 *  Operator Overloadings
 ****************************************************************************************/

Region& Region::operator=(const Region& rhs) noexcept
{
    mOutline = rhs.mOutline;
    mHoles = rhs.mHoles;
    return *this;
}

/*****************************************************************************************
 *  Static Methods
 ****************************************************************************************/

QList<Region> Region::fromClipper(const ClipperLib::PolyNode& node) noexcept
{
    QList<Region> regions;
    for (const ClipperLib::PolyNode* outline : node.Childs) { Q_ASSERT(outline);
        Q_ASSERT(!outline->IsHole());
        Region r(Path::fromClipper(outline->Contour));
        for (const ClipperLib::PolyNode* hole : outline->Childs) { Q_ASSERT(hole);
            Q_ASSERT(hole->IsHole());
            r.addHole(Path::fromClipper(hole->Contour));
            regions.append(fromClipper(*hole));
        }
        regions.append(r);
    }
    return regions;
}

QPainterPath Region::toQPainterPathPx(const QList<Region>& regions) noexcept
{
    QPainterPath p;
    foreach (const Region& r, regions) {
        p.addPath(r.toQPainterPathPx());
    }
    return p;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb
