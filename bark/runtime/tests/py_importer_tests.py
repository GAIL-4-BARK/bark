# Copyright (c) 2020 Julian Bernhard, Klemens Esterle, Patrick Hart and
# Tobias Kessler
#
# This work is licensed under the terms of the MIT license.
# For a copy, see <https://opensource.org/licenses/MIT>.

import unittest
import filecmp
import os
import matplotlib.pyplot as plt
from bark.core.geometry.standard_shapes import *
from bark.runtime.commons.parameters import ParameterServer
from bark.core.world.opendrive import XodrLaneType
from bark.core.world.map import MapInterface
from bark.runtime.commons.xodr_parser import XodrParser


def helper_plot(xodr_parser):
  for _, road in xodr_parser.map.GetRoads().items():
    for lane_section in road.lane_sections:
      for _, lane in lane_section.GetLanes().items():
        if lane.lane_type == XodrLaneType.driving:
          color = "grey"
        elif lane.lane_type == XodrLaneType.sidewalk:
          color = "green"
        elif lane.lane_type == XodrLaneType.border:
          color = "red"
        elif lane.lane_type == XodrLaneType.none:
          color = "blue"
        else:
          continue
        line_np = lane.line.ToArray()

        # print(lane.road_mark)
        plt.text(line_np[-1, 0], line_np[-1, 1],
                 'outer_{i}_{j}'.format(i=lane.lane_id, j=lane.lane_position))

        plt.plot(
            line_np[:, 0],
            line_np[:, 1],
            color=color,
            alpha=1.0)

  plt.axis("equal")
  plt.show()


class ImporterTests(unittest.TestCase):
<<<<<<< HEAD
  def test_map_highway(self):
=======
  def test_map_CulDeSac(self):
    xodr_parser = XodrParser(os.path.join(os.path.dirname(__file__),"data/CulDeSac.xodr"))

    map_interface = MapInterface()
    map_interface.SetOpenDriveMap(xodr_parser.map)
    # helper_plot(xodr_parser)

  def test_map_city_highway_straight(self):
<<<<<<< HEAD
>>>>>>> bark library -python_warpper, fix import errors, run bazel tests
    xodr_parser = XodrParser(
      "bark/runtime/tests/data/city_highway_straight.xodr")
=======
    xodr_parser = XodrParser(os.path.join(os.path.dirname(__file__),"data/city_highway_straight.xodr"))
>>>>>>> add all tests as dependency to pip_packange and fixed tests for setup.py test

    map_interface = MapInterface()
    map_interface.SetOpenDriveMap(xodr_parser.map)
    # helper_plot(xodr_parser)

<<<<<<< HEAD
=======
  def test_map_city_highway_curved(self):
    xodr_parser = XodrParser(os.path.join(os.path.dirname(__file__),
      "data/city_highway_curved.xodr"))

    map_interface = MapInterface()
    map_interface.SetOpenDriveMap(xodr_parser.map)
    # helper_plot(xodr_parser)

  def test_map_4way_intersection(self):
    xodr_parser = XodrParser(os.path.join(os.path.dirname(__file__),"data/4way_intersection.xodr"))

    map_interface = MapInterface()
    map_interface.SetOpenDriveMap(xodr_parser.map)
    # helper_plot(xodr_parser)

  def test_map_urban_road(self):
    xodr_parser = XodrParser(os.path.join(os.path.dirname(__file__),"data/urban_road.xodr"))

    map_interface = MapInterface()
    map_interface.SetOpenDriveMap(xodr_parser.map)
    # helper_plot(xodr_parser)
>>>>>>> bark library -python_warpper, fix import errors, run bazel tests

<<<<<<< HEAD
  def test_map_DR_DEU_Merging_MT_v01_shifted(self):
    xodr_parser = XodrParser(
<<<<<<< HEAD
      "modules/runtime/tests/data/DR_DEU_Merging_MT_v01_shifted.xodr")
=======
      "bark/runtime/tests/data/Crossing8Course.xodr")
>>>>>>> bark library -python_warpper, fix import errors, run bazel tests
=======
  def test_map_Crossing8(self):
    xodr_parser = XodrParser(os.path.join(os.path.dirname(__file__),"data/Crossing8Course.xodr"))
>>>>>>> add all tests as dependency to pip_packange and fixed tests for setup.py test

    map_interface = MapInterface()
    map_interface.SetOpenDriveMap(xodr_parser.map)
    # helper_plot(xodr_parser)


if __name__ == '__main__':
  unittest.main()
