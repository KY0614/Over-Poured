#include "CupLid.h"

CupLid::CupLid(const std::string objId, const float width,
	const float height, const float depth, Player& player) :
	StageObject(objId, width, height, depth, player)
{
}

void CupLid::Interact(const std::string& objId)
{

}