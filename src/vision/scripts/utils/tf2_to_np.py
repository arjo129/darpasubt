#!/usr/bin/env python
# taken from https://github.com/eric-wieser/ros_numpy
import collections
import numpy as np

import genpy
from tf_conversions import transformations
from geometry_msgs.msg import Transform, Vector3, Quaternion, Point, Pose


#---------------#
#   numpy_msg   #
#---------------#

import rospy.numpy_msg

"""
The building numpy_msg has a bug - see https://github.com/ros/ros_comm/pull/743
This patches it
"""

_numpy_msg = rospy.numpy_msg.numpy_msg
_cached = {}
def numpy_msg(cls):
	if cls not in _cached:
		res = _numpy_msg(cls)
		_cached[cls] = res
	else:
		res = _cached[cls]

	return res

# patch the original for good measure
rospy.numpy_msg.numpy_msg = numpy_msg

#---------------#
#    registry   #
#---------------#

_to_numpy = {}
_from_numpy = {}

def converts_to_numpy(msgtype, plural=False):
	assert issubclass(msgtype, genpy.Message)
	def decorator(f):
		_to_numpy[msgtype, plural] = f
		_to_numpy[numpy_msg(msgtype), plural] = f
		return f
	return decorator

def converts_from_numpy(msgtype, plural=False):
	assert issubclass(msgtype, genpy.Message)
	def decorator(f):
		_from_numpy[msgtype, plural] = f
		_from_numpy[numpy_msg(msgtype), plural] = f
		return f
	return decorator

def numpify(msg, *args, **kwargs):
    if msg is None:
        return

    conv = _to_numpy.get((msg.__class__, False))
    if not conv and isinstance(msg, collections.Sequence):
        if not msg:
            raise ValueError("Cannot determine the type of an empty Collection")
        conv = _to_numpy.get((msg[0].__class__, True))


    if not conv:
        raise ValueError("Unable to convert message {} - only supports {}".format(
            msg.__class__.__name__,
            ', '.join(cls.__name__ + ("[]" if pl else '') for cls, pl in _to_numpy.keys())
        ))

    return conv(msg, *args, **kwargs)

#--------------#
#   geometry   #
#--------------#

@converts_to_numpy(Vector3)
def vector3_to_numpy(msg, hom=False):
	if hom:
		return np.array([msg.x, msg.y, msg.z, 0])
	else:
		return np.array([msg.x, msg.y, msg.z])

@converts_from_numpy(Vector3)
def numpy_to_vector3(arr):
	if arr.shape[-1] == 4:
		assert np.all(arr[...,-1] == 0)
		arr = arr[...,:-1]

	if len(arr.shape) == 1:
		return Vector3(*arr)
	else:
		return np.apply_along_axis(lambda v: Vector3(*v), axis=-1, arr=arr)

@converts_to_numpy(Point)
def point_to_numpy(msg, hom=False):
	if hom:
		return np.array([msg.x, msg.y, msg.z, 1])
	else:
		return np.array([msg.x, msg.y, msg.z])

@converts_from_numpy(Point)
def numpy_to_point(arr):
	if arr.shape[-1] == 4:
		arr = arr[...,:-1] / arr[...,-1]

	if len(arr.shape) == 1:
		return Point(*arr)
	else:
		return np.array(map(lambda p: Point(p[0],p[1],p[2]), arr))

		# return np.apply_along_axis(lambda v: Point(*v), axis=-1, arr=arr)

@converts_to_numpy(Quaternion)
def quat_to_numpy(msg):
	return np.array([msg.x, msg.y, msg.z, msg.w])

@converts_from_numpy(Quaternion)
def numpy_to_quat(arr):
	assert arr.shape[-1] == 4

	if len(arr.shape) == 1:
		return Quaternion(*arr)
	else:
		return np.apply_along_axis(lambda v: Quaternion(*v), axis=-1, arr=arr)

# compound types
# all of these take ...x4x4 homogeneous matrices

@converts_to_numpy(Transform)
def transform_to_numpy(msg):

	return np.dot(
        transformations.translation_matrix(numpify(msg.translation)),
        transformations.quaternion_matrix(numpify(msg.rotation))
    )

@converts_from_numpy(Transform)
def numpy_to_transform(arr):

	shape, rest = arr.shape[:-2], arr.shape[-2:]
	assert rest == (4,4)

	if len(shape) == 0:
		trans = transformations.translation_from_matrix(arr)
		quat = transformations.quaternion_from_matrix(arr)

		return Transform(
			translation=Vector3(*trans),
			rotation=Quaternion(*quat)
		)
	else:
		res = np.empty(shape, dtype=np.object_)
		for idx in np.ndindex(shape):
			res[idx] = Transform(
				translation=Vector3(*transformations.translation_from_matrix(arr[idx])),
				rotation=Quaternion(*transformations.quaternion_from_matrix(arr[idx]))
			)

@converts_to_numpy(Pose)
def pose_to_numpy(msg):

	return np.dot(
        transformations.translation_matrix(numpify(msg.position)),
        transformations.quaternion_matrix(numpify(msg.orientation))
    )

@converts_from_numpy(Pose)
def numpy_to_pose(arr):

	shape, rest = arr.shape[:-2], arr.shape[-2:]
	assert rest == (4,4)

	if len(shape) == 0:
		trans = transformations.translation_from_matrix(arr)
		quat = transformations.quaternion_from_matrix(arr)

		return Pose(
			position=Vector3(*trans),
			orientation=Quaternion(*quat)
		)
	else:
		res = np.empty(shape, dtype=np.object_)
		for idx in np.ndindex(shape):
			res[idx] = Pose(
				position=Vector3(*transformations.translation_from_matrix(arr[idx])),
				orientation=Quaternion(*transformations.quaternion_from_matrix(arr[idx]))
			)

