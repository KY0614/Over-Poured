#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "CupLid.h"

CupLid::CupLid(const std::string objId, const float width,
    const float height, const float depth, Player& player,
    StageObject& object) :
    StageObject(objId, width, height, depth, player),coffees_(object)
{
}

void CupLid::Update(void)
{
    VECTOR pos = coffees_.GetTransform().pos;
    pos.y += coffees_.GetObjHeight();

    transform_.pos = pos;

    transform_.Update();
}
