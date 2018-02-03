/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/gamemodes/crap.h>
#include "chest.h"

CChest::CChest(CGameWorld *pGameWorld, int Index, vec2 Pos, int Number, int Time)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Index = Index;
	m_Pos = Pos;
	m_Number = Number;
	m_Time = Time;
	m_State = true;
	GameWorld()->InsertEntity(this);
	m_StartPoints = 10*50;
	m_HP = m_StartPoints;
	this->m_ID2 = Server()->SnapNewID(); //4 Laser
	this->m_ID3 = Server()->SnapNewID();
	this->m_ID4 = Server()->SnapNewID();
	this->m_ID5 = Server()->SnapNewID();// and a dot
}

void CChest::Tick()
{
	//compaire it with the gamecontroller state
	m_State = GameServer()->m_pController->m_Chest[m_Number];
	if(!m_State || GameServer()->m_pController->m_Warmup)
		return;
	
	CCharacter *apCloseCCharacters = GameServer()->m_World.ClosestCharacter(m_Pos, 8.0f, 0);
	if(!apCloseCCharacters || !apCloseCCharacters->IsAlive() || apCloseCCharacters->GetPlayer()->GetTeam() == TEAM_SPECTATORS || GameServer()->Collision()->IntersectLine(m_Pos, apCloseCCharacters->m_Pos, NULL, NULL))
	{
		GameServer()->m_pController->m_Destroy = false;
		return;
	}
	char aBuf[128];
	if(m_HP <= 0)//Resetting in the next tick! otherwise chat bug
		m_HP = m_StartPoints;//For the Resetting or it will make bugs
	if(apCloseCCharacters->GetPlayer()->GetTeam() == TEAM_RED)
	{
		GameServer()->m_pController->m_Destroy = true;

		m_HP--;
		str_format(aBuf, sizeof(aBuf), "Chest %d has %d %% Damage taken!", m_Number, (m_StartPoints-m_HP)*100/m_StartPoints);
		GameServer()->CreateSound(m_Pos, SOUND_WEAPON_NOAMMO);
		if(m_HP <= 0)
		{
			str_format(aBuf, sizeof(aBuf), "Chest %d got destroyed by %s", m_Number, Server()->ClientName(apCloseCCharacters->GetPlayer()->GetCID()));
			apCloseCCharacters->GetPlayer()->m_Score = apCloseCCharacters->GetPlayer()->m_Score+10;
			GameServer()->CreateSoundGlobal(SOUND_CTF_CAPTURE);
			GameServer()->m_pController->m_Door[m_Number] = false;//Open the Door
			GameServer()->m_pController->m_Chest[m_Number] = false;//For the win conditions
			GameServer()->m_pController->m_ChestTime = GameServer()->m_pController->m_ChestTime + m_Time;
			//Make a Time message!! 
			char bBuf[128];
			str_format(bBuf, sizeof(bBuf), "--- Added %d seconds to the timelimit", m_Time);
			GameServer()->SendChat(-1, CGameContext::CHAT_ALL, bBuf);
		}
	}
	if(m_HP == m_StartPoints)//Broadcast Bugfix otherwive aBuf will be send with no message
		return;
	if(apCloseCCharacters->GetPlayer()->GetTeam() == TEAM_BLUE && (m_HP < m_StartPoints))
	{
		m_HP++;
		str_format(aBuf, sizeof(aBuf), "Chest %d has %d %% Damage left!", m_Number, (m_StartPoints-m_HP)*100/m_StartPoints);
		GameServer()->CreateSound(m_Pos, SOUND_HOOK_NOATTACH);
		if(m_HP == m_StartPoints)
		{
			str_format(aBuf, sizeof(aBuf), "Chest %d got repaired by %s", m_Number, Server()->ClientName(apCloseCCharacters->GetPlayer()->GetCID()));
			GameServer()->CreateSoundGlobal(SOUND_CTF_RETURN);
			apCloseCCharacters->GetPlayer()->m_Score = apCloseCCharacters->GetPlayer()->m_Score+5;
		}	
	}	
	//Send the Message
	GameServer()->SendBroadcast(aBuf, -1);
}

void CChest::Snap(int SnappingClient)
{

		/*
		CNetObj_Flag *pFlag = (CNetObj_Flag *)Server()->SnapNewItem(NETOBJTYPE_FLAG, 1, sizeof(CNetObj_Flag));
		if(!pFlag)
			return;

		pFlag->m_X = (int)m_Pos.x;
		pFlag->m_Y = (int)m_Pos.y;
		pFlag->m_Team = 1;
		*/
	
	//Old Chest
	
	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	pObj->m_X = (int)m_Pos.x-15;
	pObj->m_Y = (int)m_Pos.y-15;
	pObj->m_FromX = (int)m_Pos.x+15;
	pObj->m_FromY = (int)m_Pos.y-15;
	pObj->m_StartTick = Server()->Tick()-1;
	
	CNetObj_Laser *pObj2 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID2, sizeof(CNetObj_Laser)));
	pObj2->m_X = (int)m_Pos.x+15;
	pObj2->m_Y = (int)m_Pos.y-15;
	pObj2->m_FromX = (int)m_Pos.x+15;
	pObj2->m_FromY = (int)m_Pos.y+15;
	pObj2->m_StartTick = Server()->Tick()-1;
	
	CNetObj_Laser *pObj3 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID3, sizeof(CNetObj_Laser)));
	pObj3->m_X = (int)m_Pos.x+15;
	pObj3->m_Y = (int)m_Pos.y+15;
	pObj3->m_FromX = (int)m_Pos.x-15;
	pObj3->m_FromY = (int)m_Pos.y+15;
	pObj3->m_StartTick = Server()->Tick()-1;
		
	CNetObj_Laser *pObj4 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID4, sizeof(CNetObj_Laser)));
	pObj4->m_X = (int)m_Pos.x-15;
	pObj4->m_Y = (int)m_Pos.y+15;
	pObj4->m_FromX = (int)m_Pos.x-15;
	pObj4->m_FromY = (int)m_Pos.y-15;
	pObj4->m_StartTick = Server()->Tick()-1;

	if(!m_State)
	{
		pObj->m_FromX = (int)m_Pos.x-15;
		pObj->m_FromY = (int)m_Pos.y-15;
		pObj2->m_FromX = (int)m_Pos.x+15;
		pObj2->m_FromY = (int)m_Pos.y-15;
		pObj3->m_FromX = (int)m_Pos.x+15;
		pObj3->m_FromY = (int)m_Pos.y+15;
		pObj4->m_FromX = (int)m_Pos.x-15;
		pObj4->m_FromY = (int)m_Pos.y+15;
	}
	else
	{
		CNetObj_Laser *pObj5 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID5, sizeof(CNetObj_Laser)));
		pObj5->m_X = (int)m_Pos.x-15;
		pObj5->m_Y = (int)m_Pos.y-15;
		pObj5->m_FromX = (int)m_Pos.x-15;
		pObj5->m_FromY = (int)m_Pos.y-15;
		pObj5->m_StartTick = Server()->Tick()-1;
	}
	//new Chest
	/*
	CNetObj_Flag *pFlag = (CNetObj_Flag *)Server()->SnapNewItem(NETOBJTYPE_FLAG, 0, sizeof(CNetObj_Flag));
	if(!pFlag)
		return;

	pFlag->m_X = (int)m_Pos.x;
	pFlag->m_Y = (int)m_Pos.y;
	pFlag->m_Team = 0;
	*/
}
