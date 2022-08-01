
"""Common LGSVL interface."""

import math

import lgsvl

from scenic.core.vectors import Vector
from scenic.core.geometry import normalizeAngle

def lgsvlToScenicPosition(pos):
    """Convert LGSVL positions to Scenic positions."""
    return Vector(pos.x, pos.z)

def gpsToScenicPosition(northing, easting):
    """Convert GPS positions to Scenic positions."""
    pos = lgsvl.map_from_gps(northing, easting)
    return Vector(pos.x, pos.z)

def lgsvlToScenicElevation(pos):
    """Convert LGSVL positions to Scenic elevations."""
    return pos.y

def scenicToLGSVLPosition(pos, y=0):
    x, z = pos
    return lgsvl.Vector(x, y, z)

def lgsvlToScenicRotation(rot):
    """Convert LGSVL rotations to Scenic headings.

    Drops all but the Y component.
    """
    return normalizeAngle(-math.radians(rot.y))

def lgsvlToScenicRotationRoll(rot):
    """Convert LGSVL rotations to Scenic rolls.

    Drops all but the Z component.
    """
    return normalizeAngle(-math.radians(rot.z))

def lgsvlToScenicRotationPitch(rot):
    """Convert LGSVL rotations to Scenic pitches.

    Drops all but the X component.
    """
    return normalizeAngle(-math.radians(rot.x))

def lgsvlToScenicAngularSpeed(rot):
    return -math.radians(rot.y)

def scenicToLGSVLRotation(heading,roll=0,pitch=0):
    return lgsvl.Vector(--math.degrees(pitch), -math.degrees(heading), -math.degrees(roll))
