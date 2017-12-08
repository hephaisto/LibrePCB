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
#include "regiontopathconverter.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

Path RegionToPathConverter::convert() const noexcept
{
    // TODO: This only works with regions which do not have any arcs!

    // sort all holes by x coordinate of their left most point
    QList<Path> sortedHoles = mRegion.getHoles();
    qSort(sortedHoles.begin(), sortedHoles.end(),
          [](const Path& p1, const Path& p2)
          {return getLeftMostPoint(p1).getX() < getLeftMostPoint(p2).getX();});

    // add holes as cut-ins to the outline
    Path p = mRegion.getOutline();
    foreach (const Path& hole, sortedHoles) {
        addCutInToPath(p, hole);
    }
    return p;
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

void RegionToPathConverter::addCutInToPath(Path& outline, const Path& hole) noexcept
{
    int position = getOrCreateConnectionPoints(outline, hole);
    QList<Vertex> vertices = reorderedHoleVertices(hole);
    for (int i = 0; i < vertices.count(); ++i) {
        outline.getVertices().insert(position + i, vertices.at(i));
    }
}

int RegionToPathConverter::getOrCreateConnectionPoints(Path& outline, const Path& hole) noexcept
{
    Point p = getLeftMostPoint(hole);
    int nearestIndex = -1;
    Point nearestPos;
    for (int i = 0; i < outline.getVertices().count() - 1; ++i) {
        const Point& p1 = outline.getVertices().at(i).getPos();
        const Point& p2 = outline.getVertices().at(i + 1).getPos();
        if ((p1.getY() <= p.getY() && p2.getY() >= p.getY()) ||
            (p1.getY() >= p.getY() && p2.getY() <= p.getY()))
        {
            Point pm = Point((p1.getX() + p2.getX()) / 2, p.getY()); // TODO!!!
            if ((pm.getX() < p.getX()) && ((nearestIndex < 0) || (pm.getX() > nearestPos.getX()))) {
                nearestIndex = i;
                nearestPos = pm;
            }
        }
    }
    outline.insertVertex(nearestIndex, nearestPos);
    outline.insertVertex(nearestIndex, nearestPos);
    return nearestIndex + 1;
}

QList<Vertex> RegionToPathConverter::reorderedHoleVertices(const Path& hole) noexcept
{
    QList<Vertex> vertices = hole.getVertices();
    vertices.removeLast();
    while (vertices.first().getPos() != getLeftMostPoint(hole)) {
        vertices.append(vertices.takeFirst());
    }
    vertices.append(vertices.first());
    return vertices;
}

Point RegionToPathConverter::getLeftMostPoint(const Path& path) noexcept
{
    if (path.getVertices().count() > 0) {
        Point p = path.getVertices().first().getPos();
        foreach (const Vertex& v, path.getVertices()) {
            if (v.getPos().getX() < p.getX()) {
                p = v.getPos();
            }
        }
        return p;
    } else {
        return Point();
    }
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb
