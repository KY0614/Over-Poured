//#pragma once
#include <DxLib.h>
#include <string>
#include"../Application.h"
#include"../Utility/StringUtility.h"

class DebugDrawFormat
{
public:
    //�R���X�g���N�^
    DebugDrawFormat(void);

    // �R���X�g���N�^
    DebugDrawFormat(int startX = 0, int startY = 0, int textColor = GetColor(255, 255, 255), int lineHeight = 20, int startLine = 0)
        : x(startX), y(startY), color(textColor),lineHeight(lineHeight) {}

    // �f�o�b�O�p�ɕ�������o�͂��郁�\�b�h
    void String(const std::string& str) 
    {
        DrawString(x, y, StringUtility::StringToWstring(str).c_str(), 0xFFFFFF);
        y += lineHeight; // ���̍s�Ɉړ�
    }


    /// <summary>
    /// �\������l���P�܂ł̃f�o�b�O�pDrawFormatString�𐶐�����
    /// </summary>
    /// <typeparam name="Value">�l�̌^</typeparam>
    /// <param name="format">�\�����镶����ƒl</param>
    /// <param name="val">�\������l</param>
    /// <param name="line">�s��</param>
    /// <param name="lineH">�s�̍���</param>
    template <typename Value>
    static void FormatString(const TCHAR* format, const Value val, int& line, const int lineH = 20)
    {
        DrawFormatString(0, line * lineH, 0xFFFFFF, format, val);
        line++;
    }

    /// <summary>
	/// �\������l���P�܂ł�DrawFormatString����ʉE�[�ɐ�������
    /// </summary>
    /// <typeparam name="Value">�l�̌^</typeparam>
    /// <param name="format">�\�����镶����ƒl</param>
    /// <param name="val">�\������l</param>
    /// <param name="line">�s��</param>
    /// <param name="lineH">�s�̍���</param>
    template <typename Value>
    static void FormatStringRight(const TCHAR* format, const Value val, int& line, const int lineH = 20)
    {
        int strWidth = GetDrawFormatStringWidth(format, val);   //������̕����擾
        int screenWidth = Application::SCREEN_SIZE_X;           //��ʂ̕�
        int rightX = screenWidth - strWidth;                    //X���W���v�Z
        DrawFormatString(rightX, line * lineH, 0xFFFFFF, format, val);
        line++;
    }

    /// <summary>
	/// ������̕����擾(�l��1�܂�)
    /// </summary>
    /// <typeparam name="Value">�l�̌^</typeparam>
    /// <param name="format">�擾���镶����</param>
    /// <param name="val">�\�������l</param>
    /// <param name="line">�s��</param>
    /// <returns>������̕�</returns>
    template <typename Value>
    static int GetFormatSize(const TCHAR* format, const Value val, const int line)
    {
        return GetDrawFormatStringWidth(format, val);
    }

    /// <summary>
	/// �l��2�܂ł̃f�o�b�O�pDrawFormatString�𐶐�����
    /// </summary>
    /// <typeparam name="ValA">�l�̌^</typeparam>
    /// <typeparam name="ValB">�l�̌^</typeparam>
    /// <param name="format">�\�����镶����ƒl</param>
    /// <param name="valA">�\������l</param>
    /// <param name="valB">�\������l</param>
    /// <param name="line">�s��</param>
    /// <param name="lineH">�s�̍���</param>
    template <typename ValA, typename ValB>
    static void FormatString(const TCHAR* format, const ValA valA, const ValB valB , int& line, const int lineH = 20)
    {
        DrawFormatString(0, line * lineH, 0xFFFFFF, format, valA, valB);
        line++;
    }

    /// <summary>
	/// �l��2�܂ł�DrawFormatString����ʉE�[�ɐ�������
    /// </summary>
    /// <typeparam name="ValA">�l�̌^</typeparam>
    /// <typeparam name="ValB">�l�̌^</typeparam>
    /// <param name="format">�\�����镶����ƒl</param>
    /// <param name="valA">�\������l</param>
    /// <param name="valB">�\������l</param>
    /// <param name="line">�s��</param>
    /// <param name="lineH">�s�̍���</param>
    template <typename ValA, typename ValB>
    static void FormatStringRight(const TCHAR* format, const ValA valA, const ValB valB, int& line, const int lineH = 20)
    {
        int strWidth = GetDrawFormatStringWidth(format, valA, valB); // ������̕����擾
        int screenWidth = Application::SCREEN_SIZE_X; // ��ʂ̕� (�K�X�ύX���Ă�������)
        int rightX = screenWidth - strWidth; // X���W���v�Z
        DrawFormatString(rightX, line * lineH, 0xFFFFFF, format, valA, valB);
        line++;
    }

    /// <summary>
    /// ������̕����擾(�l��2�܂�)
    /// </summary>
    /// <typeparam name="ValA">�l�̌^</typeparam>
    /// <typeparam name="ValB">�l�̌^</typeparam>
    /// <param name="format">�擾���镶����</param>
    /// <param name="valA">�\�������l</param>
    /// <param name="valB">�\�������l</param>
    /// <returns>������̕�</returns>
    template <typename ValA, typename ValB>
    static int GetFormatSize(const TCHAR* format, const ValA valA, const ValB valB)
    {
        return GetDrawFormatStringWidth(format, valA, valB);
    }

private:

    int x;          // X���W
    int y;          // Y���W
    int color;      // �F
    int lineHeight; // �s�̍���
};
