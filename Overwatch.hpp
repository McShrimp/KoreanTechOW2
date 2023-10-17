#pragma once
#include "Target.hpp"


float WX, WY;
namespace OW {
	
	inline void entity_scan_thread() {
		while (true) {
			last_ow_entities = ow_entities;
			ow_entities = get_ow_entities();
			if (last_ow_entities.size() != ow_entities.size())
				cout << "Entities: " << dec << ow_entities.size() << endl;
			Sleep(300);
		}
	}

	inline void entity_thread() {
		while (true) {

			if (ow_entities.size() > 0) {
				std::vector<c_entity> tmp_entities{};
				for (int i = 0; i < ow_entities.size(); i++) {
					c_entity entity{};
					auto [ComponentParent, LinkParent] = ow_entities[i];
					if (!LinkParent || !ComponentParent)
						continue;
					entity.address = ComponentParent;
					entity.HealthBase = DecryptComponent(ComponentParent, TYPE_HEALTH);
					entity.LinkBase = DecryptComponent(LinkParent, TYPE_LINK);
					entity.TeamBase = DecryptComponent(ComponentParent, TYPE_TEAM);
					entity.VelocityBase = DecryptComponent(ComponentParent, TYPE_VELOCITY);
					entity.HeroBase = DecryptComponent(LinkParent, TYPE_P_HEROID);
					entity.OutlineBase = DecryptComponent(ComponentParent, TYPE_OUTLINE);
					entity.BoneBase = DecryptComponent(ComponentParent, TYPE_BONE);
					entity.RotationBase = DecryptComponent(ComponentParent, TYPE_ROTATION);
					entity.SkillBase = DecryptComponent(ComponentParent, TYPE_SKILL);
					entity.VisBase = DecryptComponent(LinkParent, TYPE_P_VISIBILITY);
					entity.AngleBase = DecryptComponent(LinkParent, TYPE_PLAYERCONTROLLER);
					entity.EnemyAngleBase = DecryptComponent(ComponentParent, TYPE_ANGLE);
					health_compo_t health_compo{};
					velocity_compo_t velo_compo{};
					hero_compo_t hero_compo{};
					vis_compo_t vis_compo{};
					if (entity.HealthBase) {
						health_compo = SDK->RPM<health_compo_t>(entity.HealthBase);
						entity.PlayerHealth = health_compo.health + health_compo.armor + health_compo.barrier;
						entity.PlayerHealthMax = health_compo.health_max + health_compo.armor_max + health_compo.barrier_max;
						entity.MinHealth = health_compo.health;
						entity.MaxHealth = health_compo.health_max;
						entity.MinArmorHealth = health_compo.armor;
						entity.MaxArmorHealth = health_compo.armor_max;
						entity.MinBarrierHealth = health_compo.barrier;
						entity.MaxBarrierHealth = health_compo.barrier_max;
						entity.Alive = (entity.PlayerHealth > 0.f) ? true : false;
					}
					if (entity.RotationBase) {
						entity.Rot = SDK->RPM<Vector3>(SDK->RPM<uint64_t>(entity.RotationBase + 0x7C0) + 0x8FC);
					}
					if (entity.VelocityBase) {
						velo_compo = SDK->RPM<velocity_compo_t>(entity.VelocityBase);
						entity.pos = Vector3(velo_compo.location.x, velo_compo.location.y - 1.f, velo_compo.location.z); // ㅋㅋ 
						entity.velocity = Vector3(velo_compo.velocity.x, velo_compo.velocity.y, velo_compo.velocity.z);
						static int head_index = entity.GetSkel()[0];
						entity.head_pos = entity.GetBonePos(head_index);
					}
					if (entity.HeroBase) {
						hero_compo = SDK->RPM<hero_compo_t>(entity.HeroBase);
						entity.HeroID = hero_compo.heroid;
					}
					if (entity.VisBase) {
						vis_compo = SDK->RPM<vis_compo_t>(entity.VisBase);
						entity.Vis = (DecryptVis(vis_compo.key1) ^ vis_compo.key2) ? true : false;
					}
					if (entity.TeamBase) {
						entity.Team = (entity.GetTeam() == eTeam::TEAM_DEATHMATCH || entity.GetTeam() != local_entity.GetTeam()) ? true : false;
					}

					const auto angle_component = DecryptComponent(LinkParent, TYPE_PLAYERCONTROLLER);
					if (angle_component)
					{
						local_entity = entity;
						SDK->g_player_controller = angle_component;
					}
					if (ComponentParent)
						tmp_entities.push_back(entity);
				}
				entities = tmp_entities;
			}
			Sleep(5);
		}
	}

	inline void viewmatrix_thread() {
		__try {
			while (true) {
				auto viewMatrixVal = SDK->RPM<uint64_t>(SDK->dwGameBase + offset::Address_viewmatrix_base) ^ offset::offset_viewmatrix_xor_key;
				Vector2 WindowSize = SDK->RPM<Vector2>(viewMatrixVal + 0x424);
				WX = WindowSize.X;
				WY = WindowSize.Y;
				viewMatrix = SDK->RPM<Matrix>(viewMatrixPtr);
				viewMatrix_xor = SDK->RPM<Matrix>(viewMatrix_xor_ptr);
				Sleep(5);
			}
		}
		__except (1) {
		}
	}

	inline void PlayerInfo() {
		if (entities.size() > 0) {
			for (c_entity entity : entities) {
				if (entity.Alive && entity.Team && local_entity.PlayerHealth > 0) {
					Vector3 Vec3 = entity.head_pos; // 일단 원래대로 돌렸음
					float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(Vec3);
					Vector2 Vec2_A{}, Vec2_B{};
					if (!viewMatrix.WorldToScreen(Vector3(Vec3.X, Vec3.Y + 1.f, Vec3.Z), &Vec2_A, Vector2(WX, WY)))
						continue;

					if (!viewMatrix.WorldToScreen(Vector3(Vec3.X, Vec3.Y - 0.8f, Vec3.Z), &Vec2_B, Vector2(WX, WY)))
						continue;

					float height = abs(Vec2_A.Y - Vec2_B.Y);
					float width = height * 0.85;
					float Size = abs(Vec2_A.Y - Vec2_B.Y) / 2.0f;
					float Size2 = abs(Vec2_A.Y - Vec2_B.Y) / 20.0f;
					float Height2 = abs(Vec2_A.Y - Vec2_B.Y);
					int num7 = (float)(1500 / (int)Height2);
					float xpos = (Vec2_A.X + Vec2_B.X) / 2.f;
					float ypos = Vec2_A.Y + Size / 5;
					// 여기 감
					Render::DrawInfo(ImVec2(Vec2_A.X, Vec2_A.Y), ImGui::GetColorU32(ImVec4(1, 0, 0, 1)), 30, (skCrypt("[").decrypt() + std::to_string((int)dist) + skCrypt("m] ").decrypt() + skCrypt("[Hero : ").decrypt() + GetHeroNames(entity.HeroID, entity.LinkBase).c_str() + skCrypt("]").decrypt()), dist, entity.PlayerHealth, entity.PlayerHealthMax);
				}
			}
		}
	}

	inline void Draw_Skel() {
		if (entities.size() > 0) {
			for (c_entity entity : entities) {
				if (entity.Alive && entity.Team && local_entity.PlayerHealth > 0) {
					espBone a = entity.getBoneList(entity.GetSkel());
					DWORD Col = entity.Vis ? Color{ 0, 0, 0, 0 }.RGBA2ARGB(255) : Color{ 0, 0, 0, 0 }.RGBA2ARGB(255);
					DWORD Col2 = entity.Vis ? Color{ 0, 255, 0, 200 }.RGBA2ARGB(255) : Color{ 255, 0, 0, 200 }.RGBA2ARGB(255);
					Vector2 outPos{};
					if (!a.boneerror) {
						if (viewMatrix.WorldToScreen(entity.pos, &outPos, Vector2(WX, WY))) {
							if (entity.HeroID == eHero::HERO_TRAININGBOT1 || entity.HeroID == eHero::HERO_TRAININGBOT2 || entity.HeroID == eHero::HERO_TRAININGBOT3 || entity.HeroID == eHero::HERO_TRAININGBOT4) {
								Render::RenderLine(a.head, a.neck, Col, 4.f);
								Render::RenderLine(a.neck, a.body_1, Col, 4.f);
								Render::RenderLine(a.body_1, a.l_1, Col, 4.f);
								Render::RenderLine(a.body_1, a.r_1, Col, 4.f);

								Render::RenderLine(a.head, a.neck, Col2, 2.f);
								Render::RenderLine(a.neck, a.body_1, Col2, 2.f);
								Render::RenderLine(a.body_1, a.l_1, Col2, 2.f);
								Render::RenderLine(a.body_1, a.r_1, Col2, 2.f);
							}
							else {
								Render::RenderLine(a.head, a.neck, Col, 4.f);
								Render::RenderLine(a.neck, a.body_1, Col, 4.f);
								Render::RenderLine(a.body_1, a.body_2, Col, 4.f);
								Render::RenderLine(a.neck, a.l_1, Col, 4.f);
								Render::RenderLine(a.neck, a.r_1, Col, 4.f);
								Render::RenderLine(a.l_1, a.l_d_1, Col, 4.f);
								Render::RenderLine(a.r_1, a.r_d_1, Col, 4.f);
								Render::RenderLine(a.l_d_1, a.l_d_2, Col, 4.f);
								Render::RenderLine(a.r_d_1, a.r_d_2, Col, 4.f);
								Render::RenderLine(a.body_2, a.l_a_1, Col, 4.f);
								Render::RenderLine(a.body_2, a.r_a_1, Col, 4.f);
								Render::RenderLine(a.l_a_1, a.l_a_2, Col, 4.f);
								Render::RenderLine(a.r_a_1, a.r_a_2, Col, 4.f);

								Render::RenderLine(a.r_a_2, a.sex, Col, 4.f);
								Render::RenderLine(a.l_a_2, a.sex1, Col, 4.f);
								Render::RenderLine(a.sex, a.sex2, Col, 4.f);
								Render::RenderLine(a.sex1, a.sex3, Col, 4.f);

								Render::RenderLine(a.head, a.neck, Col2, 2.f);
								Render::RenderLine(a.neck, a.body_1, Col2, 2.f);
								Render::RenderLine(a.body_1, a.body_2, Col2, 2.f);
								Render::RenderLine(a.neck, a.l_1, Col2, 2.f);
								Render::RenderLine(a.neck, a.r_1, Col2, 2.f);
								Render::RenderLine(a.l_1, a.l_d_1, Col2, 2.f);
								Render::RenderLine(a.r_1, a.r_d_1, Col2, 2.f);
								Render::RenderLine(a.l_d_1, a.l_d_2, Col2, 2.f);
								Render::RenderLine(a.r_d_1, a.r_d_2, Col2, 2.f);
								Render::RenderLine(a.body_2, a.l_a_1, Col2, 2.f);
								Render::RenderLine(a.body_2, a.r_a_1, Col2, 2.f);
								Render::RenderLine(a.l_a_1, a.l_a_2, Col2, 2.f);
								Render::RenderLine(a.r_a_1, a.r_a_2, Col2, 2.f);

								Render::RenderLine(a.r_a_2, a.sex, Col2, 2.f);
								Render::RenderLine(a.l_a_2, a.sex1, Col2, 2.f);
								Render::RenderLine(a.sex, a.sex2, Col2, 2.f);
								Render::RenderLine(a.sex1, a.sex3, Col2, 2.f);
							}
						}
					}
				}
			}
		}
	}

	inline void esp() {
		__try
		{
			ImDrawList* Draw = ImGui::GetBackgroundDrawList();
			ImVec2 CrossHair = ImVec2(WX / 2.0f, WY / 2.0f);
			if (Config::draw_info)
				PlayerInfo();

			if (Config::draw_skel)
				Draw_Skel();

			if (Config::draw_edge)
			{
				if (entities.size() > 0)
				{
					for (c_entity entity : entities)
					{
						if (entity.Alive && entity.Team && local_entity.PlayerHealth > 0)
						{
							Vector3 Vec3 = entity.head_pos;
							Vector2 Vec2_A{}, Vec2_B{};
							if (!viewMatrix.WorldToScreen(Vector3(Vec3.X, Vec3.Y + 0.1f, Vec3.Z), &Vec2_A, Vector2(WX,WY)))
								continue;

							if (!viewMatrix.WorldToScreen(Vector3(Vec3.X, Vec3.Y - 1.8f, Vec3.Z), &Vec2_B, Vector2(WX, WY)))
								continue;

							float height = abs(Vec2_A.Y - Vec2_B.Y);
							float width = height * 0.85;
							float Size = abs(Vec2_A.Y - Vec2_B.Y) / 2.0f;
							float Size2 = abs(Vec2_A.Y - Vec2_B.Y) / 20.0f;
							float Height2 = abs(Vec2_A.Y - Vec2_B.Y);
							int num7 = (float)(1500 / (int)Height2);
							float xpos = (Vec2_A.X + Vec2_B.X) / 2.f;
							float ypos = Vec2_A.Y + Size / 5;

							Render::DrawCorneredBox(Vec2_A.X - (width / 2) - 1, Vec2_A.Y - (height / 5) - 1, width + 2, height + 12, ImGui::GetColorU32(Config::EnemyCol));
							Render::DrawFilledRect(Vec2_A.X - (width / 2) - 1, Vec2_A.Y - (height / 5) - 1, width + 2, height + 12, ImColor(0, 0, 0, 60));
						}
					}
				}
			}
			if (Config::draw_fov)
			{
				Draw->AddCircle(CrossHair, Config::Fov, 0xFFFFFFFF, 100);
			}
		}
		__except (1) {

		}
	}

	inline void overlay_thread() {
		__try {
			int tab_index = 0;
			std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
			std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

			HWND tWnd = FindWindowA(skCrypt("TankWindowClass"), NULL);

			WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, skCrypt("Chrome"), NULL };
			RegisterClassEx(&wc);
			HWND hwnd = CreateWindow(wc.lpszClassName, skCrypt("Chrome"), WS_POPUP, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);

			if (!CreateDeviceD3D(hwnd))
			{
				CleanupDeviceD3D();
				UnregisterClass(wc.lpszClassName, wc.hInstance);
			}

			MARGINS margins = { -1 };
			DwmExtendFrameIntoClientArea(hwnd, &margins);

			SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);

			ShowWindow(hwnd, SW_SHOWDEFAULT);
			UpdateWindow(hwnd);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO(); (void)io;

			ImGui::StyleColorsDark();

			ImGui_ImplWin32_Init(tWnd);
			ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			int FPS = 120;

			ShowWindow(GetConsoleWindow(), SW_HIDE);

			while (FindWindowA(skCrypt("TankWindowClass"), NULL))
			{
				//DEVMODE dm;
				//dm.dmSize = sizeof(DEVMODE);

				//numDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

				//FPS = dm.dmDisplayFrequency;

				a = std::chrono::system_clock::now();
				std::chrono::duration<double, std::milli> work_time = a - b;
				if (work_time.count() < 1000 / FPS)
				{
					std::chrono::duration<double, std::milli> delta_ms(1000 / FPS - work_time.count());
					auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
					std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
				}

				b = std::chrono::system_clock::now();
				std::chrono::duration<double, std::milli> sleep_time = b - a;

				DWORD Style = GetWindowLong(tWnd, GWL_STYLE);
				RECT rect;
				GetWindowRect(tWnd, &rect);

				SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_NOZORDER);

				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

				if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
					continue;
				}

				if (GetAsyncKeyState(VK_LBUTTON))
				{
					ImGui::GetIO().MouseDown[0] = true;
				}
				else
				{
					ImGui::GetIO().MouseDown[0] = false;
				}

				if (GetAsyncKeyState(VK_INSERT)) {
					Config::Menu = !Config::Menu;
					Sleep(250);
				}

				// Start the Dear ImGui frame
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				if (Config::Menu)
				{
					ImGui::SetNextWindowPos(ImVec2{ 50, 30 }, ImGuiCond_Once);
					ImGui::SetNextWindowSize(ImVec2{ 450, 370 }, ImGuiCond_Once);
					bool _visible = true;
					if (ImGui::Begin(skCrypt("SEX"), &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

						ImGui::SameLine();

						if (ImGui::Button(skCrypt("Aimbot"), ImVec2(150, 50)))
							tab_index = 1;

						ImGui::SameLine();

						if (ImGui::Button(skCrypt("Visuals"), ImVec2(150, 50)))
							tab_index = 2;

						ImGui::Separator();

						if (tab_index == 1) {
							ImGui::Checkbox(skCrypt("Tracking"), &Config::Tracking);
							if (Config::Tracking) {
								ImGui::Checkbox(skCrypt("Prediction"), &Config::Prediction);
							}
							ImGui::Checkbox(skCrypt("Flick"), &Config::Flick);
							ImGui::Checkbox(skCrypt("Hanzo bot"), &Config::hanzo_flick);
							if (Config::hanzo_flick || Config::Prediction) {
								ImGui::Separator();
								ImGui::BulletText(skCrypt("Prediction"));
								ImGui::SliderFloat(skCrypt("Prediction Value"), &Config::predit_level, 0.f, 200.f, skCrypt("%.2f"));
							}

							ImGui::BulletText(skCrypt("Key Bind"));
							if (ImGui::BeginCombo(skCrypt("##Key"), keys))
							{
								for (int i = 0; i < 5; i++)
								{
									const bool type = keys == key_type[i];
									if (ImGui::Selectable(key_type[i], type))
									{
										keys = key_type[i];
										switch (i)
										{
										case 0:
											Config::aim_key = VK_LBUTTON;
											break;
										case 1:
											Config::aim_key = VK_RBUTTON;
											break;
										case 2:
											Config::aim_key = VK_MBUTTON;
											break;
										case 3:
											Config::aim_key = VK_XBUTTON1;
											break;
										case 4:
											Config::aim_key = VK_XBUTTON2;
											break;
										}
									}
								}
								ImGui::EndCombo();
							}

							ImGui::BulletText(skCrypt("Fov"));
							ImGui::SliderFloat(skCrypt("Fov Value"), &Config::Fov, 0.f, 500.f, skCrypt("%.3f"));
							ImGui::SliderFloat(skCrypt("HitBox Size Value"), &Config::hitbox, 0.12f, 0.28f, skCrypt("%.3f"));

							ImGui::BulletText(skCrypt("Smooth"));
							ImGui::SliderFloat(skCrypt("Tracking Smooth Value"), &Config::Tracking_smooth, 0.f, 1.f, skCrypt("%.2f"));
							ImGui::SliderFloat(skCrypt("Flick Smooth Value"), &Config::Flick_smooth, 0.f, 1.f, skCrypt("%.2f"));
							ImGui::Separator();
						}

						if (tab_index == 2) {
							ImGui::Checkbox(skCrypt("Player Info"), &Config::draw_info);
							ImGui::Checkbox(skCrypt("Skeleton"), &Config::draw_skel);
							ImGui::Checkbox(skCrypt("Corner Box"), &Config::draw_edge);
							ImGui::ColorEdit3(skCrypt("Corner Box Color"), (float*)&Config::EnemyCol);
							ImGui::Checkbox(skCrypt("Fov"), &Config::draw_fov);
							ImGui::Separator();
						}
					}
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
					ImGui::End();
				}

				esp();
				ImGui::EndFrame();
				ImGui::Render();
				g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
				auto ss = ImVec4(0, 0, 0, 0);
				g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&ss);
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				g_pSwapChain->Present(1, 0);
			}

			// Cleanup
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();

			CleanupDeviceD3D();
			::DestroyWindow(hwnd);
			::UnregisterClass(wc.lpszClassName, wc.hInstance);
		}
		__except (1) {

		}
	}

	inline void aimbot_thread()
	{
		__try {
			Vector2 CrossHair = Vector2(GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f);
			while (true) {
				static float origin_sens = 0.f;
				if (SDK->RPM<float>(GetSenstivePTR()))
					origin_sens = SDK->RPM<float>(GetSenstivePTR());
				else if (origin_sens)
					SDK->WPM<float>(GetSenstivePTR(), origin_sens);

				bool shooted = false;

				//Tracking
				if (Config::Tracking) {
					while (GetAsyncKeyState(Config::aim_key))
					{
						auto vec = GetVector3(Config::Prediction);
						if (vec != Vector3(0, 0, 0)) {
							auto local_angle = SDK->RPM<Vector3>(SDK->g_player_controller + 0x12A0);
							auto calc_target = CalcAngle(XMFLOAT3(vec.X, vec.Y, vec.Z), viewMatrix_xor.get_location());
							auto vec_calc_target = Vector3(calc_target.x, calc_target.y, calc_target.z);
							auto Target = SmoothLinear(local_angle, vec_calc_target, Config::Tracking_smooth / 10.f);
							auto local_loc = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z);

							if (Target != Vector3(0, 0, 0)) {
								SDK->WPM<Vector3>(SDK->g_player_controller + 0x12A0, Target);
							}
						}
						Sleep(1);
					}
				}
				//Flick
				else if (Config::Flick) {
					while (GetAsyncKeyState(Config::aim_key))
					{
						if (!shooted) {
							auto vec = GetVector3(false);
							if (vec != Vector3(0, 0, 0)) {
								auto local_angle = SDK->RPM<Vector3>(SDK->g_player_controller + 0x12A0);
								auto calc_target = CalcAngle(XMFLOAT3(vec.X, vec.Y, vec.Z), viewMatrix_xor.get_location());
								auto vec_calc_target = Vector3(calc_target.x, calc_target.y, calc_target.z);
								auto Target = SmoothAccelerate(local_angle, vec_calc_target, Config::Flick_smooth / 10.f, 75.0f / 100.0f);
								auto local_loc = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z);

								if (Target != Vector3(0, 0, 0)) {
									SDK->WPM<Vector3>(SDK->g_player_controller + 0x12A0, Target);
									if (in_range(local_angle, vec_calc_target, local_loc, vec, Config::hitbox)) {
										SDK->WPM<float>(GetSenstivePTR(), 0);
										SetKey(0x1);
										SDK->WPM<float>(GetSenstivePTR(), origin_sens);
										shooted = true;
									}
								}
							}
						}
						Sleep(1);
					}
				}
				else if (Config::hanzo_flick)
				{
					while (GetAsyncKeyState(Config::aim_key))
					{
						if (!shooted) {
							auto vec = GetVector3(true); // 아 여깄네 
							if (vec != Vector3(0, 0, 0))
							{

								auto local_angle = SDK->RPM<Vector3>(SDK->g_player_controller + 0x12A0);
								auto calc_target = CalcAngle(XMFLOAT3(vec.X, vec.Y, vec.Z), viewMatrix_xor.get_location());
								auto vec_calc_target = Vector3(calc_target.x, calc_target.y, calc_target.z);
								auto Target = SmoothAccelerate(local_angle, vec_calc_target, Config::Flick_smooth / 10.f, 75.0f / 100.0f); // 탄낙 어디에있음 
								auto local_loc = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z);

								if (Target != Vector3(0, 0, 0))
								{
									SDK->WPM<Vector3>(SDK->g_player_controller + 0x12A0, Target);
									if (in_range(local_angle, vec_calc_target, local_loc, vec, Config::hitbox))
									{
										SDK->WPM<float>(GetSenstivePTR(), 0);
										SetKeyHold(0x1000, 100);
										SDK->WPM<float>(GetSenstivePTR(), origin_sens);
										shooted = true;
									}
								}
							}
						}
						Sleep(1);
					}
				}
				Sleep(1);
			}
		}
		__except (1) {

		}
	}
}
