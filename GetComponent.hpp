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
			OFF_MEMB(XMFLOAT3, location, 0x200);
			OFF_MEMB(uint64_t, bonedata, 0x870);

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
		case eHero::HERO_CASSIDY:
			return skCrypt("Cassidy").decrypt();
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
		case eHero::HERO_LIFEWEAVER:
			return skCrypt("Lifeweaver").decrypt();
		case eHero::HERO_ILLARI:
			return skCrypt("Illari").decrypt();
		case eHero::HERO_TRAININGBOT1:
			return skCrypt("Bot").decrypt();
		case eHero::HERO_TRAININGBOT2:
			return skCrypt("Bot").decrypt();
		case eHero::HERO_TRAININGBOT3:
			return skCrypt("Bot").decrypt();
		case eHero::HERO_TRAININGBOT4:
			return skCrypt("Bot").decrypt();
		case eHero::HERO_TRAININGBOT5:
			return skCrypt("Bot").decrypt();
		case eHero::HERO_TRAININGBOT6:
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

	inline unsigned __int64 __fastcall DecryptVis(__int64 a1) {
		__int64 v1; // rsi
		unsigned __int64 v2; // rbx
		unsigned __int64 v3; // r9
		__int64 v4; // rsi
		unsigned __int64 v5; // rdx
		unsigned __int64 v6; // rcx
		__m128i v7; // xmm1
		__m128i v8; // xmm2
		__m128i v9; // xmm0
		__m128i v10; // xmm1
		v1 = a1;
		v2 = SDK->dwGameBase + offset::Visible_Fn;
		v3 = v2 + 0x8;
		v4 = SDK->RPM<uint64_t>(SDK->dwGameBase + 0x37451C0
			+ 8 * (((uint8_t)a1 + 0x51) & 0x7F)
			+ (((uint64_t)(a1 - offset::Visible_Key) >> 7) & 7)) ^ v2 ^ (a1 - offset::Visible_Key);
		v5 = (v3 - v2 + 7) >> 3;
		v6 = 0i64;
		if (v2 > v3)
			v5 = 0i64;
		if (v5) {
			if (v5 >= 4) {
				ZeroMemory(&v7, sizeof(v7));
				ZeroMemory(&v8, sizeof(v8));
				do {
					v6 += 4i64;
					v7 = _mm_xor_si128(v7, _mm_loadu_si128((const __m128i*)v2));
					v9 = _mm_loadu_si128((const __m128i*)(v2 + 16));
					v2 += 32i64;
					v8 = _mm_xor_si128(v8, v9);
				} while (v6 < (v5 & 0xFFFFFFFFFFFFFFFCui64));
				v10 = _mm_xor_si128(v7, v8);
				auto addr = _mm_xor_si128(v10, _mm_srli_si128(v10, 8));
				v4 ^= *(__int64*)&addr;
			}
		}
		for (; v6 < v5; ++v6) {
			v4 ^= SDK->RPM<uintptr_t>(v2);
			v2 += 8i64;
		}
		return v4 ^ ~v3 ^ 0x9233688BA979DB51;
	}

	inline uintptr_t DecryptComponent(uintptr_t parent, uint8_t idx) {
		__try {
			if (parent) {
				unsigned __int64 v1 = parent;
				unsigned __int64 v2 = (uintptr_t)1 << (uintptr_t)(idx & 0x3F);
				unsigned __int64 v3 = v2 - 1;
				unsigned __int64 v4 = idx & 0x3F;
				unsigned __int64 v5 = idx / 0x3F;
				unsigned __int64 v6 = SDK->RPM<uintptr_t>((v1 + 8 * (uint32_t)v5 + 0x100));
				unsigned __int64 v7 = (v2 & SDK->RPM<uintptr_t>((v1 + 8 * (uint32_t)v5 + 0x100))) >> v4;
				unsigned __int64 v8 = (v3 & v6) - (((v3 & v6) >> 1) & 0x5555555555555555);
				unsigned __int64 v9 = SDK->RPM<uintptr_t>((SDK->RPM<uintptr_t>((v1 + 0x70)) + 8 * (SDK->RPM<uint8_t>(((uint32_t)v5 + v1 + 0x120)) + ((0x101010101010101 * (((v8 & 0x3333333333333333) + ((v8 >> 2) & 0x3333333333333333) + (((v8 & 0x3333333333333333) + ((v8 >> 2) & 0x3333333333333333)) >> 4)) & 0xF0F0F0F0F0F0F0F)) >> 0x38))));
				unsigned __int64 Key1 = SDK->GlobalKey1;
				unsigned __int64 Key2 = SDK->GlobalKey2;
				unsigned __int64 dummy = SDK->RPM<_QWORD>(SDK->dwGameBase + 0x37441A0 + (Key1 >> 0x34));
				unsigned __int64 dummy2 = SDK->RPM<_QWORD>(SDK->dwGameBase + 0x37441A0 + (Key1 & 0xFFF));
				unsigned __int64 v10 = Key2 ^ ((unsigned int)v9 | ((unsigned int)v9 | v9 & 0xFFFFFFFF00000000ui64 ^ (((unsigned int)v9 ^ 0xFFFFFFFFAE692641ui64) << 0x20)) & 0xFFFFFFFF00000000ui64 ^ ((unsigned __int64)((unsigned int)v9 ^ (unsigned int)~HIDWORD(dummy)) << 0x20));
				unsigned __int64 v11 = -(int)v7 & ((unsigned int)v10 | ((unsigned int)v10 | v10 & 0xFFFFFFFF00000000ui64 ^ ((unsigned __int64)((unsigned int)dummy2 - (unsigned int)v10) << 0x20)) & 0xFFFFFFFF00000000ui64 ^ ((unsigned __int64)((unsigned int)v10 ^ 0x51DD7F1) << 0x20));
				return v11;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
		return NULL;
	}

	inline uintptr_t GetHeapManager(uint8_t index) {
		uintptr_t v0 = SDK->RPM<uintptr_t>((SDK->dwGameBase + offset::HeapManager));
		if (v0 != 0) {
			auto v1 = SDK->RPM<uintptr_t>(v0 + offset::HeapManager_Pointer) ^ SDK->RPM<uintptr_t>(SDK->dwGameBase + offset::HeapManager_Var) ^ offset::HeapManager_Key;
			if (v1 != 0) {
				uintptr_t v2 = SDK->RPM<uintptr_t>((v1 + 0x8 * index));
				return v2;
			}
		}
		return NULL;
	}

	inline uintptr_t GetSenstivePTR()
	{
		uintptr_t heap = GetHeapManager(6);
		if (heap) {
			return heap + 0x1FE0;
		}
		return NULL;
	}

	__forceinline std::vector<std::pair<uint64_t, uint64_t>> get_ow_entities()
	{
		std::vector<std::pair<uint64_t, uint64_t>> result{};

		uintptr_t entity_list = SDK->RPM<uint64_t>(SDK->dwGameBase + offset::Address_entity_base);

		for (size_t i = 0x00; i < SDK->SectionSize; i += 0x10)
		{
			uint64_t cur_entity = SDK->RPM<uint64_t>(entity_list + i);
			if (!cur_entity)
				continue;
			uint64_t common_linker = DecryptComponent(cur_entity, TYPE_LINK);
			if (!common_linker)
				continue;
			uint64_t common = 0;
			uint32_t unique_id = SDK->RPM<uint32_t>(common_linker + 0xD4);
			for (size_t o = 0x00; o < SDK->SectionSize; o += 0x10)
			{
				uint64_t ent = SDK->RPM<uint64_t>(entity_list + o);
				if (!ent)
					continue;
				if (SDK->RPM<uint32_t>(ent + 0x128) == unique_id)
				{
					common = ent;
					break;
				}
			}
			if (!common)
				continue;
			result.emplace_back(common, cur_entity);
		}
		return result;
	}
}