param map = localPath('../../../tests/formats/opendrive/maps/CARLA/Town01.xodr')
param carla_map = 'Town01'
model scenic.simulators.carla.model


EGO_MODEL = "vehicle.audi.a2"
EGO_SPEED = 10
EGO_BRAKING_THRESHOLD = 5
BRAKE_ACTION = 1.0
behavior GoForward(speed):
	take SetSpeedAction(speed)
lane = Uniform(*network.lanes)
spot = OrientedPoint on lane.centerline,
	with positionStdDev 0
spot_1 = OrientedPoint at 0 @ 30 relative to spot,
	with positionStdDev 0,
	with heading 0 deg relative to spot.heading,
	with headingStdDev 0,
	with pitch 0 deg relative to spot.pitch,
	with roll 0 deg relative to spot.roll

behavior EgoBehavior(speed=70):
    try:
        do FollowLaneBehavior(speed)

    interrupt when withinDistanceToAnyCars(self, EGO_BRAKING_THRESHOLD):
        take SetBrakeAction(BRAKE_ACTION)



ego = Car following roadDirection from spot for 0,
	with blueprint EGO_MODEL,
	with allowCollisions True,
	with behavior EgoBehavior()


car_1 = Car at spot_1,
	with heading 250 deg relative to spot_1.heading,
	with roll 90 deg relative to spot_1.roll,
	with roadDeviation 0,
	with color Color(169/255,169/255,169/255),
	with allowCollisions True




terminate after 40 seconds
