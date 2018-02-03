#ifndef GAME_SERVER_ENTITIES_DOOR_H
#define GAME_SERVER_ENTITIES_DOOR_H

#include <game/server/entity.h>
/**********************************************************************************************
	Door States:
				false	= Open
				true	= Closed
***********************************************************************************************/
class CeDoor : public CEntity
{
public:
	CeDoor(CGameWorld *pGameWorld, int Index, vec2 Pos, int Number);

	virtual void Tick();
	virtual void Snap(int SnappingClient);

private:
	int m_Index;
	vec2 m_Pos;
	int m_Number;
	bool m_State;
};

#endif
