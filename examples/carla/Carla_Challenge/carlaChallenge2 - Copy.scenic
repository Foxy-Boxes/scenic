""" Scenario Description
Traffic Scenario 02.
Longitudinal control after leading vehicle’s brake.
The leading vehicle decelerates suddenly due to an obstacle and the ego-vehicle must perform an
emergency brake or an avoidance maneuver.
"""

## SET MAP AND MODEL (i.e. definitions of all referenceable vehicle types, road library, etc)
param map = localPath('../../../tests/formats/opendrive/maps/CARLA/Town01.xodr')  # or other CARLA map that definitely works
param carla_map = 'Town01'
model scenic.simulators.carla.model

## CONSTANTS
EGO_MODEL = "vehicle.audi.a2"

## DEFINING SPATIAL RELATIONS
# Please refer to scenic/domains/driving/roads.py how to access detailed road infrastructure
# 'network' is the 'class Network' object in roads.py

# make sure to put '*' to uniformly randomly select from all elements of the list, 'lanes'
lane = Uniform(*network.lanes)
spot = OrientedPoint on lane.centerline,
	with heading 0 deg relative to roadDirection
 
sto = OrientedPoint at 0 @ 10 relative to spot,
	with heading 30 deg relative to spot.heading

ego = Car following roadDirection from spot for 0,
	with blueprint EGO_MODEL,
	with behavior AutopilotBehavior()

a = Car at sto,
	with heading 0 deg relative to sto.heading

require ego can see a
require (distance to intersection) > 80
terminate when withinDistanceToAnyObjs(ego,7) and ego.speed < 0.01