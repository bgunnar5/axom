// Copyright (c) 2017-2024, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

/*! \file primal_introduction.cpp
 *  \brief This example code is a demonstration of Axom's primal component.
 *
 *  This file shows how to use Primal to represent geometric primitives, perform
 *  geometric operations, and use a spatial index.  Running the executable from
 *  this file will produce a collection of Asymptote source files.  When
 *  compiled, the Asymptote files produce the figures that accompany primal's
 *  Sphinx documentation.
 */

/* This example code contains snippets used in the Primal Sphinx documentation.
 * They begin and end with comments such as
 *
 * prims_header_start
 * prims_header_end
 * clip_header_start
 * clip_header_end
 * closest_point_header_start
 * closest_point_header_end
 *
 * each prepended with an underscore.
 */

#include "axom/config.hpp"

// _prims_header_start
// Axom primitives
#include "axom/primal/geometry/BoundingBox.hpp"
#include "axom/primal/geometry/OrientedBoundingBox.hpp"
#include "axom/primal/geometry/Point.hpp"
#include "axom/primal/geometry/Polygon.hpp"
#include "axom/primal/geometry/Ray.hpp"
#include "axom/primal/geometry/Segment.hpp"
#include "axom/primal/geometry/Triangle.hpp"
#include "axom/primal/geometry/Vector.hpp"
// _prims_header_end

// Axom operations
// Each header is used in its own example, so each one is bracketed for separate
// inclusion.
// _clip_header_start
#include "axom/primal/operators/clip.hpp"
// _clip_header_end
// _closest_point_header_start
#include "axom/primal/operators/closest_point.hpp"
// _closest_point_header_end
// _bbox_header_start
#include "axom/primal/operators/compute_bounding_box.hpp"
// _bbox_header_end
// _intersect_header_start
#include "axom/primal/operators/intersect.hpp"
// _intersect_header_end
// _orient_header_start
#include "axom/primal/operators/orientation.hpp"
// _orient_header_end
// _sqdist_header_start
#include "axom/primal/operators/squared_distance.hpp"
// _sqdist_header_end

// C++ headers
#include <cmath>  // do we need this?
#include <iostream>
#include <fstream>

#include "axom/fmt.hpp"

// _using_start
// "using" directives to simplify code
namespace primal = axom::primal;

// almost all our examples are in 3D
constexpr int in3D = 3;

// primitives represented by doubles in 3D
using PointType = primal::Point<double, in3D>;
using TriangleType = primal::Triangle<double, in3D>;
using BoundingBoxType = primal::BoundingBox<double, in3D>;
using OrientedBoundingBoxType = primal::OrientedBoundingBox<double, in3D>;
using PolygonType = primal::Polygon<double, in3D>;
using RayType = primal::Ray<double, in3D>;
using SegmentType = primal::Segment<double, in3D>;
using VectorType = primal::Vector<double, in3D>;
// _using_end

std::string asyheader =
  "// Autogenerated from examples/primal_introduction_ex\n\n"
  "// To turn this Asymptote source file into an image for inclusion in\n"
  "// Axom's documentation,\n"
  "// 1. run Asymptote:\n"
  "//    asy -f png {0}\n"
  "// 2. Optionally, use ImageMagick to convert the white background to "
  "transparent:\n"
  "//    convert {1} -transparent white {1}\n\n"
  "// preamble\n"
  "settings.render = 6;\n"
  "import three;\n"
  "size(6cm, 0);\n\n";

std::string printPoint(PointType pt)
{
  return axom::fmt::format("({},{},{})", pt[0], pt[1], pt[2]);
}

std::string printPoint(double* pt)
{
  return axom::fmt::format("({},{},{})", pt[0], pt[1], pt[2]);
}

void writeToFile(std::string fname, std::string contents)
{
  std::ofstream outfile(fname);
  if(!outfile.good())
  {
    std::cout << "Could not write to " << fname << std::endl;
  }
  else
  {
    outfile << contents;
  }
}

PolygonType showClip()
{
  // _clip_start
  TriangleType tri(PointType {1.2, 0, 0},
                   PointType {0, 1.8, 0},
                   PointType {0, 0, 1.4});

  BoundingBoxType bbox(PointType {0, -0.5, 0}, PointType {1, 1, 1});

  PolygonType poly = clip(tri, bbox);
  // _clip_end

  std::cout << "----- showClip -----" << std::endl;
  std::cout << "clipping triangle " << tri << " with bounding box " << bbox
            << " gives polygon " << poly << std::endl
            << std::endl;

  // Now write out an Asymptote file showing what we did.
  std::string basefname = "showClip";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";

  std::string polygon;
  for(int i = 0; i < poly.numVertices(); ++i)
  {
    polygon += printPoint(poly[i]);
    polygon += "--";
  }
  polygon += "cycle";

  std::string contents = axom::fmt::format(
    "{0}"
    "// axes\n"
    "draw(O -- 1.7X, arrow=Arrow3(DefaultHead2), "
    "L=Label(\"$x$\", position=EndPoint, align=W));\n"
    "draw(O -- 2.4Y, arrow=Arrow3(), "
    "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 2Z, arrow=Arrow3(), "
    "L=Label(\"$z$\", position=EndPoint));\n\n"

    "// polygon\n"
    "path3 pgon = {1};\n\n"

    "// triangle\n"
    "path3 tri = {2}--{3}--{4}--cycle;\n\n"

    "// draw triangle then polygon\n"
    "draw(surface(tri), surfacepen=blue+opacity(0.4));\n"
    "draw(tri);\n\n"
    "draw(surface(pgon), surfacepen=yellow+opacity(0.4));\n"
    "draw(pgon, yellow);\n\n"

    "// bounding box\n"
    "draw(box({5}, {6}));\n",
    axom::fmt::format(asyheader, fname, ifname),
    polygon,
    printPoint(tri[0]),
    printPoint(tri[1]),
    printPoint(tri[2]),
    printPoint(bbox.getMin()),
    printPoint(bbox.getMax()));

  writeToFile(fname, contents);

  return poly;
}

void showClosestPoint()
{
  // _closest_point_start
  TriangleType tri(PointType {1, 0, 0}, PointType {0, 1, 0}, PointType {0, 0, 1});

  PointType pto = PointType {0, 0, 0};
  PointType pta = PointType {-1, 2, 1};

  // Query point o lies at the origin.  Its closest point lies in the
  // interior of tri.
  PointType cpto = closest_point(pto, tri);

  // Query point a lies farther from the triangle.  Its closest point
  // is on tri's edge.
  int lcpta = 0;
  PointType cpta = closest_point(pta, tri, &lcpta);
  // _closest_point_end

  std::cout << "----- showClosestPoint -----" << std::endl;
  std::cout << "For triangle " << tri << "," << std::endl
            << "point closest to " << pto << " is " << cpto << std::endl
            << "point closest to " << pta << " is " << cpta << std::endl
            << std::endl;

  // Now write out an Asymptote file showing what we did.
  // Projected points
  PointType ppta = PointType {pta[0], pta[1], 0.};
  PointType pcpta = PointType {cpta[0], cpta[1], 0.};
  PointType pcpto = PointType {cpto[0], cpto[1], 0.};
  std::string basefname = "showClosestPoint";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";

  std::string contents = axom::fmt::format(
    "{0}"
    "// axes\n"
    "draw(-4.5X -- 1.7X, arrow=Arrow3(DefaultHead2), "
    "L=Label(\"$x$\", position=EndPoint, align=W));\n"
    "draw(O -- 2.4Y, arrow=Arrow3(), "
    "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 2Z, arrow=Arrow3(), "
    "L=Label(\"$z$\", position=EndPoint));\n\n"

    "// triangle\n"
    "path3 tri = {1}--{2}--{3}--cycle;\n\n"

    "// triangle\n"
    "triple pto = {4};\n"
    "triple pta = {5};\n"
    "triple cpto = {6};\n"
    "triple cpta = {7};\n"
    "triple ppta = {8};\n"
    "triple pcpto = {9};\n"
    "triple pcpta = {10};\n\n"

    "// draw triangle then points and projections\n"
    "draw(tri);\n"
    "dot(pto, blue);\n"
    "label(\"$o$\", pto, align=W);\n"
    "dot(cpto, mediumblue);\n"
    "label(\"$o'$\", cpto, align=N);\n"
    "draw(cpto--pcpto, dotted);\n"
    "dot(pta, lightolive);\n"
    "label(\"$a$\", pta, align=W);\n"
    "draw(pta--ppta, dotted);\n"
    "dot(cpta, yellow);\n"
    "label(\"$a'$\", cpta, align=NE);\n"
    "draw(cpta--pcpta, dotted);\n",
    axom::fmt::format(asyheader, fname, ifname),
    printPoint(tri[0]),
    printPoint(tri[1]),
    printPoint(tri[2]),
    printPoint(pto),
    printPoint(pta),
    printPoint(cpto),
    printPoint(cpta),
    printPoint(ppta),
    printPoint(pcpto),
    printPoint(pcpta));

  writeToFile(fname, contents);
}

void showBoundingBoxes()
{
  // _bbox_start
  // An array of Points to include in the bounding boxes
  const int nbr_points = 6;
  PointType data[nbr_points] = {PointType {0.6, 1.2, 1.0},
                                PointType {1.3, 1.6, 1.8},
                                PointType {2.9, 2.4, 2.3},
                                PointType {3.2, 3.5, 3.0},
                                PointType {3.6, 3.2, 4.0},
                                PointType {4.3, 4.3, 4.5}};

  // A BoundingBox constructor takes an array of Point objects
  BoundingBoxType bbox(data, nbr_points);
  // Make an OrientedBoundingBox
  OrientedBoundingBoxType obbox = compute_oriented_bounding_box(data, nbr_points);
  // _bbox_end

  std::cout << "----- showBoundingBoxes -----" << std::endl;
  std::cout << "For the " << nbr_points << " points:" << std::endl;
  for(int i = 0; i < nbr_points; ++i)
  {
    std::cout << data[i] << std::endl;
  }
  std::cout << "(Axis-aligned) bounding box is " << bbox << std::endl
            << "oriented bounding box is " << obbox << std::endl;

  // Now write out an Asymptote file showing what we did.
  std::string pointses, dotses;
  for(int i = 0; i < nbr_points; ++i)
  {
    pointses += axom::fmt::format("points[{0}] = {1};\n", i, printPoint(data[i]));
    dotses += axom::fmt::format("dot(points[{0}], blue);\n", i);
  }

  std::vector<PointType> obpts = obbox.vertices();
  std::string obboxpts;
  for(int i = 0; i < 8; ++i)
  {
    obboxpts += axom::fmt::format("obpts[{0}] = {1};\n", i, printPoint(obpts[i]));
  }

  std::string basefname = "showBoundingBoxes";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";
  std::string contents = axom::fmt::format(
    "{0}"
    "// projection\n"
    "currentprojection = perspective((4, -1.8, 3), (0.07, 0.07, 1));\n\n"

    "// axes\n"
    "draw(O -- 4X, arrow=Arrow3(DefaultHead2), "
    "L=Label(\"$x$\", position=EndPoint));\n"
    "draw(O -- 7Y, arrow=Arrow3(), "
    "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 5Z, arrow=Arrow3(), "
    "L=Label(\"$z$\", position=EndPoint));\n\n"

    "// points\n"
    "triple[] points = new triple[6];\n"
    "{1}\n"

    "// bbox\n"
    "triple bboxmin = {2};\n"
    "triple bboxmax = {3};\n\n"

    "// oriented bounding box\n"
    "triple[] obpts = new triple[8];\n"
    "{4}\n"

    "// draw points\n"
    "{5}\n"

    "// draw bbox\n"
    "draw(box(bboxmin, bboxmax));\n\n"

    "// draw oriented bounding box\n"
    "path3[] obboxpath = obpts[0]--obpts[1]--obpts[3]--obpts[2]--cycle\n"
    "     ^^ obpts[4]--obpts[5]--obpts[7]--obpts[6]--cycle\n"
    "     ^^ obpts[0]--obpts[4] ^^ obpts[1]--obpts[5]\n"
    "     ^^ obpts[2]--obpts[6] ^^ obpts[3]--obpts[7];\n"
    "draw(obboxpath, orange);\n\n",
    axom::fmt::format(asyheader, fname, ifname),
    pointses,
    printPoint(bbox.getMin()),
    printPoint(bbox.getMax()),
    obboxpts,
    dotses);

  writeToFile(fname, contents);
}

void showIntersect()
{
  std::cout << "----- showIntersect -----" << std::endl;

  // _intersect_start
  // Two triangles
  TriangleType tri1(PointType {1.2, 0, 0},
                    PointType {0, 1.8, 0},
                    PointType {0, 0, 1.4});

  TriangleType tri2(PointType {0, 0, 0.5},
                    PointType {0.8, 0.1, 1.2},
                    PointType {0.8, 1.4, 1.2});

  // tri1 and tri2 should intersect
  if(intersect(tri1, tri2))
  {
    std::cout << "Triangles intersect as expected." << std::endl;
  }
  else
  {
    std::cout << "There's an error somewhere..." << std::endl;
  }

  // A vertical ray constructed from origin and point
  RayType ray(SegmentType(PointType {0.4, 0.4, 0}, PointType {0.4, 0.4, 1}));

  // t will hold the intersection point between ray and tri1,
  // as parameterized along ray.
  double rt1t = 0;
  // rt1b will hold the intersection point barycentric coordinates,
  // and rt1p will hold the physical coordinates.
  PointType rt1b, rt1p;

  // The ray should intersect tri1 and tri2.
  if(intersect(tri1, ray, rt1t, rt1b) && intersect(tri2, ray))
  {
    // Retrieve the physical coordinates from barycentric coordinates
    rt1p = tri1.baryToPhysical(rt1b);
    // Retrieve the physical coordinates from ray parameter
    PointType rt1p2 = ray.at(rt1t);
    std::cout << "Ray intersects tri1 as expected.  Parameter t: " << rt1t
              << std::endl
              << "  Intersection point along ray: " << rt1p2 << std::endl
              << "  Intersect barycentric coordinates: " << rt1b << std::endl
              << "  Intersect physical coordinates: " << rt1p << std::endl
              << "Ray also intersects tri2 as expected." << std::endl;
  }
  else
  {
    std::cout << "There's an error somewhere..." << std::endl;
  }

  // A bounding box
  BoundingBoxType bbox(PointType {0.1, -0.23, 0.1}, PointType {0.8, 0.5, 0.4});

  // The bounding box should intersect tri1 and ray but not tr2.
  PointType bbtr1;
  if(intersect(ray, bbox, bbtr1) && intersect(tri1, bbox) &&
     !intersect(tri2, bbox))
  {
    std::cout << "As hoped, bounding box intersects tri1 at " << bbtr1
              << " and ray, but not tri2." << std::endl;
  }
  else
  {
    std::cout << "There is at least one error somewhere..." << std::endl;
  }
  // _intersect_end

  // helper variables
  PolygonType poly = clip(tri1, bbox);
  // These are parametric coordinates ray-tri2-t, tri1-tri2 leg A-t,
  // tri1-tri2 leg C-t
  // and corresponding physical points.
  double rt2t, t1t2at, t1t2ct;
  (void)intersect(tri2, ray, rt2t);
  PointType rt2p = ray.at(rt2t);
  SegmentType t2lega(tri2[0], tri2[1]);
  (void)intersect(tri1, t2lega, t1t2at);
  PointType t1t2ap = t2lega.at(t1t2at);
  SegmentType t2legc(tri2[2], tri2[0]);
  (void)intersect(tri1, t2legc, t1t2ct);
  PointType t1t2cp = t2legc.at(t1t2ct);
  // Project point C of tri2 onto the XY plane
  PointType tr2c = tri2[2];
  PointType pp = PointType::make_point(tr2c[0], tr2c[1], 0.);

  // Now write out an Asymptote file showing what we did.
  std::string basefname = "showIntersect";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";

  std::string polygon;
  for(int i = 0; i < poly.numVertices(); ++i)
  {
    polygon += printPoint(poly[i]);
    polygon += "--";
  }
  polygon += "cycle";

  std::string contents = axom::fmt::format(
    "{0}"
    "// axes\n"
    "draw(O -- 1.7X, arrow=Arrow3(DefaultHead2), "
    "L=Label(\"$x$\", position=EndPoint));\n"
    "draw(O -- 2.4Y, arrow=Arrow3(), "
    "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 2Z, arrow=Arrow3(), "
    "L=Label(\"$z$\", position=EndPoint, align=W));\n\n"

    "// triangle 1\n"
    "path3 tri1 = {1}--{2}--{3}--cycle;\n\n"

    "// triangle 2\n"
    "path3 tri2 = {4}--{5}--{6}--cycle;\n\n"

    "// ray\n"
    "path3 ray = {7}--{8};\n\n"

    "// polygon of intersection between bbox and triangle\n"
    "path3 pgon = {9};\n\n"

    "// draw bounding box and other geometry\n"
    "draw(box({10}, {11}), blue);\n"
    "draw(pgon, deepblue);\n\n"
    "draw(ray, arrow=Arrow3(DefaultHead2), red);\n"
    "  dot({12}, red);\n  dot({13}, red);\n  dot({14}, red);\n"
    "  draw(tri1);\n  draw(tri2, blue);\n"
    "draw({15}--{16}, deepblue);\n"
    "draw({17}--{18}, dotted);\n",
    axom::fmt::format(asyheader, fname, ifname),
    printPoint(tri1[0]),
    printPoint(tri1[1]),
    printPoint(tri1[2]),
    printPoint(tri2[0]),
    printPoint(tri2[1]),
    printPoint(tri2[2]),
    printPoint(ray.origin()),
    printPoint(ray.at(1.8)),
    polygon,
    printPoint(bbox.getMin()),
    printPoint(bbox.getMax()),
    printPoint(bbtr1),
    printPoint(rt1p),
    printPoint(rt2p),
    printPoint(t1t2ap),
    printPoint(t1t2cp),
    printPoint(tr2c),
    printPoint(pp));

  writeToFile(fname, contents);
}

void showOrientation()
{
  std::cout << "----- showOrientation -----" << std::endl;

  // _orient_start
  // A triangle
  TriangleType tri(PointType {1.2, 0, 0},
                   PointType {0, 1.8, 0},
                   PointType {0, 0, 1.4});

  // Three points:
  //    one on the triangle's positive side,
  PointType pos = PointType {0.45, 1.5, 1};
  //    one coplanar to the triangle, the centroid,
  PointType cpl =
    PointType::lerp(PointType::lerp(tri[0], tri[1], 0.5), tri[2], 1. / 3.);
  //    and one on the negative side
  PointType neg = PointType {0, 0, 0.7};

  // Test orientation
  if(orientation(pos, tri) == primal::ON_POSITIVE_SIDE &&
     orientation(cpl, tri) == primal::ON_BOUNDARY &&
     orientation(neg, tri) == primal::ON_NEGATIVE_SIDE)
  {
    std::cout << "As expected, point pos is on the positive side," << std::endl
              << "    point cpl is on the boundary (on the triangle),"
              << std::endl
              << "    and point neg is on the negative side." << std::endl;
  }
  else
  {
    std::cout << "Someone wrote this wrong." << std::endl;
  }
  // _orient_end

  // Helper variables
  // Project onto the XY plane
  PointType ppos {pos[0], pos[1], 0.};
  //PointType pcpl = PointType::make_point(cpl[0], cpl[1], 0.);

  // Now write out an Asymptote file showing what we did.
  std::string basefname = "showOrientation";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";

  std::string contents = axom::fmt::format(
    "{0}"
    "// axes\n"
    "draw(O -- 1.7X, arrow=Arrow3(DefaultHead2), "
    "L=Label(\"$x$\", position=EndPoint));\n"
    "draw(O -- 2.4Y, arrow=Arrow3(), "
    "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 2Z, arrow=Arrow3(), "
    "L=Label(\"$z$\", position=EndPoint, align=W));\n\n"

    "// triangle\n"
    "path3 tri = {1}--{2}--{3}--cycle;\n\n"
    "triple centroid = {4};\n"

    "draw(tri);\n"
    "dot({5}, blue);\n"
    "label(\"$N$\", {5}, align=W);\n"
    "dot({4}, blue);\n"
    "draw(centroid--1.6centroid, arrow=Arrow3(DefaultHead2));\n"
    "dot({6}, blue);\n"
    "label(\"$P$\", {6}, align=E);\n"
    "draw({6}--{7}, dotted);\n",
    axom::fmt::format(asyheader, fname, ifname),
    printPoint(tri[0]),
    printPoint(tri[1]),
    printPoint(tri[2]),
    printPoint(cpl),
    printPoint(neg),
    printPoint(pos),
    printPoint(ppos));

  writeToFile(fname, contents);
}

void showDistance()
{
  // _sqdist_start
  // The point from which we'll query
  PointType q = PointType::make_point(0.75, 1.2, 0.4);

  // Find distance to:
  PointType p {0.2, 1.4, 1.1};
  SegmentType seg(PointType {1.1, 0.0, 0.2}, PointType {1.1, 0.5, 0.2});
  TriangleType tri(PointType {0.2, -0.3, 0.4},
                   PointType {0.25, -0.1, 0.3},
                   PointType {0.3, -0.3, 0.35});
  BoundingBoxType bbox(PointType {-0.3, -0.2, 0.7}, PointType {0.4, 0.3, 0.9});

  double dp = squared_distance(q, p);
  double dseg = squared_distance(q, seg);
  double dtri = squared_distance(q, tri);
  double dbox = squared_distance(q, bbox);
  // _sqdist_end

  std::cout << "----- showDistance -----" << std::endl;
  std::cout << "Squared distance from query point q " << q << std::endl
            << dp << " to point " << p << std::endl
            << dseg << " to segment " << seg << std::endl
            << dtri << " to triangle " << tri << std::endl
            << dbox << " to bounding box " << bbox << std::endl
            << std::endl;

  // Helper variables
  // Project q and p onto onto XY plane
  PointType pq {q[0], q[1], 0.};
  PointType pp {p[0], p[1], 0.};
  PointType boxpt = bbox.getMax();
  boxpt[2] = bbox.getMin()[2];
  PointType pboxpt = boxpt;
  pboxpt[2] = 0;
  PointType pseg = seg.target();
  pseg[2] = 0;
  PointType ptri = tri[1];
  ptri[2] = 0;

  // Now write out an Asymptote file showing what we did.
  std::string basefname = "showDistance";
  std::string fname = basefname + ".asy";
  std::string ifname = basefname + ".png";
  std::string contents = axom::fmt::format(
    "{0}"
    "// axes\n"
    "draw(O -- 1.3X, arrow=Arrow3(DefaultHead2), "
    "L=Label(\"$x$\", position=EndPoint, align=W));\n"
    "draw(O -- 1.8Y, arrow=Arrow3(), "
    "L=Label(\"$y$\", position=EndPoint));\n"
    "draw(O -- 1.2Z, arrow=Arrow3(), "
    "L=Label(\"$z$\", position=EndPoint, align=W));\n\n"

    "// query point\n"
    "triple q = {1};\n"
    "// other primitives\n"
    "triple boxpt = {2};\n"
    "triple p = {3};\n"
    "dot(q);    dot(p, blue);\n"
    "draw({4}--{5}, blue);\n"
    "draw({6}--{7}--{8}--cycle, blue);\n"
    "draw(box({9}, {10}), blue);\n\n"

    "// distances and drop-points\n"
    "draw(q--p, L=Label(\"{11}\"));\n"
    "draw(q--{5}, L=Label(\"{12}\"));\n"
    "draw(q--{7}, L=Label(\"{13}\"));\n"
    "draw(q--boxpt, L=Label(\"{14}\"));\n"
    "draw(q--{15}, dotted);\n"
    "draw(p--{16}, dotted);\n"
    "draw({5}--{17}, dotted);\n"
    "draw({7}--{18}, dotted);\n"
    "draw(boxpt--{19}, dotted);\n",
    axom::fmt::format(asyheader, fname, ifname),
    printPoint(q),
    printPoint(boxpt),
    printPoint(p),
    printPoint(seg.source()),
    printPoint(seg.target()),
    printPoint(tri[0]),
    printPoint(tri[1]),
    printPoint(tri[2]),
    printPoint(bbox.getMin()),
    printPoint(bbox.getMax()),
    dp,
    dseg,
    dtri,
    dbox,
    printPoint(pq),
    printPoint(pp),
    printPoint(pseg),
    printPoint(ptri),
    printPoint(pboxpt));

  writeToFile(fname, contents);
}

// _naive_triintersect_start
void findTriIntersectionsNaively(std::vector<TriangleType>& tris,
                                 std::vector<std::pair<int, int>>& clashes)
{
  int tcount = static_cast<int>(tris.size());

  for(int i = 0; i < tcount; ++i)
  {
    TriangleType& t1 = tris[i];
    for(int j = i + 1; j < tcount; ++j)
    {
      TriangleType& t2 = tris[j];
      if(intersect(t1, t2))
      {
        clashes.push_back(std::make_pair(i, j));
      }
    }
  }
}
// _naive_triintersect_end

BoundingBoxType findBbox(std::vector<TriangleType>& tris)
{
  BoundingBoxType bbox;

  for(size_t i = 0; i < tris.size(); ++i)
  {
    bbox.addPoint(tris[i][0]);
    bbox.addPoint(tris[i][1]);
    bbox.addPoint(tris[i][2]);
  }

  return bbox;
}

BoundingBoxType findBbox(TriangleType& tri)
{
  BoundingBoxType bbox;

  bbox.addPoint(tri[0]);
  bbox.addPoint(tri[1]);
  bbox.addPoint(tri[2]);

  return bbox;
}

int main(int argc, char** argv)
{
  AXOM_UNUSED_VAR(argc);
  AXOM_UNUSED_VAR(argv);

  showClip();
  showClosestPoint();
  showBoundingBoxes();
  showIntersect();
  showOrientation();
  showDistance();

  return 0;
}
