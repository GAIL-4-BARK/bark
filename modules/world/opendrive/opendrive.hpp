// Copyright (c) 2020 Julian Bernhard, Klemens Esterle, Patrick Hart and
// Tobias Kessler
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.


#ifndef MODULES_WORLD_OPENDRIVE_OPENDRIVE_HPP_
#define MODULES_WORLD_OPENDRIVE_OPENDRIVE_HPP_

#include <vector>
#include <map>

#include "modules/world/opendrive/road.hpp"
#include "modules/world/opendrive/junction.hpp"
#include "modules/world/opendrive/lane.hpp"

namespace modules {
namespace world {
namespace opendrive {

using Junctions = std::map<uint32_t, std::shared_ptr<Junction>>;
using modules::geometry::Point2d;

class OpenDriveMap {
 public:
  OpenDriveMap() : roads_(), lanes_(), junctions_() {}
  ~OpenDriveMap() {}

  //! setter functions
  bool AddRoad(std::shared_ptr<XodrRoad> r) {
    roads_[r->GetId()] = r;
    XodrLanes road_lanes = r->GetLanes();
    lanes_.insert(road_lanes.begin(), road_lanes.end());
    return true;
  }
  bool AddJunction(std::shared_ptr<Junction> j) {
    junctions_[j->GetId()] = j;
    return true;
  }
  //! getter functions
  XodrRoadPtr GetRoad(XodrRoadId id) const { return roads_.at(id); }
  std::shared_ptr<Junction> GetJunction(uint32_t id) const {
    return junctions_.at(id);
  }
  XodrLanePtr GetLane(XodrLaneId id) const { return lanes_.at(id); }

  XodrRoads GetRoads() const { return roads_; }
  Junctions GetJunctions() const { return junctions_; }
  XodrLanes GetLanes() const { return lanes_;}

  std::pair<Point2d, Point2d> BoundingBox() const {
    modules::geometry::Line all_lanes_linestrings;
    for (auto &road : GetRoads()) {
      for (auto &lane_section : road.second->GetLaneSections()) {
        for (auto &lane : lane_section->GetLanes()) {
          auto linestring = lane.second->GetLine();
          all_lanes_linestrings.AppendLinestring(linestring);
        }
      }
    }
    return all_lanes_linestrings.BoundingBox();
  }

 private:
  XodrRoads roads_;
  XodrLanes lanes_;
  Junctions junctions_;
};

using OpenDriveMapPtr = std::shared_ptr<OpenDriveMap>;

}  // namespace opendrive
}  // namespace world
}  // namespace modules

#endif  // MODULES_WORLD_OPENDRIVE_OPENDRIVE_HPP_
