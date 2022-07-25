param map = localPath('../../../tests/formats/opendrive/maps/CARLA/Town01.xodr')
param carla_map = 'Town01'
model scenic.simulators.carla.model


EGO_MODEL = "vehicle.audi.a2"
behavior GoForward(speed):
	totalTime = 0
	while totalTime < 60:
		totalTime += 1
		wait
	take SetSpeedAction(speed)
lane = Uniform(*network.lanes)
spot = OrientedPoint on lane.centerline
spot_1 = OrientedPoint at -0.260000 @ -26 relative to spot,
	with heading 0 deg relative to spot.heading
EGO_SPEED = 10
EGO_BRAKING_THRESHOLD = 12
BRAKE_ACTION = 1.0
behavior EgoBehavior(speed=80):
    try:
        do FollowLaneBehavior(speed)

    interrupt when withinDistanceToAnyCars(self, EGO_BRAKING_THRESHOLD):
        take SetBrakeAction(BRAKE_ACTION)



ego = Car following roadDirection from spot for 0,
	with blueprint EGO_MODEL,
	with behavior EgoBehavior()


car_1 = Car at spot_1,
	with heading 0 deg relative to spot_1.heading,
	with behavior GoForward(40)



terminate after 40 seconds

