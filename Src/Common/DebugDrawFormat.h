//#pragma once
#include <DxLib.h>
#include <string>

class DebugDrawFormat
{
public:
    //�R���X�g���N�^
    DebugDrawFormat(void);

    // �R���X�g���N�^
    DebugDrawFormat(int startX = 0, int startY = 0, int textColor = GetColor(255, 255, 255), int lineHeight = 20)
        : x(startX), y(startY), color(textColor), lineHeight(lineHeight) {}

    // �f�o�b�O�p�ɕ�������o�͂��郁�\�b�h
    void String(const std::string& str) {
        DrawString(x, y, str.c_str(), color);
        y += lineHeight; // ���̍s�Ɉړ�
    }

    //�l���P�܂ł�DrawFormat
    template <typename Value>
    static void FormatString(const char* format, const Value val, const int line, const int lineH = 20)
    {
        DrawFormatString(0, line * lineH,0xffffff, format, val);
    }

    template <typename Value>
    static int GetFormatSize(const char* format, const Value val, const int line)
    {
        return GetDrawFormatStringWidth(format, val);
    }

    //�l���Q�܂ł�DrawFormat
    template <typename ValA,typename ValB>
    static void FormatString(const char* format, const ValA valA, const ValB valB , const int line, const int lineH = 20)
    {
        DrawFormatString(0, line * lineH, 0xffffff, format, valA, valB);
    }

    template <typename ValA, typename ValB>
    static int GetFormatSize(const char* format, const ValA valA, const ValB valB)
    {
        return GetDrawFormatStringWidth(format, valA, valB);
    }

private:

    int x;          // X���W
    int y;          // Y���W
    int color;      // �F
    int lineHeight; // �s�̍���
};
