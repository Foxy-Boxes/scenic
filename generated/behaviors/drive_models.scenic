
model scenic.domains.driving.model

behavior Drive():
    try:
        do FollowLaneBehavior()
    interrupt when self.distanceToNextObstacle() < 20:
        do PassingBehavior()
    interrupt when self.timeToCollision() < 5:
        do CollisionAvoidance()