#pragma once

namespace OW {
	namespace Config {
		/* Aimbot */
		float Flick_smooth = 0.1f;
		float Tracking_smooth = 0.1f;
		float Fov = 200.f;
		float y_pitch = 0.45f;
		float hitbox = 0.130f;
		float predit_level = 115.f; // ¼ÒÀü 160.f
		
		bool Flick = false;
		bool Tracking = false;
		bool Prediction = false;
		bool hanzo_flick = false;

		/* Draw */
		bool draw_fov = false;
		bool draw_box = false;
		bool draw_edge = false;
		bool draw_info = false;
		bool draw_skel = true;

		/* Draw Menu */
		bool Menu = true;

		/* Team */
		bool is_team = false;

		int aim_key = VK_XBUTTON2;

		/* Color */
		ImVec4 EnemyCol = ImVec4(1, 1, 1, 1);
	}
}