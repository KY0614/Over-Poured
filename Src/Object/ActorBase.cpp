#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "ActorBase.h"

ActorBase::ActorBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
}

ActorBase::~ActorBase(void)
{
}

const Transform& ActorBase::GetTransform(void) const
{
	return transform_;
}
