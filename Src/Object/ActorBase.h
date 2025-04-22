#pragma once
#include "Common/Transform.h"
class ResourceManager;
class SceneManager;

class ActorBase
{

public:

	//�R���X�g���N�^
	ActorBase(void);

	//�f�X�g���N�^
	virtual ~ActorBase(void);

	//���
	virtual void Destroy(void);

	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);

	const Transform& GetTransform(void) const;

protected:

	//�V���O���g���Q��
	SceneManager& scnMng_;

	//���f������̊�{���
	Transform transform_;

};
