param map = localPath('../../../tests/formats/opendrive/maps/CARLA/Town01.xodr')
param carla_map = 'Town01'
model scenic.simulators.carla.model


EGO_MODEL = "vehicle.audi.a2"
behavior GoForward(speed):
	while True:
		totalTime = 0
		while totalTime < 15:
			totalTime += 1
			wait
		take SetSpeedAction(speed)
		totalTime = 0
		while totalTime < 5:
			totalTime += 1
			wait
		take SetBrakeAction(1)
behavior AutopilotWithInitial(speed=26):
	take SetSpeedAction(speed)
	do AutopilotBehavior()
lane = Uniform(*network.lanes)
spot = OrientedPoint on lane.centerline
spot_1 = OrientedPoint at 0 @ 20 relative to spot,
	with heading 0 deg relative to spot.heading



ego = Car following roadDirection from spot for 0,
	with blueprint EGO_MODEL,
	with behavior AutopilotWithInitial()


car_1 = Car at spot_1,
	with heading 0 deg relative to spot_1.heading,
	with behavior GoForward(25)


require (distance to intersection) > 100

terminate after 60 seconds
