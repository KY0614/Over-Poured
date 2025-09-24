#include<DxLib.h>
#include<cassert>
#include "Input.h"

namespace {
	//ヘッダ部
	struct Header {
		char signature[4];
		float version;
		uint32_t dataSize;//4バイト
	};

	//アナログスティックのしきい値
	const int MAX_ANALOG_STICK = 10000;	//スティックの最大値
	const int MAX_ANALOG_TRIGGER = 128;	//トリガーの最大値
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
	//containsはキーがあるかどうか
	if (!currentInput_.contains(eventcode))return false;

	//atは読み取り専用的な、
	return currentInput_.at(eventcode) && !lastInput_.at(eventcode);
}

bool Input::IsPressed(const std::string& eventcode) const
{
	if (!currentInput_.contains(eventcode)) return false;
	return currentInput_.at(eventcode);
}

void Input::Update(void)
{
	//押した顔してないか記録する部分
	lastInput_ = currentInput_;//前のプッシュ情報を記憶

	//キーボード情報
	char keyState[256] = {};
	GetHitKeyStateAll(keyState);

	//パッド情報
	int padState = GetJoypadInputState(DX_INPUT_PAD1);

	//マウス情報
	int mouseState = GetMouseInput();

	//アナログ情報
	XINPUT_STATE xinputState = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState);

	//テーブルの行を回す
	for (const auto& keyvalue : inputTable_)
	{
		//特定のキーの入力情報を取得
		for (auto input : keyvalue.second)
		{
			bool pressed = false;
			//キーの種類によって処理を分ける
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
	//入力テーブルの初期化
	inputTable_ = {
		
	{"pause",	//ポーズ
		{
		{PeripheralType::KEYBOARD,KEY_INPUT_P},
		{PeripheralType::GAMEPAD,PAD_INPUT_R}}	//STARTボタン
		},
	};
	//ダッシュ
	inputTable_["Dash"] = { {PeripheralType::KEYBOARD,KEY_INPUT_LCONTROL},
							{PeripheralType::GAMEPAD, PAD_INPUT_Y},
							{PeripheralType::X_ANALOG,(int)AnalogInputType::L_TRIGGER} };
	//インタラクト
	inputTable_["Interact"] = { {PeripheralType::KEYBOARD,KEY_INPUT_SPACE},
							{PeripheralType::GAMEPAD, PAD_INPUT_B} };
	//上下左右
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