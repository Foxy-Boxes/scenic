param map = localPath('maps/borregasave.xodr')
param lgsvl_map = 'BorregasAve'
param time_step = 1.0/10

model scenic.simulators.lgsvl.model



lane = Uniform(*network.lanes)
spot = OrientedPoint on lane.centerline,
	with positionStdDev 0
spot_1 = OrientedPoint at 0 @ 30 relative to spot,
	with positionStdDev 0,
	with heading 0 deg relative to spot.heading,
	with headingStdDev 0,
	with pitch 0 deg relative to spot.pitch,
	with roll 0 deg relative to spot.roll




ego = Car following roadDirection from spot for 0,
	with allowCollisions True


car_1 = Car at spot_1,
	with heading 250 deg relative to spot_1.heading,
	with roll 90 deg relative to spot_1.roll,
	with roadDeviation 0,
	with color Color(169/255,169/255,169/255)




terminate after 40 seconds
