// Copyright (c) 2020 fortiss GmbH
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#ifndef BARK_WORLD_EVALUATION_LTL_EVALUATOR_SAFE_DISTANCE_HPP_
#define BARK_WORLD_EVALUATION_LTL_EVALUATOR_SAFE_DISTANCE_HPP_

#include "bark/world/evaluation/ltl/evaluator_ltl.hpp"

namespace bark {
namespace world {
namespace evaluation {

class EvaluatorSafeDistance : public EvaluatorLTL {
 public:
  explicit EvaluatorSafeDistance(AgentId agent_id)
      : EvaluatorLTL(agent_id, formula_, labels_) {}

  static const char formula_[];
  static const LabelFunctions labels_;
  constexpr static double reaction_time = 1.0;
  // From "A formally verified motion planner for autonomous vehicles"
  constexpr static double decel_front = -7.84;
  constexpr static double decel_ego = -7.84;
};

}  // namespace evaluation
}  // namespace world
}  // namespace bark
#endif  // BARK_WORLD_EVALUATION_LTL_EVALUATOR_SAFE_DISTANCE_HPP_
