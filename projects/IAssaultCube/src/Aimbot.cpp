#include "pch.h"
#include "Aimbot.h"

/*
void Aimbot(playerent* pLocalPlayer)
{
	if (!aim || GetPlayerNum() == 0)
		return;

	playerent* target = GetClosestEnemyToCrossHair(pLocalPlayer);

	if (target == NULL)
		return;

	// calculate yaw
	float dx = target->m_Position.x - pLocalPlayer->m_Position.x;
	float dy = target->m_Position.y - pLocalPlayer->m_Position.y;
	double yaw = atan2(dy, dx) * 100 / M_PI;

	// calculate pitch
	double distance = sqrt(dx * dx + dy * dy);
	float dz = target->m_Position.z - pLocalPlayer->m_Position.z;
	double pitch = atan2(dz, distance) * 100 / M_PI;

	// aim at calculated target
	pLocalPlayer->m_Yaw = (float)yaw + 90.f;
	pLocalPlayer->m_Pitch = (float)pitch;
}

void GetClosestEnemyToCrossHair(playerent* pLocalPlayer)
{
	//find first living enemy player in view
	Vector2 targetPos = new Vector2();
	Player target = players.Find(p = > p.Team != self.Team && p.Health > 0
		&& viewMatrix.WorldToScreen(p.PositionHead, gameWidth, gameHeight, out targetPos));
	if (target == null) return null;

	//calculate distance to crosshair
	Vector2 crossHair = new Vector2(gameWidth / 2, gameHeight / 2);
	float dist = crossHair.Distance(targetPos);

	//find player closest to crosshair
	foreach(Player p in players) {
		if (p.Team != self.Team && p.Health > 0) {
			Vector2 headPos;
			if (viewMatrix.WorldToScreen(p.PositionHead, gameWidth, gameHeight, out headPos)) {
				float newDist = crossHair.Distance(headPos);
				if (newDist < dist) {
					target = p;
					dist = newDist;
				}
			}
		}
	}

	return target;
}
*/