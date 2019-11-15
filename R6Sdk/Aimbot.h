#pragma once
#include "Cheat.h"
constexpr float r2d = 57.2957795131f;
constexpr float d2r = 0.01745329251f;
#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI_F / 180.f) )
enum
{
	PITCH = 0,	// up / down
	YAW,		// left / right
	ROLL		// fall over
};
static Vector4D QuaternionFromYPR(float yaw, float pitch, float roll)
{
	Vector4D q;
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cr = cos(pitch * 0.5);
	double sr = sin(pitch * 0.5);
	double cp = cos(roll * 0.5);
	double sp = sin(roll * 0.5);

	q.w = (float)(cy * cr * cp + sy * sr * sp);
	q.x = (float)(cy * sr * cp - sy * cr * sp);
	q.y = (float)(cy * cr * sp + sy * sr * cp);
	q.z = (float)(sy * cr * cp - cy * sr * sp);
	return q;
}
static float GetDifference(float firstAngle, float secondAngle)
{
	float difference = secondAngle - firstAngle;
	while (difference < -180.f) difference += 360.f;
	while (difference > 180.f) difference -= 360.f;
	return difference;
}
static Vector ToEuler(Vector4D q)
{
	Vector end = Vector();

	float sinr = (float)(+2.0 * (q.w * q.x + q.y * q.z));
	float cosr = (float)(+1.0 - 2.0 * (q.x * q.x + q.y * q.y));
	end.z = (float)atan2(sinr, cosr);

	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
	if (abs(sinp) >= 1)
		end.x = (float)copysign(M_PI / 2, sinp);
	else
		end.x = (float)asin(sinp);

	double siny = +2.0 * (q.w * q.z + q.x * q.y);
	double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	end.y = (float)atan2(siny, cosy);

	return end;
}
static void inline SinCos(float radians, float* sine, float* cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

static Vector VectorAngles(Vector forward)
{
	Vector angles;
	float tmp, yaw, pitch;

	if (forward.y == 0 && forward.y == 0)
	{
		yaw = 0;
		if (forward.y > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (float)(atan2(forward.y, forward.x) * 180 / M_PI);


		tmp = (float)sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = (float)(atan2(forward.z * -1, tmp) * 180 / M_PI);

	}

	yaw += 90;

	if (yaw > 180)
	{
		yaw -= 360;
	}
	if (pitch > 180)
	{
		pitch -= 360;
	}

	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
	return angles;
}
static void AngleVectors(const Vector& angles, Vector* forward)
{
	float sp, sy, cp, cy;
	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}
static void VectorAngles(const Vector& forward, Vector& angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;
		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}
	angles[2] = 0.0f;

}
static float W2SDistance(Vector position)
{
	ImGuiIO& io = ImGui::GetIO();
	auto pCameraManager = I::Engine->GetCamera();
	Vector2D out;
	I::Engine->WorldToScreen(position, out);
	return (fabs(out.x - (io.DisplaySize.x / 2)) + fabs(out.y - (io.DisplaySize.y / 2)));
}

static Vector CalcAngle(Vector src, Vector dst)
{
	Vector angles;
	Vector delta = src - dst;
	VectorAngles(delta, angles);
	delta.Normalize();
	return angles;
}
class CAimBot
{
public:
	Vector4D GetAngle(Vector viewTranslation, Vector enemyHead, Vector4D originalAngle);
	void DropTarget();
	void FindTarget();
	void DoAimbot();
	void AntiAim();
	
	CBaseEntity* m_BastFovTarget;
	Vector m_BestPoint;
	bool aimboting;
	bool shoting;
	bool zooming;
	bool reseting;
	bool goAntiAim;
	int m_besthitbox;
	//	int m_tickcount_backup;
};