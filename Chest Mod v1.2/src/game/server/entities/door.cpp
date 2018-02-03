#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/gamemodes/crap.h>
#include "door.h"

CeDoor::CeDoor(CGameWorld *pGameWorld, int Index, vec2 Pos, int Number)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP)
{
	m_Index = Index;
	m_Pos = Pos;
	m_Number = Number;
	m_State = true;
	GameWorld()->InsertEntity(this);
}

void CeDoor::Tick()
{
	//compaire it with the gamecontroller state
	m_State = GameServer()->m_pController->m_Door[m_Number];
	if(!m_State)
		return;
	CCharacter *apCloseCCharacters[MAX_CLIENTS];
	int Num = GameServer()->m_World.FindEntities(m_Pos, 8.0f, (CEntity**)apCloseCCharacters, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER);
	for(int i = 0; i < Num; i++)
	{
		if(!apCloseCCharacters[i]->IsAlive() || apCloseCCharacters[i]->GetPlayer()->GetTeam() == TEAM_SPECTATORS || GameServer()->Collision()->IntersectLine(m_Pos, apCloseCCharacters[i]->m_Pos, NULL, NULL))
			continue;
		apCloseCCharacters[i]->m_HittingDoor = true;
		apCloseCCharacters[i]->m_PushDirection = normalize(apCloseCCharacters[i]->m_OldPos - m_Pos);
		//apCloseCCharacters[i]->m_Ninja.m_CurrentMoveTime = 0; I must add a command for this COMMAND
	}
}

void CeDoor::Snap(int SnappingClient)
{
	if(!m_State)
		return;

	CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pP)
		return;

	pP->m_X = (int)m_Pos.x;
	pP->m_Y = (int)m_Pos.y;
	pP->m_Type = POWERUP_ARMOR;
	pP->m_Subtype = 0;
}
