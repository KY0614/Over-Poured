//#pragma once
#include <DxLib.h>
#include <string>
#include"../Application.h"
#include"../Utility/StringUtility.h"

class DebugDrawFormat
{
public:
    //コンストラクタ
    DebugDrawFormat(void);

    // コンストラクタ
    DebugDrawFormat(int startX = 0, int startY = 0, int textColor = GetColor(255, 255, 255), int lineHeight = 20, int startLine = 0)
        : x(startX), y(startY), color(textColor),lineHeight(lineHeight) {}

    // デバッグ用に文字列を出力するメソッド
    void String(const std::string& str) 
    {
        DrawString(x, y, StringUtility::StringToWstring(str).c_str(), 0xFFFFFF);
        y += lineHeight; // 次の行に移動
    }


    /// <summary>
    /// 表示する値が１つまでのデバッグ用DrawFormatStringを生成する
    /// </summary>
    /// <typeparam name="Value">値の型</typeparam>
    /// <param name="format">表示する文字列と値</param>
    /// <param name="val">表示する値</param>
    /// <param name="line">行数</param>
    /// <param name="lineH">行の高さ</param>
    template <typename Value>
    static void FormatString(const TCHAR* format, const Value val, int& line, const int lineH = 20)
    {
        DrawFormatString(0, line * lineH, 0xFFFFFF, format, val);
        line++;
    }

    /// <summary>
	/// 表示する値が１つまでのDrawFormatStringを画面右端に生成する
    /// </summary>
    /// <typeparam name="Value">値の型</typeparam>
    /// <param name="format">表示する文字列と値</param>
    /// <param name="val">表示する値</param>
    /// <param name="line">行数</param>
    /// <param name="lineH">行の高さ</param>
    template <typename Value>
    static void FormatStringRight(const TCHAR* format, const Value val, int& line, const int lineH = 20)
    {
        int strWidth = GetDrawFormatStringWidth(format, val);   //文字列の幅を取得
        int screenWidth = Application::SCREEN_SIZE_X;           //画面の幅
        int rightX = screenWidth - strWidth;                    //X座標を計算
        DrawFormatString(rightX, line * lineH, 0xFFFFFF, format, val);
        line++;
    }

    /// <summary>
	/// 文字列の幅を取得(値が1つまで)
    /// </summary>
    /// <typeparam name="Value">値の型</typeparam>
    /// <param name="format">取得する文字列</param>
    /// <param name="val">表示した値</param>
    /// <param name="line">行数</param>
    /// <returns>文字列の幅</returns>
    template <typename Value>
    static int GetFormatSize(const TCHAR* format, const Value val, const int line)
    {
        return GetDrawFormatStringWidth(format, val);
    }

    /// <summary>
	/// 値が2つまでのデバッグ用DrawFormatStringを生成する
    /// </summary>
    /// <typeparam name="ValA">値の型</typeparam>
    /// <typeparam name="ValB">値の型</typeparam>
    /// <param name="format">表示する文字列と値</param>
    /// <param name="valA">表示する値</param>
    /// <param name="valB">表示する値</param>
    /// <param name="line">行数</param>
    /// <param name="lineH">行の高さ</param>
    template <typename ValA, typename ValB>
    static void FormatString(const TCHAR* format, const ValA valA, const ValB valB , int& line, const int lineH = 20)
    {
        DrawFormatString(0, line * lineH, 0xFFFFFF, format, valA, valB);
        line++;
    }

    /// <summary>
	/// 値が2つまでのDrawFormatStringを画面右端に生成する
    /// </summary>
    /// <typeparam name="ValA">値の型</typeparam>
    /// <typeparam name="ValB">値の型</typeparam>
    /// <param name="format">表示する文字列と値</param>
    /// <param name="valA">表示する値</param>
    /// <param name="valB">表示する値</param>
    /// <param name="line">行数</param>
    /// <param name="lineH">行の高さ</param>
    template <typename ValA, typename ValB>
    static void FormatStringRight(const TCHAR* format, const ValA valA, const ValB valB, int& line, const int lineH = 20)
    {
        int strWidth = GetDrawFormatStringWidth(format, valA, valB); // 文字列の幅を取得
        int screenWidth = Application::SCREEN_SIZE_X; // 画面の幅 (適宜変更してください)
        int rightX = screenWidth - strWidth; // X座標を計算
        DrawFormatString(rightX, line * lineH, 0xFFFFFF, format, valA, valB);
        line++;
    }

    /// <summary>
    /// 文字列の幅を取得(値が2つまで)
    /// </summary>
    /// <typeparam name="ValA">値の型</typeparam>
    /// <typeparam name="ValB">値の型</typeparam>
    /// <param name="format">取得する文字列</param>
    /// <param name="valA">表示した値</param>
    /// <param name="valB">表示した値</param>
    /// <returns>文字列の幅</returns>
    template <typename ValA, typename ValB>
    static int GetFormatSize(const TCHAR* format, const ValA valA, const ValB valB)
    {
        return GetDrawFormatStringWidth(format, valA, valB);
    }

private:

    int x;          // X座標
    int y;          // Y座標
    int color;      // 色
    int lineHeight; // 行の高さ
};
