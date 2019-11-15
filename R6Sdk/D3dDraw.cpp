#include "Cheat.h"
#include "D3dDraw.h"
#include "math\Vector2D.hpp"

wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}
string WStringToMBytes(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen;
	// wide char to multi char
	iTextLen = ::WideCharToMultiByte(CP_ACP, 0, lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
wstring UTF8ToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset((void*)pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	wstring wstrReturn(pUnicode);
	delete[] pUnicode;
	return wstrReturn;
}
string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
string readString(const DWORD64 dwPtr)
{
	string ret = WStringToMBytes(UTF8ToWString((const char*)dwPtr).c_str());
	return ret;
}
void Draw::DrawString(float fontSize, const Vector2D &vec, ImU32 col, bool bCenter, bool stroke, const char *pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	Vector2D drawPos = vec;
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		drawPos.x = vec.x - textSize.x / 2;
		drawPos.y = vec.y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(drawPos.x + 1, drawPos.y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(drawPos.x - 1, drawPos.y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(drawPos.x + 1, drawPos.y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(drawPos.x - 1, drawPos.y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(drawPos.x, drawPos.y), col, text.c_str());
}
void Draw::DrawBox(const Vector2D &vecStart, const Vector2D &vecEnd, ImU32 col, float thickness)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(vecStart.x, vecStart.y), ImVec2(vecStart.x + vecEnd.x, vecStart.y + vecEnd.y), col, 0, 0, thickness);
}

void Draw::DrawFilledBox(const Vector2D &vecStart, const Vector2D &vecEnd, ImU32 col)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(vecStart.x, vecStart.y), ImVec2(vecStart.x + vecEnd.x, vecStart.y + vecEnd.y), col);
}

void Draw::DrawLine(const Vector2D &vecStart, const Vector2D &vecEnd, ImU32 col, float thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(vecStart.x, vecStart.y), ImVec2(vecEnd.x, vecEnd.y), col, thickness);
}

void Draw::DrawCircle(const Vector2D &vecCenter, float radius, ImU32 col, int num_seg, float thickness)
{

	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(vecCenter.x, vecCenter.y), radius, col, num_seg, thickness);

}