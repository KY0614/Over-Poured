#include<DxLib.h>
#include<cassert>
#include "Input.h"

namespace {
	//�w�b�_��
	struct Header {
		char signature[4];
		float version;
		uint32_t dataSize;//4�o�C�g
	};

	//�A�i���O�X�e�B�b�N�̂������l
	const int MAX_ANALOG_STICK = 10000;	//�X�e�B�b�N�̍ő�l
	const int MAX_ANALOG_TRIGGER = 128;	//�g���K�[�̍ő�l
}

Input::Input()
{
	ResetTable();

	analogInputTable_[AnalogInputType::L_UP] = [](const XINPUT_STATE& state) {
		return state.ThumbLY > MAX_ANALOG_STICK;
	};	
	analogInputTable_[AnalogInputType::L_DOWN] = [](const XINPUT_STATE& state) {
		return state.ThumbLY < -MAX_ANALOG_STICK;
	};
	analogInputTable_[AnalogInputType::L_RIGHT] = [](const XINPUT_STATE& state) {
		return state.ThumbLX > MAX_ANALOG_STICK;
	};
	analogInputTable_[AnalogInputType::L_LEFT] = [](const XINPUT_STATE& state) {
		return state.ThumbLX < -MAX_ANALOG_STICK;
	};
	analogInputTable_[AnalogInputType::R_UP] = [](const XINPUT_STATE& state) {
		return state.ThumbRY > MAX_ANALOG_STICK;
	};
	analogInputTable_[AnalogInputType::R_DOWN] = [](const XINPUT_STATE& state) {
		return state.ThumbRY < -MAX_ANALOG_STICK;
	};
	analogInputTable_[AnalogInputType::R_LEFT] = [](const XINPUT_STATE& state) {
		return state.ThumbRX > MAX_ANALOG_STICK;
	};
	analogInputTable_[AnalogInputType::R_RIGHT] = [](const XINPUT_STATE& state) {
		return state.ThumbRX < -MAX_ANALOG_STICK;
	};
	analogInputTable_[AnalogInputType::L_TRIGGER] = [](const XINPUT_STATE& state) {
		return state.LeftTrigger > MAX_ANALOG_TRIGGER;
	};
	analogInputTable_[AnalogInputType::R_TRIGGER] = [](const XINPUT_STATE& state) {
		return state.RightTrigger > MAX_ANALOG_TRIGGER;
	};

	for (const auto& keyvalue : inputTable_)
	{
		currentInput_[keyvalue.first] = false;
	}
	lastInput_ = currentInput_;
}

bool Input::IsTrigerred(const std::string& eventcode) const
{
	//contains�̓L�[�����邩�ǂ���
	if (!currentInput_.contains(eventcode))return false;

	//at�͓ǂݎ���p�I�ȁA
	return currentInput_.at(eventcode) && !lastInput_.at(eventcode);
}

bool Input::IsPressed(const std::string& eventcode) const
{
	if (!currentInput_.contains(eventcode)) return false;
	return currentInput_.at(eventcode);
}

void Input::Update(void)
{
	//�������炵�ĂȂ����L�^���镔��
	lastInput_ = currentInput_;//�O�̃v�b�V�������L��

	//�L�[�{�[�h���
	char keyState[256] = {};
	GetHitKeyStateAll(keyState);

	//�p�b�h���
	int padState = GetJoypadInputState(DX_INPUT_PAD1);

	//�}�E�X���
	int mouseState = GetMouseInput();

	//�A�i���O���
	XINPUT_STATE xinputState = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);

	//�e�[�u���̍s����
	for (const auto& keyvalue : inputTable_)
	{
		//����̃L�[�̓��͏����擾
		for (auto input : keyvalue.second)
		{
			bool pressed = false;
			//�L�[�̎�ނɂ���ď����𕪂���
			if (input.type == PeripheralType::KEYBOARD)
			{
				pressed = keyState[input.code];
			}
			else if (input.type == PeripheralType::GAMEPAD)
			{
				pressed = padState&input.code;
			}
			else if (input.type == PeripheralType::MOUSE)
			{
				pressed = mouseState & input.code;
			}
			else if (input.type == PeripheralType::X_ANALOG)
			{
				pressed = analogInputTable_[(AnalogInputType)input.code](xinputState);
			}
			currentInput_[keyvalue.first] = pressed;
			if (pressed)break;
		}
	}
}

void Input::ResetTable()
{
	//���̓e�[�u���̏�����
	inputTable_ = {
		
	{"pause",	//�|�[�Y
		{
		{PeripheralType::KEYBOARD,KEY_INPUT_P},
		{PeripheralType::GAMEPAD,PAD_INPUT_R}}	//START�{�^��
		},
	};
	//�_�b�V��
	inputTable_["Dash"] = { {PeripheralType::KEYBOARD,KEY_INPUT_LCONTROL},
							{PeripheralType::GAMEPAD, PAD_INPUT_Y},
							{PeripheralType::X_ANALOG,(int)AnalogInputType::L_TRIGGER} };
	//�C���^���N�g
	inputTable_["Interact"] = { {PeripheralType::KEYBOARD,KEY_INPUT_SPACE},
							{PeripheralType::GAMEPAD, PAD_INPUT_B} };
	//�㉺���E
	inputTable_["Up"] = {	{PeripheralType::KEYBOARD,KEY_INPUT_UP},
							{PeripheralType::GAMEPAD,PAD_INPUT_UP},
							{PeripheralType::X_ANALOG,(int)AnalogInputType::L_UP} };
	inputTable_["Down"] = { {PeripheralType::KEYBOARD,KEY_INPUT_DOWN},
							{PeripheralType::GAMEPAD,PAD_INPUT_DOWN},
							{PeripheralType::X_ANALOG,(int)AnalogInputType::L_DOWN} };
	inputTable_["Right"] = {{PeripheralType::KEYBOARD,KEY_INPUT_RIGHT},
							{PeripheralType::GAMEPAD,PAD_INPUT_RIGHT},
							{PeripheralType::X_ANALOG,(int)AnalogInputType::L_RIGHT} };
	inputTable_["Left"] = { {PeripheralType::KEYBOARD,KEY_INPUT_LEFT},
							{PeripheralType::GAMEPAD,PAD_INPUT_LEFT},
							{PeripheralType::X_ANALOG,(int)AnalogInputType::L_LEFT} };
}