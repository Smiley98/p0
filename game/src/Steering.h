#pragma once
#include "raymathext.h"

inline Vector3 Seek(Vector3 seeker_position, Vector3 target_position, Vector3 seeker_velocity, float speed)
{
	Vector3 desired_velocity = Vector3Normalize(target_position - seeker_position) * speed;
	return desired_velocity - seeker_velocity;
}

inline Vector3 Flee(Vector3 seeker_position, Vector3 target_position, Vector3 seeker_velocity, float speed)
{
	Vector3 desired_velocity = Vector3Normalize(seeker_position - target_position) * speed;
	return desired_velocity - seeker_velocity;
}
