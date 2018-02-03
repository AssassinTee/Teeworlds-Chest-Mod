#ifndef GAME_SERVER_ENTITIES_LASER_H
#define GAME_SERVER_ENTITIES_LASER_H

#include <game/server/entity.h>
/**********************************************************************************************
	Door States:
				false	= Open
				true	= Closed
***********************************************************************************************/
class CDoorLaser : public CEntity
{
	public:
	CDoorLaser(CGameWorld *pGameWorld, vec2 From, vec2 To, int Number);

	virtual void Snap(int SnappingClient);
	virtual void Tick();
	bool m_State;
	int m_Number;
	int m_ID2;
	vec2 m_From;
	vec2 m_Pos;
};

#endif
