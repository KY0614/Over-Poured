#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "CupLid.h"

CupLid::CupLid(const std::string objId, const float width,
    const float height, const float depth, Player& player,
    StageObject& object) :
    StageObject(objId, width, height, depth, player),coffee_(object)
{
}

void CupLid::Update(void)
{
    VECTOR pos = coffee_.GetTransform().pos;
    pos.y += coffee_.GetObjHeight();

    transform_.pos = pos;

    transform_.Update();
}
