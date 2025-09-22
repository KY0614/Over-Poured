#pragma once

#include<DxLib.h>		//�Ή��\�̂���
#include<unordered_map>		//�Ή��\�̂���
#include<functional>		//�Ή��\�̂���
#include<string>	//�Ή��L�[�̂���
#include<vector>	//�f�o�C�X�ǉ��̂���

/// <summary>
/// ���Ӌ@����
/// </summary>
enum class PeripheralType {
	KEYBOARD,	//�L�[�{�[�h
	GAMEPAD,	//�Q�[���p�b�h
	MOUSE,		//�}�E�X
	X_ANALOG,	//Xbox�R���g���[���[�̃A�i���O����
	END			//for���ŉ񂷂Ƃ��p�ɒ�`
};

/// <summary>
/// �A�i���O���͎��
/// </summary>
enum class AnalogInputType {
	NONE,	//�����ĂȂ�
	L_UP,	//���X�e�B�b�N�̏�
	L_DOWN,	//���X�e�B�b�N�̉�
	L_LEFT,	//���X�e�B�b�N�̍�
	L_RIGHT,//���X�e�B�b�N�̉E
	R_UP,	//�E�X�e�B�b�N�̏�
	R_DOWN,	//�E�X�e�B�b�N�̉�
	R_LEFT,	//�E�X�e�B�b�N�̍�
	R_RIGHT,//�E�X�e�B�b�N�̉E
	L_TRIGGER,
	R_TRIGGER
};

class KeyConfigScene;

class Input
{
	friend KeyConfigScene;	//private�������Ă�����

public:
	//�R���X�g���N�^
	Input();

	/// <summary>
	///	�w�肳�ꂽ�R�[�h�������ꂽ�u�Ԃ�
	/// </summary>
	/// <param name="eventcode">�Ώۂ̃C�x���g�R�[�h</param>
	/// <returns>�������u��:true / false :������Ă��Ȃ�</returns>
	bool IsTrigerred(const std::string& eventcode)const;

	/// <summary>
	/// �w�肳�ꂽ�R�[�h��������Ă���Ԃ�
	/// </summary>
	/// <param name="eventcode">�Ώۂ̃C�x���g�R�[�h</param>
	/// <returns>������Ă����:true / false :������Ă��Ȃ�</returns>
	bool IsPressed(const std::string& eventcode)const;

	void Update(void);//���͏�Ԃ��X�V����

private:

	struct InputState {
		PeripheralType type;	//���Ӌ@����
		uint32_t code;			//���̓R�[�h(�ėp)
	};

	using InputTable_t = std::unordered_map<std::string, std::vector<InputState>>;
	InputTable_t inputTable_;	//�C�x���g�Ǝ��ۂ̓��͂̑Ή��\
	InputTable_t tmpInputTable_;//�ꎞ�I�ȓ��̓e�[�u��

	using InputData_t = std::unordered_map<std::string, bool>;
	InputData_t currentInput_;	//���̃C�x���g�ɑΉ�����{�^����������Ă����Ԃ�
	InputData_t lastInput_;		//���̃C�x���g�ɑΉ�����{�^����������Ă����Ԃ�(���O)

	using AnalogInputTable_t = 
		std::unordered_map < AnalogInputType, std::function<bool(const XINPUT_STATE&)>>;

	AnalogInputTable_t analogInputTable_;


	/// <summary>
	/// ���̓e�[�u���̃Z�[�u
	/// </summary>
	void Save();

	void Load();

	void ResetTable();
};

