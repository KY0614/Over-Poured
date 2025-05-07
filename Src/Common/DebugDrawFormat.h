//#pragma once
#include <DxLib.h>
#include <string>
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
    void String(const std::string& str) {
        DrawString(x, y, StringUtility::StringToWstring(str).c_str(), 0xFFFFFF);
        y += lineHeight; // ���̍s�Ɉړ�
    }

    //�l���P�܂ł�DrawFormat
    template <typename Value>
    static void FormatString(const TCHAR* format, const Value val, int& line, const int lineH = 20)
    {
        DrawFormatString(0, line * lineH, 0xFFFFFF, format, val);
        line++;
    }

    template <typename Value>
    static void FormatStringRight(const TCHAR* format, const Value val, int& line, const int lineH = 20)
    {
        int strWidth = GetDrawFormatStringWidth(format, val); // ������̕����擾
        int screenWidth = 1024; // ��ʂ̕� (�K�X�ύX���Ă�������)
        int rightX = screenWidth - strWidth; // X���W���v�Z
        DrawFormatString(rightX, line * lineH, 0xFFFFFF, format, val);
        line++;
    }

    template <typename Value>
    static int GetFormatSize(const TCHAR* format, const Value val, const int line)
    {
        return GetDrawFormatStringWidth(format, val);
    }

    //�l���Q�܂ł�DrawFormat
    template <typename ValA,typename ValB>
    static void FormatString(const TCHAR* format, const ValA valA, const ValB valB , int& line, const int lineH = 20)
    {
        DrawFormatString(0, line * lineH, 0xFFFFFF, format, valA, valB);
        line++;
    }

    template <typename ValA, typename ValB>
    static void FormatStringRight(const TCHAR* format, const ValA valA, const ValB valB, int& line, const int lineH = 20)
    {
        int strWidth = GetDrawFormatStringWidth(format, valA, valB); // ������̕����擾
        int screenWidth = 1024; // ��ʂ̕� (�K�X�ύX���Ă�������)
        int rightX = screenWidth - strWidth; // X���W���v�Z
        DrawFormatString(rightX, line * lineH, 0xFFFFFF, format, valA, valB);
        line++;
    }

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
