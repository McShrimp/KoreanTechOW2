#pragma once
#include "GetComponent.hpp"

namespace OW {
	inline void SetKey(uint32_t key) {
		__try {
			SDK->WPM<uint32_t>(SDK->g_player_controller + 0x1244, key);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}

	inline Vector3 SmoothLinear(Vector3 LocalAngle, Vector3 TargetAngle, float speed)
	{
		float d = LocalAngle.DistTo(TargetAngle);

		Vector3 Result;
		Result.X = LocalAngle.X + (TargetAngle.X - LocalAngle.X) * speed;
		Result.Y = LocalAngle.Y + (TargetAngle.Y - LocalAngle.Y) * speed;
		Result.Z = LocalAngle.Z + (TargetAngle.Z - LocalAngle.Z) * speed;
		return Result;
	}

	inline void SetKeyHold(int Key, float duration) {
		clock_t previous = clock();
		while (clock() - previous < duration) {
			SDK->WPM<uint32_t>(SDK->g_player_controller + 0x1244, Key);
		}
	}

	inline void AimCorrection(Vector3* InVecArg, Vector3 currVelocity, float Distance, float Bulletspeed)
	{
		if (Config::hanzo_flick)
		{
			float m_time = powf(Distance / Bulletspeed, 2.f);

			(*InVecArg).X = (*InVecArg).X + ((currVelocity.X) * (Distance / (Bulletspeed)));
			(*InVecArg).Y = (*InVecArg).Y + ((currVelocity.Y) * (Distance / (Bulletspeed)));
			(*InVecArg).Z = (*InVecArg).Z + ((currVelocity.Z) * (Distance / (Bulletspeed)));

			(*InVecArg).Y += (0.5f * 9.8f * m_time);
		}
		else
		{
			(*InVecArg).X = (*InVecArg).X + ((currVelocity.X) * (Distance / (Bulletspeed)));
			(*InVecArg).Y = (*InVecArg).Y + ((currVelocity.Y) * (Distance / (Bulletspeed)));
			(*InVecArg).Z = (*InVecArg).Z + ((currVelocity.Z) * (Distance / (Bulletspeed)));
		}
	}

	inline bool in_range(Vector3 MyAngle, Vector3 EnemyAngle, Vector3 MyPosition, Vector3 EnemyPosition, float radius)
	{
		float dist = MyPosition.DistTo(EnemyPosition);
		radius /= dist;
		Vector3 sub = MyAngle - EnemyAngle;

		return MyAngle.DistTo(EnemyAngle) <= radius;
	}

	inline Vector3 SmoothAccelerate(Vector3 LocalAngle, Vector3 TargetAngle, float Speed, float Acc)
	{
		Vector3 Result = LocalAngle;
		__try
		{
			Vector3 delta = TargetAngle - LocalAngle;

			float tmp = Acc / delta.get_length();
			tmp = tmp * tmp * 0.005f;

			float c = Speed;
			float chu = tmp * Speed;
			c += chu;

			if (c >= 1) c = 1;

			Result.X += delta.X * c;
			Result.Y += delta.Y * c;
			Result.Z += delta.Z * c;

			return Result;
		}
		__except (1)
		{
			return Result;
		}
	}

	inline XMFLOAT3 CalcAngle(XMFLOAT3 Target, XMFLOAT3 CameraPos) { 
		XMFLOAT3 Result;
		float Distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&Target) - XMLoadFloat3(&CameraPos)));
		XMStoreFloat3(&Result, (XMLoadFloat3(&Target) - XMLoadFloat3(&CameraPos)) / Distance);
		return Result;
	}


	float GetLookUpSkill(uint16_t a1) // for get Hanzo's Projectile Speed
	{
		__try
		{
			uint64_t pSkill = SDK->RPM<uint64_t>(local_entity.SkillBase + 0x1848);
			uint64_t SkillRawList = SDK->RPM<uint64_t>(pSkill + 0x10);
			uint32_t SkillSize = SDK->RPM<uint32_t>(pSkill + 0x18);
			for (uint32_t i = 0x0; i < SkillSize; i++)
			{
				if (SDK->RPM<uint16_t>(SkillRawList + (i * 0x80)) == a1)
					if (SDK->RPM<float>(SkillRawList + (i * 0x80) + 0x30) >= 0.5f) // 50% ÀÌ»ó Â÷Â¡ ½Ã 
						return SDK->RPM<float>(SkillRawList + (i * 0x80) + 0x30);
			}
		}
		__except (1) {}
		return 0.f;

	}

	Vector3 GetVector3(bool predit = false)
	{
		int TarGetIndex = -1;
		Vector3 target{};
		Vector2 CrossHair = Vector2(GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f);

		float origin = 100000.f;

		if (TarGetIndex == -1)
		{
			if (entities.size() > 0) {
				{
					for (int i = 0; i < entities.size(); i++)
					{
						if (entities[i].Alive && entities[i].Team && entities[i].Vis)
						{
							Vector3 PreditPos = entities[i].head_pos;
							Vector3 RootPos = entities[i].head_pos;
							Vector3 Vel = entities[i].velocity;
							if (predit) 
							{
								float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
								if (local_entity.HeroID == eHero::HERO_HANJO && GetLookUpSkill(0xC9) >= 0.5f)
								{
									AimCorrection(&PreditPos, Vel, dist, 25.f + (85.f * GetLookUpSkill(0xC9)));
								}
								else
									AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
							}
							Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
							float CrossDist = CrossHair.Distance(Vec2);

							if (CrossDist < origin && CrossDist <= Config::Fov)
							{
								target = predit ? PreditPos : RootPos;
								origin = CrossDist;
								TarGetIndex = i;
							}
							else
								TarGetIndex = -1;
						}
						else
							TarGetIndex = -1;
					}
				}
			}
			else
			{
				if (entities[TarGetIndex].Alive && entities[TarGetIndex].Team && entities[TarGetIndex].Vis)
				{
					Vector3 PreditPos = entities[TarGetIndex].head_pos;
					Vector3 RootPos = entities[TarGetIndex].head_pos;
					Vector3 Vel = entities[TarGetIndex].velocity;
					if (predit) 
					{
						float dist = Vector3(viewMatrix_xor.get_location().x, viewMatrix_xor.get_location().y, viewMatrix_xor.get_location().z).DistTo(PreditPos);
						if (local_entity.HeroID == eHero::HERO_HANJO && GetLookUpSkill(0xC9) >= 0.5f)
						{
							AimCorrection(&PreditPos, Vel, dist, 25.f + (85.f * GetLookUpSkill(0xC9)));
						}
						else
							AimCorrection(&PreditPos, Vel, dist, Config::predit_level);
					}
					Vector2 Vec2 = predit ? viewMatrix.WorldToScreen(PreditPos) : viewMatrix.WorldToScreen(RootPos);
					float CrossDist = CrossHair.Distance(Vec2);

					if (CrossDist < origin && CrossDist <= Config::Fov)
					{
						target = predit ? PreditPos : RootPos;
						origin = CrossDist;
					}
					else
					{
						TarGetIndex = -1;

					}
				}
				else
				{
					TarGetIndex = -1;
				}
			}
			return target;
		}
	}
}