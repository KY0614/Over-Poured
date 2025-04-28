#pragma once
#include<string>

class StringUtility
{
public:

	/// <summary>
	/// string��wstring�ɕϊ�
	/// </summary>
	/// <param name="str">string�̕�����</param>
	/// <returns>wstring�^�̕�����</returns>
	static std::wstring StringToWstring(const std::string& str);

	/// <summary>
	/// wstring��string�ɕϊ�
	/// </summary>
	/// <param name="wstr">wstring�̕�����</param>
	/// <returns>string�̕�����</returns>
	static std::string WstringToString(const std::wstring& wstr);

private:

};

