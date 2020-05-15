// Copyright (c) 2019 fortiss GmbH, Julian Bernhard, Klemens Esterle, Patrick
// Hart, Tobias Kessler
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include <Eigen/Core>
#include "gtest/gtest.h"

#include "modules/commons/params/default_params.hpp"
#include "modules/commons/params/setter_params.hpp"
#include "modules/geometry/commons.hpp"
#include "modules/geometry/line.hpp"
#include "modules/geometry/polygon.hpp"
#include "modules/models/behavior/motion_primitives/continuous_actions.hpp"
#include "modules/models/behavior/motion_primitives/macro_actions.hpp"
#include "modules/models/behavior/motion_primitives/primitives/primitive.hpp"
#include "modules/models/behavior/motion_primitives/primitives/primitive_const_acceleration.hpp"
#include "modules/models/behavior/motion_primitives/primitives/primitive_gap_keeping.hpp"
#include "modules/models/behavior/motion_primitives/primitives/primitive_const_acc_change_to_left.hpp"
#include "modules/models/behavior/motion_primitives/primitives/primitive_const_acc_change_to_right.hpp"
#include "modules/models/dynamic/single_track.hpp"
#include "modules/world/observed_world.hpp"
#include "modules/world/tests/make_test_world.hpp"

using namespace modules::models::dynamic;
using namespace modules::models::execution;
using namespace modules::commons;
using namespace modules::models::behavior;
using namespace modules::models::dynamic;
using namespace modules::world;
using namespace modules::geometry;
using namespace modules::world::tests;

class DummyObservedWorld : public ObservedWorld {
 public:
  DummyObservedWorld(const State& init_state, const ParamsPtr& params)
      : ObservedWorld(std::make_shared<World>(params), AgentId()),
        init_state_(init_state) {}

  virtual State CurrentEgoState() const { return init_state_; }

  virtual double GetWorldTime() const { return 0.0f; }

 private:
  State init_state_;
};

AdjacentLaneCorridors GetCorridors(const ObservedWorld& observed_world) {
  auto target_corridor = observed_world.GetLaneCorridor();
  AdjacentLaneCorridors adjacent_corridors;
  auto ego_pos = observed_world.CurrentEgoPosition();
  auto road_corridor = observed_world.GetRoadCorridor();
  std::tie(adjacent_corridors.left, adjacent_corridors.right) =
      road_corridor->GetLeftRightLaneCorridor(ego_pos);
  adjacent_corridors.current = target_corridor;
  return adjacent_corridors;
}

TEST(behavior_motion_primitives_add, behavior_test) {
  auto params = std::make_shared<DefaultParams>();
  DynamicModelPtr dynamics(new SingleTrackModel(params));
  BehaviorMPContinuousActions behavior(dynamics, params);
  Input u(2);
  u << 0, 0;
  behavior.AddMotionPrimitive(u);
}

TEST(behavior_motion_primitives_plan, behavior_test) {
  auto params = std::make_shared<SetterParams>();
  params->SetReal("integration_time_delta", 0.01);
  DynamicModelPtr dynamics(new SingleTrackModel(params));

  BehaviorMPContinuousActions behavior(dynamics, params);
  Input u1(2);
  u1 << 2, 0;
  BehaviorMPContinuousActions::MotionIdx idx1 = behavior.AddMotionPrimitive(u1);
  Input u2(2);
  u2 << 0, 1;
  BehaviorMPContinuousActions::MotionIdx idx2 = behavior.AddMotionPrimitive(u2);
  Input u3(2);
  u3 << 0, 0;
  BehaviorMPContinuousActions::MotionIdx idx3 = behavior.AddMotionPrimitive(u3);

  // X Longitudinal with zero velocity
  State init_state(static_cast<int>(StateDefinition::MIN_STATE_SIZE));
  init_state << 0.0, 0.0, 0.0, 0.0, 0.0;
  DummyObservedWorld world(init_state, params);

  behavior.ActionToBehavior(idx1);
  Trajectory traj1 = behavior.Plan(0.5, world);
  EXPECT_NEAR(traj1(traj1.rows() - 1, StateDefinition::X_POSITION),
              2 / 2 * 0.5 * 0.5, 0.05);

  // X Longitudinal with nonzero velocity
  init_state << 0.0, 0.0, 0.0, 0.0, 5.0;
  DummyObservedWorld world1(init_state, params);
  behavior.ActionToBehavior(idx1);
  traj1 = behavior.Plan(0.5, world1);
  EXPECT_NEAR(traj1(traj1.rows() - 1, StateDefinition::X_POSITION),
              5.0 * 0.5 + 2 / 2 * 0.5 * 0.5, 0.1);

  // Y Longitudinal
  init_state << 0.0, 0.0, 0.0, B_PI_2, 0.0;
  DummyObservedWorld world2(init_state, params);
  behavior.ActionToBehavior(idx1);
  traj1 = behavior.Plan(0.5, world2);
  EXPECT_NEAR(traj1(traj1.rows() - 1, StateDefinition::Y_POSITION),
              2 / 2 * 0.5 * 0.5, 0.05);

  // X Constant motion
  init_state << 0.0, 0.0, 0.0, 0.0, 2.0;
  DummyObservedWorld world3(init_state, params);
  behavior.ActionToBehavior(idx3);
  Trajectory traj3 = behavior.Plan(0.5, world3);
  EXPECT_NEAR(traj3(traj3.rows() - 1, StateDefinition::X_POSITION), 0.5 * 2,
              0.005);
}

TEST(primitive_constant_acceleration, behavior_test) {
  using modules::models::behavior::primitives::PrimitiveConstAcceleration;
  using modules::models::behavior::primitives::AdjacentLaneCorridors;
  auto params = std::make_shared<DefaultParams>();
  PrimitiveConstAcceleration primitive(params, 0);

  auto world1 = make_test_observed_world(0, 0.0, 5.0, 0.0);
  AdjacentLaneCorridors corridors = GetCorridors(world1);
  EXPECT_TRUE(primitive.IsPreConditionSatisfied(world1, corridors));
//  auto traj = primitive.Plan(0.5, world1);

  PrimitiveConstAcceleration dec_primitive(params, -5.0);
  EXPECT_TRUE(dec_primitive.IsPreConditionSatisfied(world1, corridors));
  auto world2 = make_test_observed_world(0, 0.0, 0.0, 0.0);
  AdjacentLaneCorridors corridors2 = GetCorridors(world2);
  EXPECT_FALSE(dec_primitive.IsPreConditionSatisfied(world2, corridors2));


  PrimitiveConstAcceleration acc_primitive(params, 5.0);
  EXPECT_TRUE(acc_primitive.IsPreConditionSatisfied(world1, corridors));
  auto world3 = make_test_observed_world(0, 0.0, 50.0, 0.0);
  AdjacentLaneCorridors corridors3 = GetCorridors(world3);
  EXPECT_FALSE(acc_primitive.IsPreConditionSatisfied(world3, corridors));
}

TEST(primitive_change_left, behavior_test) {
  using modules::models::behavior::primitives::PrimitiveConstAccChangeToLeft;
  auto params = std::make_shared<DefaultParams>();
  PrimitiveConstAccChangeToLeft primitive(params);
  auto world = MakeTestWorldHighway();
  auto observed_worlds = world->Observe({0, 3});
  auto corridors0 = GetCorridors(observed_worlds[0]);
  EXPECT_FALSE(primitive.IsPreConditionSatisfied(observed_worlds[0], corridors0));
  auto corridors1 = GetCorridors(observed_worlds[1]);
  EXPECT_TRUE(primitive.IsPreConditionSatisfied(observed_worlds[1], corridors1));
  // auto traj = primitive.Plan(0.5, world1);
}

TEST(primitive_change_right, behavior_test) {
  using modules::models::behavior::primitives::PrimitiveConstAccChangeToRight;
  auto params = std::make_shared<DefaultParams>();
  PrimitiveConstAccChangeToRight primitive(params);
  auto world = MakeTestWorldHighway();
  auto observed_worlds = world->Observe({0, 3});
  auto corridors0 = GetCorridors(observed_worlds[0]);
  EXPECT_TRUE(primitive.IsPreConditionSatisfied(observed_worlds[0], corridors0));
  auto corridors1 = GetCorridors(observed_worlds[1]);
  EXPECT_FALSE(primitive.IsPreConditionSatisfied(observed_worlds[1], corridors1));
  // auto traj = primitive.Plan(0.5, world1);
}

TEST(primitive_gap_keeping, behavior_test) {
  using modules::models::behavior::primitives::PrimitiveGapKeeping;
  auto params = std::make_shared<DefaultParams>();
  DynamicModelPtr dynamics(new SingleTrackModel(params));
  PrimitiveGapKeeping primitive(params);

  State init_state(static_cast<int>(StateDefinition::MIN_STATE_SIZE));
  init_state << 0.0, 0.0, 0.0, 0.0, 5.0;
  auto world1 = DummyObservedWorld(init_state, params);
  AdjacentLaneCorridors corridors = {nullptr, nullptr, nullptr};
  EXPECT_TRUE(primitive.IsPreConditionSatisfied(world1, corridors));
  // auto traj = primitive.Plan(0.5, world1);
}

TEST(macro_actions, behavior_test) {
  using namespace modules::models::behavior::primitives;
  using modules::models::behavior::primitives::PrimitiveConstAcceleration;

  auto params = std::make_shared<DefaultParams>();
  params->SetReal("integration_time_delta", 0.01);

  std::vector<std::shared_ptr<Primitive>> prim_vec;
  
  auto primitive =
      std::make_shared<PrimitiveConstAcceleration>(params, 0.0);
  prim_vec.push_back(primitive);

  auto primitive_left =
      std::make_shared<PrimitiveConstAccChangeToLeft>(params);
  prim_vec.push_back(primitive_left);

  auto primitive_right =
      std::make_shared<PrimitiveConstAccChangeToRight>(params);
  prim_vec.push_back(primitive_right);

  BehaviorMPMacroActions behavior(params);
  for (const auto& p : prim_vec) {
    auto idx = behavior.AddMotionPrimitive(p);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
