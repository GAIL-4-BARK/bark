# Copyright (c) 2020 Julian Bernhard, Klemens Esterle, Patrick Hart and
# Tobias Kessler
#
# This work is licensed under the terms of the MIT license.
# For a copy, see <https://opensource.org/licenses/MIT>.

from bark.core.world.agent import Agent
from bark.core.world import World
from bark.core.world.map import MapInterface
from bark.runtime.commons.parameters import ParameterServer
from bark.runtime.commons.xodr_parser import XodrParser
import copy
from pathlib import Path


class Scenario:
  def __init__(self,
               agent_list=None,
               eval_agent_ids=None,
               map_file_name=None,
               json_params=None,
               map_interface=None):
    self._agent_list = agent_list or []
    self._eval_agent_ids = eval_agent_ids or []
    self._map_file_name = map_file_name
    self._json_params = json_params
    self._map_interface = map_interface

  @property
  def map_file_name(self):
    return self._map_file_name
  
  @property
  def json_params(self):
    return self._json_params

  @property
  def eval_agent_ids(self):
    return self._eval_agent_ids

  @property
  def map_interface(self):
    return self._map_interface

  @map_interface.setter
  def map_interface(self, map_interface):
    self._map_interface = map_interface
  
  def GetWorldState(self):
    """get initial world state of scenario to start simulation from here
    
    Returns:
        [bark.core.world.World]
    """
    return self._build_world_state()

  def copy(self):
    return Scenario(agent_list=copy.deepcopy(self._agent_list),
                    eval_agent_ids=self._eval_agent_ids.copy(),
                    map_file_name=self._map_file_name,
                    json_params=self._json_params.copy(),
                    map_interface=self._map_interface)

  def _build_world_state(self):
    param_server = ParameterServer(json=self._json_params)
    world = World(param_server)
    if self._map_interface is None:
      self.CreateMapInterface(self._map_file_name)
      world.SetMap(self._map_interface)
    else:
      world.SetMap(self._map_interface)
    for agent in self._agent_list:
      agent.GenerateRoadCorridor(self._map_interface)
      world.AddAgent(agent)
    world.UpdateAgentRTree()
    return world

  def __getstate__(self):
    odict = self.__dict__.copy()
    del odict['_map_interface']
    return odict

  def __setstate__(self, sdict):
    sdict['_map_interface'] = None
    self.__dict__.update(sdict)

  # TODO(@hart): should be a commons function
  def CreateMapInterface(self, map_file_name):
    map_file_load_test = Path(map_file_name)
    if map_file_load_test.is_file():
      xodr_parser = XodrParser(map_file_name)
    else:
      objects_found = sorted(Path().rglob(map_file_name))
      if len(objects_found) == 0:
        raise ValueError("No Map found")
      elif len(objects_found) > 1:
        raise ValueError("Multiple Maps found")
      else:
        xodr_parser = XodrParser(objects_found[0].as_posix())

    map_interface = MapInterface()
    map_interface.SetOpenDriveMap(xodr_parser.map)
    self._map_interface = map_interface





