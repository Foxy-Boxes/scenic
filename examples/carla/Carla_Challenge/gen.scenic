param map = localPath('../../../tests/formats/opendrive/maps/CARLA/Town01.xodr')
param carla_map = 'Town01'
model scenic.simulators.carla.model


EGO_MODEL = "vehicle.audi.a2"
behavior GoForward(speed):
	take SetSpeedAction(speed)
lane = Uniform(*network.lanes)
spot = OrientedPoint on lane.centerline
spot_1 = OrientedPoint at -0.100000 @ 33 relative to spot,
	with heading 0 deg relative to spot.heading
spot_2 = OrientedPoint at -0.060000 @ 66 relative to spot,
	with heading 13 deg relative to spot.heading


ospot_0 = OrientedPoint at -2.180000 @ 32 relative to spot,
	with heading 0 deg relative to spot.heading
ospot_1 = OrientedPoint at -2.320000 @ 66 relative to spot,
	with heading 0 deg relative to spot.heading


ego = Car following roadDirection from spot for 0,
	with blueprint EGO_MODEL,
	with behavior AutopilotBehavior()


car_1 = Car at spot_1,
	with heading 0 deg relative to spot_1.heading,
	with behavior AutopilotBehavior()
car_2 = Car at spot_2,
	with heading 0 deg relative to spot_2.heading,
	with behavior GoForward(10)


obj_0 = Trash at ospot_0,
	with heading 0 deg relative to ospot_0.heading
obj_1 = Trash at ospot_1,
	with heading 0 deg relative to ospot_1.heading


terminate after 120 seconds
terminate when withinDistanceToAnyObjs(ego,7) and ego.speed < 0.01
