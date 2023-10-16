#pragma once
#include "Vector.hpp"

namespace OW {
	namespace Render {
		inline void DrawFilledRect(float x, float y, float w, float h, const ImU32& color)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();

			auto a = (color >> 24) & 0xFF;
			auto r = (color >> 16) & 0xFF;
			auto g = (color >> 8) & 0xFF;
			auto b = (color) & 0xFF;

			Draw->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), NULL, NULL);
		}

		inline void DrawStrokeText(const ImVec2& pos, ImU32 col, const char* str, float size = 15.f)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();

			Draw->AddText(NULL, size, ImVec2(pos.x, pos.y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), str);
			Draw->AddText(NULL, size, ImVec2(pos.x, pos.y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), str);
			Draw->AddText(NULL, size, ImVec2(pos.x - 1, pos.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), str);
			Draw->AddText(NULL, size, ImVec2(pos.x + 1, pos.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), str);
			Draw->AddText(NULL, size, ImVec2(pos.x, pos.y), col, str);
		}

		inline void DrawInfo(const ImVec2& Pos, ImU32 TagCol, int HeightSize, std::string Info, float distance, float EnHealth , float EnHealthMax) {
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();
			ImVec2 InfoSize = ImGui::CalcTextSize(Info.c_str());
			if (distance < 200)  // 병신같은게 1 / distance 안해놓고 걍 연산때려서 그런거아님 
			{
				// 
				if (EnHealth > 0.f) 
				{
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2), Pos.y + (HeightSize / 2)), ImVec2(Pos.x + (InfoSize.x / 2) + 35, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(0, 0, 0, 0.7)));
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2), Pos.y + (HeightSize / 2)), ImVec2(Pos.x - (InfoSize.x / 2) + 5, Pos.y - (HeightSize / 2)), TagCol);
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2) + 7, Pos.y + (HeightSize / 2) - 6), ImVec2(Pos.x - (InfoSize.x / 2) + 7 + ((abs((Pos.x - (InfoSize.x / 2) + 7) - (Pos.x + (InfoSize.x / 2))) / EnHealthMax) * EnHealth), Pos.y + (HeightSize / 2) - 2), ImGui::GetColorU32(ImVec4(0, 1, 0, 1)));
					DrawStrokeText(ImVec2(Pos.x - ((InfoSize.x / 2)) + 10, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), Info.c_str());
					// 여기부분
				}
				else 
				{
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2), Pos.y + (HeightSize / 2)), ImVec2(Pos.x + (InfoSize.x / 2) + 35, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(0, 0, 0, 0.7)));
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2), Pos.y + (HeightSize / 2)), ImVec2(Pos.x - (InfoSize.x / 2) + 5, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(0.4, 0.4, 0.4, 1.0f)));
					Draw->AddRectFilled(ImVec2(Pos.x - (InfoSize.x / 2) + 7, Pos.y + (HeightSize / 2) - 6), ImVec2(Pos.x - (InfoSize.x / 2) + 7 + ((abs((Pos.x - (InfoSize.x / 2) + 7) - (Pos.x + (InfoSize.x / 2))) / EnHealthMax) * EnHealth), Pos.y + (HeightSize / 2) - 2), ImGui::GetColorU32(ImVec4(0, 1, 0, 1)));
					DrawStrokeText(ImVec2(Pos.x - ((InfoSize.x / 2)) + 10, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), Info.c_str());
				}
			}
			else  // distance >= 200m
			{
				DrawStrokeText(ImVec2(Pos.x - ((InfoSize.x / 2)) + 10, Pos.y - (HeightSize / 2)), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), Info.c_str());
			}
		}  

		inline void RenderLine(const Vector2& from, const Vector2& to, uint32_t color, float thickness = 1.0f)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();

			auto a = (color >> 24) & 0xff;
			auto r = (color >> 16) & 0xff;
			auto g = (color >> 8) & 0xff;
			auto b = (color) & 0xff;

			Draw->AddLine(ImVec2(from.X, from.Y), ImVec2(to.X, to.Y), ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
		}

		inline void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness = 0.5f)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();
			float lineW = (W / 3);
			float lineH = (H / 3);

			////black outlines
			Draw->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
			Draw->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

			//corners
			Draw->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);
			Draw->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
			Draw->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
			Draw->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);
			Draw->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
			Draw->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);
			Draw->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
			Draw->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
		}
		inline void DrawEdges(const ImVec2& top, const ImVec2& bot, const ImVec2& base, ImU32 col)
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();
			float scale = (bot.y - top.y) / 3.0f;
			float length = scale / 1;
			Draw->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale + length, top.y), col, 2.0f); //  --- Top left
			Draw->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale, top.y + length), col, 2.0f); // | Top left
			Draw->AddLine(ImVec2(base.x + scale / 3, top.y), ImVec2(base.x + scale / 3 + length, top.y), col, 2.0f); // --- Top right
			Draw->AddLine(ImVec2(base.x + scale / 3 + length, top.y), ImVec2(base.x + scale / 3 + length, top.y + length), col, 2.0f); // | Top right
			Draw->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale + length, bot.y), col, 2.0f); // --- Bottom left
			Draw->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale, bot.y - length), col, 2.0f); // | Bottom left
			Draw->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3, bot.y), col, 2.0f); // --- Bottom right
			Draw->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3 + length, bot.y - length), col, 2.0f); // | Bottom right
		}
	}
}