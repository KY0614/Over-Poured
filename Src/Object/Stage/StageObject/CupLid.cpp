#include "../Manager/Generic/InputManager.h"
#include "../Object/Common/Sphere.h"
#include "../Object/Player.h"
#include "CupLid.h"

CupLid::CupLid(const std::string objId, const float width,
    const float height, const float depth, Player& player) :
    StageObject(objId, width, height, depth, player)
{
}

void CupLid::Update(void)
{
    SetFollowPos();
    transform_.Update();
}
