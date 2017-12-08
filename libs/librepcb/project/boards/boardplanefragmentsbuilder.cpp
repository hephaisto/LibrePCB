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
#include "boardplanefragmentsbuilder.h"
#include <librepcb/common/geometry/polygon.h>
#include <librepcb/common/graphics/graphicslayer.h>
#include <librepcb/library/pkg/footprintpad.h>
#include "items/bi_plane.h"
#include "items/bi_device.h"
#include "items/bi_footprint.h"
#include "items/bi_footprintpad.h"
#include "items/bi_netsegment.h"
#include "items/bi_via.h"
#include "items/bi_netpoint.h"
#include "items/bi_netline.h"
#include "items/bi_polygon.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

BoardPlaneFragmentsBuilder::BoardPlaneFragmentsBuilder(BI_Plane& plane) noexcept :
    mPlane(plane)
{
}

BoardPlaneFragmentsBuilder::~BoardPlaneFragmentsBuilder() noexcept
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

QList<Region> BoardPlaneFragmentsBuilder::calculateRegions() const noexcept
{
    QList<Region> sameNetSignalAreas;

    // get outline
    ClipperLib::Clipper c;
    c.AddPath(mPlane.getOutline().toClipperPath(), ClipperLib::ptSubject, true);

    // subtract arbitrary polygons
    //foreach (const BI_Polygon* polygon, mPlane.getBoard().getPolygons()) {
    //    if (polygon->getPolygon().getLayerName() == GraphicsLayer::sBoardOutlines) {
    //        Path outline = polygon->getPolygon().toPath();
    //
    //    }
    //    //else if (polygon->getPolygon().getLayerName() is keepout layer of the plane) {
    //    //    TODO
    //    //}
    //}

    // subtract other planes
    foreach (const BI_Plane* plane, mPlane.getBoard().getPlanes()) {
        if (plane == &mPlane) continue;
        if (*plane < mPlane) continue; // ignore planes with lower priority
        if (plane->getLayerName() != mPlane.getLayerName()) continue;
        if (&plane->getNetSignal() == &mPlane.getNetSignal()) continue;
        foreach (const Region& fragment, plane->getFragments()) {
            Path simplified = fragment.toSinglePathWithCutIns();
            for (const Path& path : simplified.offset(mPlane.getMinClearance())) {
                c.AddPath(path.toClipperPath(), ClipperLib::ptClip, true);
            }
        }
    }

    // subtract pads
    foreach (const BI_Device* device, mPlane.getBoard().getDeviceInstances()) {
        foreach (const BI_FootprintPad* pad, device->getFootprint().getPads()) {
            if (!pad->isOnLayer(mPlane.getLayerName())) continue;
            if (pad->getCompSigInstNetSignal() == &mPlane.getNetSignal()) {
                sameNetSignalAreas.append(pad->toSceneRegion());
            }
            c.AddPaths(createPadCutOuts(*pad), ClipperLib::ptClip, true);
        }
    }

    // subtract net segment items
    foreach (const BI_NetSegment* netsegment, mPlane.getBoard().getNetSegments()) {

        // subtract vias
        foreach (const BI_Via* via, netsegment->getVias()) {
            if (&netsegment->getNetSignal() == &mPlane.getNetSignal()) {
                sameNetSignalAreas.append(via->toSceneRegion());
            } else {
                for (const Path& path : via->toSceneOutline().offset(mPlane.getMinClearance())) {
                    c.AddPath(path.toClipperPath(), ClipperLib::ptClip, true);
                }
            }
        }

        // subtract netlines
        foreach (const BI_NetLine* netline, netsegment->getNetLines()) {
            if (netline->getLayer().getName() == mPlane.getLayerName()) {
                if (&netsegment->getNetSignal() == &mPlane.getNetSignal()) {
                    Path path = Path::line(netline->getStartPoint().getPosition(),
                                           netline->getEndPoint().getPosition(),
                                           netline->getWidth());
                    sameNetSignalAreas.append(Region(path));
                } else {
                    Path path = Path::line(netline->getStartPoint().getPosition(),
                                           netline->getEndPoint().getPosition(),
                                           netline->getWidth() + (mPlane.getMinClearance() * 2));
                    c.AddPath(path.toClipperPath(), ClipperLib::ptClip, true);
                }
            }
        }
    }

    ClipperLib::Paths p1;
    c.Execute(ClipperLib::ctDifference, p1, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
    //ClipperLib::SimplifyPolygons(p1);
    //ClipperLib::CleanPolygons(p1);

    // clip to board outline (with additional clearance)
    ClipperLib::Paths p1_5;
    QList<Region> boardArea = mPlane.getBoard().getArea().offset(-mPlane.getMinClearance());
    if (boardArea.count() == 1) {
        ClipperLib::Clipper clBoardOutline;
        clBoardOutline.AddPaths(p1, ClipperLib::ptSubject, true);
        clBoardOutline.AddPaths(boardArea.first().toClipperPaths(), ClipperLib::ptClip, true);
        clBoardOutline.Execute(ClipperLib::ctIntersection, p1_5, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
    } else {
        p1_5 = p1;
    }

    ClipperLib::Paths p2;
    ClipperLib::ClipperOffset o2(2.0, 5000); // max. 5000nm deviation from perfect arc
    o2.AddPaths(p1_5, ClipperLib::jtRound, ClipperLib::etClosedPolygon);
    o2.Execute(p2, (mPlane.getMinWidth() / -2).toNm());
    //ClipperLib::SimplifyPolygons(p2);
    //ClipperLib::CleanPolygons(p2);

    ClipperLib::PolyTree p3;
    ClipperLib::ClipperOffset o3(2.0, 5000); // max. 5000nm deviation from perfect arc
    o3.AddPaths(p2, ClipperLib::jtRound, ClipperLib::etClosedPolygon);
    o3.Execute(p3, (mPlane.getMinWidth() / 2).toNm());
    //ClipperLib::SimplifyPolygons(p3);
    //ClipperLib::CleanPolygons(p3);

    QList<Region> fragments = Region::fromClipper(p3);

    // remove orphans if required
    if (!mPlane.getKeepOrphans()) {
        QPainterPath sameNetSignalAreasPx;
        foreach (const Region& r, sameNetSignalAreas) {
            sameNetSignalAreasPx.addPath(r.toQPainterPathPx());
        }
        for (int i = fragments.size() - 1; i >= 0; --i) {
            if (!fragments.at(i).toQPainterPathPx().intersects(sameNetSignalAreasPx)) {
                fragments.removeAt(i);
            }
        }
    }

    return fragments;
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

ClipperLib::Paths BoardPlaneFragmentsBuilder::createPadCutOuts(const BI_FootprintPad& pad) const noexcept
{
    ClipperLib::Paths paths;
    if (pad.getCompSigInstNetSignal() == &mPlane.getNetSignal()) {
        switch (mPlane.getConnectStyle()) {
            case BI_Plane::ConnectStyle::None: {
                for (const Path& path : pad.toSceneOutline().offset(mPlane.getMinClearance())) {
                    paths.push_back(path.toClipperPath());
                }
                break;
            }
            case BI_Plane::ConnectStyle::Thermal: {
                Length spokeWidth = qMax(mPlane.getThermalSpokeWidth(),
                                         mPlane.getMinWidth() + Length(10));
                Path h = Path::rect(spokeWidth, pad.getLibPad().getHeight() * 5).translate(pad.getPosition());
                Path v = Path::rect(pad.getLibPad().getWidth() * 5, spokeWidth).translate(pad.getPosition());
                for (const Path& path : pad.toSceneOutline().offset(mPlane.getThermalGapWidth())) {
                    ClipperLib::Paths thermals;
                    ClipperLib::Clipper c;
                    c.AddPath(path.toClipperPath(), ClipperLib::ptSubject, true);
                    c.AddPath(h.toClipperPath(), ClipperLib::ptClip, true);
                    c.AddPath(v.toClipperPath(), ClipperLib::ptClip, true);
                    c.Execute(ClipperLib::ctDifference, thermals, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
                    for (ClipperLib::Path& p : thermals) {
                        paths.push_back(p);
                    }
                }
                break;
            }
            case BI_Plane::ConnectStyle::Solid:
                // do not remove anything, the pad is completely covered by the plane
                break;
            default:
                Q_ASSERT(false);
                break;
        }
    } else {
        for (const Path& path : pad.toSceneOutline().offset(mPlane.getMinClearance())) {
            paths.push_back(path.toClipperPath());
        }
    }

    for (ClipperLib::Path& p : paths) {
        if (ClipperLib::Orientation(p)) {
            ClipperLib::ReversePath(p);
        }
    }

    return paths;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project
} // namespace librepcb
