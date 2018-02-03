/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_CHEST_H
#define GAME_SERVER_ENTITIES_CHEST_H

#include <game/server/entity.h>
/**********************************************************************************************
	Chest States:
				false	= Desroyed
				true	= Undestroyed
***********************************************************************************************/
class CChest : public CEntity
{
public:
	CChest(CGameWorld *pGameWorld, int Index, vec2 Pos, int Number, int Time);

	virtual void Tick();
	virtual void Snap(int SnappingClient);

private:
	int m_Index;
	vec2 m_Pos;
	int m_Number;
	int m_Time;
	bool m_State;
	int m_StartPoints;
	int m_HP;
	int m_ID2;
	int m_ID3;
	int m_ID4;
	int m_ID5;
	bool m_Player[MAX_CLIENTS];
};

#endif
