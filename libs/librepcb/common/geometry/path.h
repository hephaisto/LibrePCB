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

#ifndef LIBREPCB_PATH_H
#define LIBREPCB_PATH_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <clipper/clipper.hpp>
#include "vertex.h"

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

/*****************************************************************************************
 *  Class Path
 ****************************************************************************************/

/**
 * @brief The Path class represents a list of vertices connected by straight lines or
 *        circular arcs
 *
 * This class is similar to a polygon or polyline, but it doesn't have properties like
 * width or layer. It's only a list of coordinates which are connected together by traight
 * lines or circular arcs.
 */
class Path final : public SerializableObject
{
    public:

        // Constructors / Destructor
        Path() noexcept : mVertices() {}
        Path(const Path& other) noexcept : mVertices(other.mVertices) {}
        explicit Path(const SExpression& node);
        ~Path() noexcept {}

        // Getters
        bool isClosed() const noexcept;
        QList<Vertex>& getVertices() noexcept {return mVertices;}
        const QList<Vertex>& getVertices() const noexcept {return mVertices;}
        ClipperLib::Path toClipperPath() const noexcept;
        QPainterPath toQPainterPathPx() const noexcept;

        // Transformations
        Path& translate(const Point& offset) noexcept;
        Path translated(const Point& offset) const noexcept;
        Path& rotate(const Angle& angle, const Point& center = Point(0, 0)) noexcept;
        Path rotated(const Angle& angle, const Point& center = Point(0, 0)) const noexcept;
        QList<Path> offset(const Length& offset) const noexcept;

        // General Methods
        void addVertex(const Point& pos, const Angle& angle = Angle::deg0()) noexcept;
        void insertVertex(int index, const Point& pos, const Angle& angle = Angle::deg0()) noexcept;
        bool close() noexcept;

        /// @copydoc librepcb::SerializableObject::serialize()
        void serialize(SExpression& root) const override;

        // Operator Overloadings
        bool operator==(const Path& rhs) const noexcept {return mVertices == rhs.mVertices;}
        bool operator!=(const Path& rhs) const noexcept {return !(*this == rhs);}
        Path& operator=(const Path& rhs) noexcept {mVertices = rhs.mVertices; return *this;}

        // Static Methods
        static ClipperLib::IntPoint toClipperPoint(const Point& p) noexcept;
        static Path fromClipper(const ClipperLib::Path& path) noexcept;
        static Path circle(const Length& diameter) noexcept;
        static Path obround(const Length& width, const Length& height) noexcept;
        static Path rect(const Length& width, const Length& height) noexcept;
        static Path octagon(const Length& width, const Length& height) noexcept;
        static Path line(const Point& p1, const Point& p2, const Length& width) noexcept;
        static Path flatArc(const Point& p1, const Point& p2, const Angle& angle, qreal tolerance) noexcept;
        static QPainterPath toQPainterPathPx(const QList<Path>& paths) noexcept;


    private: // Data
        QList<Vertex> mVertices;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace librepcb

#endif // LIBREPCB_PATH_H
