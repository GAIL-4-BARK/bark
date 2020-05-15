// Copyright (c) 2019 fortiss GmbH, Julian Bernhard, Klemens Esterle, Patrick
// Hart, Tobias Kessler
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#ifndef MODULES_MODELS_BEHAVIOR_MOTION_PRIMITIVES_MACRO_ACTIONS_HPP_
#define MODULES_MODELS_BEHAVIOR_MOTION_PRIMITIVES_MACRO_ACTIONS_HPP_

#include <vector>

#include "modules/models/behavior/motion_primitives/motion_primitives.hpp"
#include "modules/models/behavior/motion_primitives/primitives/primitive.hpp"
#include "modules/world/map/lane_corridor.hpp"

namespace modules {
namespace models {
namespace behavior {

using commons::ParamsPtr;
using primitives::AdjacentLaneCorridors;
using world::map::LaneCorridorPtr;

class BehaviorMPMacroActions : public BehaviorMotionPrimitives {
 public:
  BehaviorMPMacroActions(const ParamsPtr& params)
      : BehaviorMotionPrimitives(params) {}
  BehaviorMPMacroActions(const ParamsPtr& params,
      const std::vector<primitives::PrimitivePtr>& motion_primitives);

  virtual ~BehaviorMPMacroActions() {}

  virtual Trajectory Plan(float delta_time,
                          const ObservedWorld& observed_world);

  virtual MotionIdx GetNumMotionPrimitives(
      const ObservedWorldPtr& observed_world);

  MotionIdx AddMotionPrimitive(const primitives::PrimitivePtr& primitive);

  virtual std::shared_ptr<BehaviorModel> Clone() const;
  const std::vector<primitives::PrimitivePtr>& GetMotionPrimitives() const;
  const std::vector<BehaviorMPMacroActions::MotionIdx>& GetValidPrimitives(
      const ObservedWorldPtr& observed_world);

 private:
  MotionIdx GetNumMotionPrimitivesByCorridors(
      const ObservedWorld& observed_world,
      const AdjacentLaneCorridors& adjacent_corridors);
  std::vector<primitives::PrimitivePtr> motion_primitives_;
  std::vector<MotionIdx> valid_primitives_;
  LaneCorridorPtr target_corridor_;
  AdjacentLaneCorridors GetCorridors(const ObservedWorld& observed_world);
};

}  // namespace behavior
}  // namespace models
}  // namespace modules

#endif  // MODULES_MODELS_BEHAVIOR_MOTION_PRIMITIVES_MACRO_ACTIONS_HPP_
