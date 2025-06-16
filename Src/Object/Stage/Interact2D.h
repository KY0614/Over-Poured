#pragma once
#include <memory>
#include <map>
#include <functional>

class Interact2D
{
public:

	enum class MODE {
		GAME_3D,		// 3D�Q�[�����[�h
		MACHINE_2D,		// 2D�}�V�����[�h
		ICE_2D,			// 2D���X�@���[�h
		LIDRACK_2D,		// 2D���b�h���b�N���[�h
	};

	Interact2D();
	~Interact2D();

	void Init(void);
	void Update(void);
	void Draw(void);

	//��ԑJ��
	void ChangeMode(MODE mode);

private:

	//���[�h�Ǘ�
	MODE mode_;

	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<MODE, std::function<void(void)>> stateChanges_;

	//��ԊǗ�
	std::map<MODE, std::function<void(void)>> updateFunc_;
	std::map<MODE, std::function<void(void)>> drawFunc_;

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> modeUpdate_;
	std::function<void(void)> modeDraw_;

	int frame_;

	//��ԑJ��
	void ChangeMode3DGame(void);
	void ChangeModeMachine2D(void);
	void ChangeModeIce2D(void);
	void ChangeModeLidRack2D(void);

	//�X�V�X�e�b�v
	void AppearUpdate(void);
	void DisappearUpdate(void);
	void Update3DGame(void);
	void UpdateMachine2D(void);
	void UpdateIce2D(void);
	void UpdateLidRack2D(void);

	//�`��X�e�b�v
	void ProcessDraw(void);
	void Draw3DGame(void);
	void DrawMachine2D(void);
	void DrawIce2D(void);
	void DrawLidRack2D(void);
};