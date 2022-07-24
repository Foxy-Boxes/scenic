param map = localPath('../../../tests/formats/opendrive/maps/CARLA/Town01.xodr')
param carla_map = 'Town01'
model scenic.simulators.carla.model


EGO_MODEL = "vehicle.audi.a2"
behavior GoForward(speed):
	take SetSpeedAction(speed)
lane = Uniform(*network.lanes)
spot = OrientedPoint on lane.centerline
spot_1 = OrientedPoint at -2.200000 @ 45 relative to spot,
	with heading 0 deg relative to spot.heading
spot_2 = OrientedPoint at -0.160000 @ 68 relative to spot,
	with heading 0 deg relative to spot.heading
spot_3 = OrientedPoint at 0.020000 @ 27 relative to spot,
	with heading 0 deg relative to spot.heading


ospot_0 = OrientedPoint at -0.600000 @ 46 relative to spot,
	with heading 0 deg relative to spot.heading


ego = Car following roadDirection from spot for 0,
	with blueprint EGO_MODEL,
	with behavior AutopilotBehavior()


car_1 = Car at spot_1,
	with heading 0 deg relative to spot_1.heading,
	with behavior AutopilotBehavior()
car_2 = Car at spot_2,
	with heading 0 deg relative to spot_2.heading,
	with behavior AutopilotBehavior()
car_3 = Car at spot_3,
	with heading 0 deg relative to spot_3.heading,
	with behavior GoForward(-55)


obj_0 = Trash at ospot_0,
	with heading 0 deg relative to ospot_0.heading


terminate after 120 seconds
terminate when withinDistanceToAnyObjs(ego,7) and ego.speed < 0.01
