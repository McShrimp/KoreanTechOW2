#pragma once
#include "IDAdefs.h"
namespace OW {
	class HPComponent
	{
	public:
		char pad_0001[0xDC];
		Vector2 Health;
		char pad_0002[0x21C];
		Vector2 Armor;
		char pad_0003[0x35C];
		Vector2 Barrier;

		Vector2 GetHealth()
		{
			Vector2 HP = SDK->RPM<Vector2>((uint64_t)&this->Health);
			Vector2 Ar = SDK->RPM<Vector2>((uint64_t)&this->Armor);
			Vector2 Br = SDK->RPM<Vector2>((uint64_t)&this->Barrier);
			//	HP.X = Max , HP.Y = Current HP
			return { HP.X + Ar.X + Br.X ,HP.Y + Ar.Y + Br.Y };
		}
	};
	struct health_compo_t {
		union {
			OFF_MEMB(float, health, 0xE0);
			OFF_MEMB(float, health_max, 0xDC);
			OFF_MEMB(float, armor, 0x220);
			OFF_MEMB(float, armor_max, 0x21C);
			OFF_MEMB(float, barrier, 0x360);
			OFF_MEMB(float, barrier_max, 0x35C);
		};
	};
	struct obj_compo_t {
		union {
			OFF_MEMB(XMFLOAT3, obj_pos, 0xE0);
		};
	};
	struct velocity_compo_t {
		union {
			OFF_MEMB(XMFLOAT3, velocity, 0x50);
			OFF_MEMB(XMFLOAT3, location, 0x1B0);
			OFF_MEMB(uint64_t, bonedata, 0x810);
		};
	};
	struct hero_compo_t {
		union {
			OFF_MEMB(uint64_t, heroid, 0xD0);
		};
	};
	struct vis_compo_t {
		union {
			OFF_MEMB(uint64_t, key1, 0xA0);
			OFF_MEMB(uint64_t, key2, 0x98);
		};
	};

	inline std::string GetHeroNames(uint64_t HeroID, uint64_t LinkBase) {
		switch (HeroID)
		{
		case eHero::HERO_REAPER:
			return skCrypt("Reaper").decrypt();
		case eHero::HERO_TRACER:
			return skCrypt("Tracer").decrypt();
		case eHero::HERO_MERCY:
			return skCrypt("Mercy").decrypt();
		case eHero::HERO_HANJO:
			return skCrypt("Hanzo").decrypt();
		case eHero::HERO_TORBJORN:
			return skCrypt("Torbjorn").decrypt();
		case eHero::HERO_REINHARDT:
			return skCrypt("Reinhardt").decrypt();
		case eHero::HERO_PHARAH:
			return skCrypt("Pharah").decrypt();
		case eHero::HERO_WINSTON:
			return skCrypt("Winston").decrypt();
		case eHero::HERO_WIDOWMAKER:
			return skCrypt("Widowmaker").decrypt();
		case eHero::HERO_BASTION:
			return skCrypt("Bastion").decrypt();
		case eHero::HERO_SYMMETRA:
			return skCrypt("Symmetra").decrypt();
		case eHero::HERO_ZENYATTA:
			return skCrypt("Zenyatta").decrypt();
		case eHero::HERO_GENJI:
			return skCrypt("Genji").decrypt();
		case eHero::HERO_ROADHOG:
			return skCrypt("Roadhog").decrypt();
		case eHero::HERO_MCCREE:
			return skCrypt("McCree").decrypt();
		case eHero::HERO_JUNKRAT:
			return skCrypt("Junkrat").decrypt();
		case eHero::HERO_ZARYA:
			return skCrypt("Zarya").decrypt();
		case eHero::HERO_SOLDIER76:
			return skCrypt("Soldier 76").decrypt();
		case eHero::HERO_LUCIO:
			return skCrypt("Lucio").decrypt();
		case eHero::HERO_DVA:
			if (SDK->RPM<uint16_t>(LinkBase + 0xD4) != SDK->RPM<uint16_t>(LinkBase + 0xD8))
				return skCrypt("D.Va").decrypt();
			else
				return skCrypt("Hana").decrypt();
		case eHero::HERO_MEI:
			return skCrypt("Mei").decrypt();
		case eHero::HERO_ANA:
			return skCrypt("Ana").decrypt();
		case eHero::HERO_SOMBRA:
			return skCrypt("Sombra").decrypt();
		case eHero::HERO_ORISA:
			return skCrypt("Orisa").decrypt();
		case eHero::HERO_DOOMFIST:
			return skCrypt("Doomfist").decrypt();
		case eHero::HERO_MOIRA:
			return skCrypt("Moira").decrypt();
		case eHero::HERO_BRIGITTE:
			return skCrypt("Brigitte").decrypt();
		case eHero::HERO_WRECKINGBALL:
			return skCrypt("Wrecking Ball").decrypt();
		case eHero::HERO_SOJOURN:
			return skCrypt("Sojourn").decrypt();
		case eHero::HERO_ASHE:
			return skCrypt("Ashe").decrypt();
		case eHero::HERO_BAPTISTE:
			return skCrypt("Baptiste").decrypt();
		case eHero::HERO_KIRIKO:
			return skCrypt("Kiriko").decrypt();
		case eHero::HERO_JUNKERQUEEN:
			return skCrypt("Junker Queen").decrypt();
		case eHero::HERO_SIGMA:
			return skCrypt("Sigma").decrypt();                    
		case eHero::HERO_ECHO:
			return skCrypt("Echo").decrypt();
		case eHero::HERO_RAMATTRA:
			return skCrypt("Ramattra").decrypt();
		case eHero::HERO_TRAININGBOT1:
			return skCrypt("Bot").decrypt();
		case eHero::HERO_TRAININGBOT2:
			return skCrypt("Bot").decrypt();
		case eHero::HERO_TRAININGBOT3:
			return skCrypt("Bot").decrypt();
		case eHero::HERO_TRAININGBOT4:
			return skCrypt("Bot").decrypt();
		default:
			return skCrypt("Unknown").decrypt();
		}
	}

	inline uint64_t GetParent(uint64_t encrypted) {
		__try {
			auto result = encrypted;
			result -= 0x401C60913E3B91CE;
			result = (result >> 0x20) | (result << 0x20);
			return result;
		}
		__except (1) {

		}
	}

	inline unsigned __int64 __fastcall DecryptVis(__int64 a1) 

	{
		uint64_t v2;
		uint64_t v3;
		uint64_t v4;
		uint64_t v5;
		uint64_t v6;
		uint64_t v7;
		__m128i v8;
		__m128i v9;
		__m128i v10;
		__m128i v11;

		v2 = SDK->dwGameBase + 0x7A4552;
		v3 = v2 + 0x8;
		v4 = 0x0;
		v5 = SDK->RPM<uint64_t>(SDK->dwGameBase + 0x3AA7180
			+ 8 * (((uint8_t)a1 - 0x11) & 0x7F)
			+ (((uint64_t)(a1 + 0x220329B4B2A3D7EF) >> 7) & 7)) ^ v2 ^ (a1 + 0x220329B4B2A3D7EF);
		v6 = (v3 - v2 + 0x7) >> 0x3;
		if (v2 > v3)
			v6 = 0x0;
		if (v6 && v6 >= 0x4)
		{
			v7 = v6 & 0xFFFFFFFFFFFFFFFC;
			ZeroMemory(&v8, sizeof(v8));
			ZeroMemory(&v9, sizeof(v9));
			do
			{
				v4 += 0x4;
				v8 = _mm_xor_si128(v8, _mm_loadu_si128((const __m128i*)v2));
				v10 = _mm_loadu_si128((const __m128i*)(v2 + 0x10));
				v2 += 0x20;
				v9 = _mm_xor_si128(v9, v10);
			} while (v4 < v7);
			v11 = _mm_xor_si128(v8, v9);
			v5 ^= _mm_xor_si128(v11, _mm_srli_si128(v11, 8)).m128i_u64[0];
		}
		for (; v2 < v3; v2 += 0x8)
			v5 ^= SDK->RPM<uint64_t>(v2);
		return v5 ^ ~v3 ^ 0x220329B4B2A3D7EF;
	}

	inline uint64_t DecryptComponent(uint64_t parent, uint8_t idx) 
	{
		__try {
			if (parent)
			{
				unsigned __int64 v1 = parent;
				unsigned __int64 v2 = (uint64_t)1 << (uint64_t)(idx & 0x3F);
				unsigned __int64 v3 = v2 - 1;
				unsigned __int64 v4 = idx & 0x3F;
				unsigned __int64 v5 = idx / 0x3F;
				unsigned __int64 v6 = SDK->RPM<uint64_t>(v1 + 8 * (uint32_t)v5 + 0xF0);
				__int64 v7 = (v2 & SDK->RPM<uint64_t>(v1 + 8 * (uint32_t)v5 + 0xF0)) >> v4;
				unsigned __int64 v8 = (v3 & v6) - (((v3 & v6) >> 1) & 0x5555555555555555);
				unsigned __int64 v9 = SDK->RPM<uint64_t>(SDK->RPM<uint64_t>(v1 + 0x60) + 8 * (SDK->RPM<uint8_t>((uint32_t)v5 + v1 + 0x110) + ((0x101010101010101 * (((v8 & 0x3333333333333333) + ((v8 >> 2) & 0x3333333333333333) + (((v8 & 0x3333333333333333) + ((v8 >> 2) & 0x3333333333333333)) >> 4)) & 0xF0F0F0F0F0F0F0F)) >> 0x38)));
				unsigned __int64 Key1 = SDK->GlobalKey1;
				unsigned __int64 Key2 = SDK->GlobalKey2;
				uint64_t D1 = (unsigned int)v9 | v9 & 0xFFFFFFFF00000000ui64 ^ ((unsigned __int64)((unsigned int)v9
					- (unsigned int)SDK->RPM<uint64_t>(SDK->dwGameBase + 0x3AA6160 + (Key1 & 0xFFF))) << 32);
				uint64_t D2 = (unsigned int)Key2 ^ (unsigned int)D1 | (Key2 ^ ((unsigned int)D1 | D1 & 0xFFFFFFFF00000000ui64 ^ ((unsigned __int64)(unsigned int)(970067735 - D1) << 32))) & 0xFFFFFFFF00000000ui64 ^ ((unsigned __int64)(2 * ((unsigned int)Key2 ^ (unsigned int)D1) - __ROL4__(SDK->RPM<uint64_t>(SDK->dwGameBase + 0x3AA6160 + (Key1 >> 52)), 9)) << 32);
				uint64_t D3 = -(int)v7 & ((unsigned int)D2 | D2 & 0xFFFFFFFF00000000ui64 ^ ((unsigned __int64)(unsigned int)(HIDWORD(SDK->RPM<uint64_t>(SDK->dwGameBase + 0x3AA6160 + (Key1 & 0xFFF))) - D2) << 32));

				return D3;
			}
		}
		__except (1) {

		}
	}

	inline uint64_t GetSenstivePTR()
	{
		uint64_t tmp = SDK->RPM<uint64_t>(SDK->dwGameBase + offset::Address_sens_base);
		tmp = SDK->RPM<uint64_t>(tmp + 0x38);
		tmp = SDK->RPM<uint64_t>(tmp + 0x8);
		tmp = SDK->RPM<uint64_t>(tmp + 0x8);
		tmp = SDK->RPM<uint64_t>(tmp + 0x8);
		tmp += 0x1FC8;
		return tmp;
	}

	inline uint64_t GetHeapManager(uint8_t index) 
	{
		uint64_t v5 = SDK->RPM<uint64_t>(SDK->RPM<uint64_t>(SDK->dwGameBase + offset::Address_HeapManager) + 0x158);
		v5 += 0x393770CF029B96C7;
		v5 ^= 0x4C27BCB15D3ABEB1;
		return SDK->RPM<uint64_t>(v5 + (index * 0x8));
	}
	 
	__forceinline std::vector<std::pair<uint64_t, uint64_t>> get_ow_entities()
	{
		std::vector<std::pair<uint64_t, uint64_t>> result{};

		struct Entity {
			uint64_t entity;
			uint64_t pad;
		};

		uintptr_t entity_list = SDK->RPM<uint64_t>(SDK->dwGameBase + offset::Address_entity_base); 

		MEMORY_BASIC_INFORMATION mbi{};
		VirtualQueryEx(SDK->hProcess, (LPCVOID)entity_list, &mbi, sizeof(mbi));

		SIZE_T entity_list_size = mbi.RegionSize, count = 0, count2 = 0; 
		Entity* raw_list = new Entity[entity_list_size];

		for (size_t i = entity_list_size; i > 0; i -= 0x400) // 이거 홍승이가 올린거쓰나보네 ㄷㄷ 
		{
			if (ReadProcessMemory(SDK->hProcess, reinterpret_cast<PVOID>(entity_list), raw_list, i * sizeof(Entity), nullptr))
			{
				entity_list_size = i;
				break;
			}
		}

		for (size_t i = 0; i < entity_list_size; ++i)
		{
			uint64_t cur_entity = raw_list[i].entity;
			if (!cur_entity)
				continue;
			uint64_t common_linker = DecryptComponent(cur_entity, TYPE_LINK);
			if (!common_linker)
				continue;
			auto _get_entity_by_unique_id = [&raw_list, &entity_list_size](uint32_t unique_id) -> uint64_t
			{
				for (size_t x = 0; x < entity_list_size; ++x)
				{
					uint64_t cur_entity = raw_list[x].entity;
					if (!cur_entity)
						continue;

					if (SDK->RPM<uint32_t>(cur_entity + 0x118) == unique_id)
						return cur_entity;
				}

				return 0;
			};

			uint64_t common = _get_entity_by_unique_id(SDK->RPM<uint32_t>(common_linker + 0xD4)), dummy; // D4 
			if (!ReadProcessMemory(SDK->hProcess, LPVOID(common), &dummy, 8, nullptr))
				continue;
			if (!cur_entity)
				continue;
			result.emplace_back(common, cur_entity);
		}
		return result;
	}
}