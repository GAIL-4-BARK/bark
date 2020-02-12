// Copyright (c) 2019 fortiss GmbH, Julian Bernhard, Klemens Esterle, Patrick
// Hart, Tobias Kessler
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#ifndef MODULES_MODELS_BEHAVIOR_MOTION_PRIMITIVES_HPP_
#define MODULES_MODELS_BEHAVIOR_MOTION_PRIMITIVES_HPP_

#include "modules/models/behavior/behavior_model.hpp"
#include "modules/models/dynamic/dynamic_model.hpp"

namespace modules {
namespace models {
namespace behavior {

using dynamic::DynamicModelPtr;
using dynamic::Input;
using dynamic::State;
using dynamic::Trajectory;
using world::ObservedWorld;
using world::objects::AgentId;

class BehaviorMotionPrimitives : public BehaviorModel {
 public:
  BehaviorMotionPrimitives(const DynamicModelPtr& dynamic_model,
                           commons::Params* params);

  virtual ~BehaviorMotionPrimitives() {}

  virtual Trajectory Plan(float delta_time,
                          const ObservedWorld& observed_world);

  typedef unsigned int MotionIdx;
  MotionIdx AddMotionPrimitive(const Input& dynamic_input);
  MotionIdx GetNumMotionPrimitives() const { return motion_primitives_.size(); }
  void ActionToBehavior(const MotionIdx& motion_idx);

  virtual std::shared_ptr<BehaviorModel> Clone() const;

 private:
  DynamicModelPtr dynamic_model_;
  std::vector<Input> motion_primitives_;
  MotionIdx active_motion_;

  // Parameters
  float integration_time_delta_;
};

inline std::shared_ptr<BehaviorModel> BehaviorMotionPrimitives::Clone() const {
  std::shared_ptr<BehaviorMotionPrimitives> model_ptr =
      std::make_shared<BehaviorMotionPrimitives>(*this);
  return std::dynamic_pointer_cast<BehaviorModel>(model_ptr);
}

}  // namespace behavior
}  // namespace models
}  // namespace modules

#endif  // MODULES_MODELS_BEHAVIOR_MOTION_PRIMITIVES_HPP_
