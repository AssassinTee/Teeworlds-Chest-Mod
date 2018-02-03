#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/gamemodes/crap.h>
#include "laser_door.h"

CDoorLaser::CDoorLaser(CGameWorld *pGameWorld, vec2 From, vec2 To, int Number)
: CEntity(pGameWorld, NETOBJTYPE_LASER)
{
	m_From = From;
	m_Pos = To;
	m_Number = Number;
	GameWorld()->InsertEntity(this);
	this->m_ID2 = Server()->SnapNewID();
}

void CDoorLaser::Tick()
{
	m_State = GameServer()->m_pController->m_Door[m_Number];
	if(!m_State)
		return;
	vec2 At;
	CCharacter *Hit[MAX_CLIENTS] = {0};

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		Hit[i] = GameServer()->m_World.LaserIntersectCharacter(m_From, m_Pos, 2.5f, At, Hit);
		if(Hit[i])
		{
			Hit[i]->m_HittingDoor = true;
			Hit[i]->m_PushDirection = normalize(Hit[i]->m_OldPos - At);
		}
	}
}

void CDoorLaser::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient) && distance(m_From, m_Pos) < 1600.0f && (!GameServer()->m_apPlayers[SnappingClient] || distance(m_From, GameServer()->m_apPlayers[SnappingClient]->m_ViewPos) > 800.0f))
		return;

	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_FromX = (int)m_From.x;
	pObj->m_FromY = (int)m_From.y;
	if(!m_State)
	{
		pObj->m_FromX = (int)m_Pos.x;
		pObj->m_FromY = (int)m_Pos.y;
	}
	pObj->m_StartTick = Server()->Tick();
	
	CNetObj_Laser *pObj2 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID2, sizeof(CNetObj_Laser)));
	pObj2->m_X = (int)m_From.x;
	pObj2->m_Y = (int)m_From.y;
	pObj2->m_FromX = (int)m_From.x;
	pObj2->m_FromY = (int)m_From.y;
	pObj2->m_StartTick = Server()->Tick();
}
