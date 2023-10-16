#pragma once
#include "includes.hpp"

namespace OW {
	using namespace std;
	class MemorySDK {
	private:
		std::vector<MEMORY_BASIC_INFORMATION64> mbis;
		bool CompressMbis = false;

		inline uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
		{
			uintptr_t modBaseAddr = 0;
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
			if (hSnap != INVALID_HANDLE_VALUE)
			{
				MODULEENTRY32 modEntry;
				modEntry.dwSize = sizeof(modEntry);
				if (Module32First(hSnap, &modEntry))
				{
					do
					{
						if (!_stricmp(modEntry.szModule, modName))
						{
							modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
							break;
						}
					} while (Module32Next(hSnap, &modEntry));
				}
			}
			CloseHandle(hSnap);
			return modBaseAddr;
		}
	public:
		HANDLE hProcess = 0;
		uint64_t dwGameBase = 0;
		uint64_t GlobalKey1 = 0, GlobalKey2 = 0;
		uint64_t g_player_controller = 0;
		size_t SectionSize;
	public:
		inline bool Initialize()
		{
			DWORD dwPID;
			GetWindowThreadProcessId(FindWindowA(skCrypt("TankWindowClass"), NULL), &dwPID);

			hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);
			dwGameBase = GetModuleBaseAddress(dwPID, skCrypt("Overwatch.exe"));

			MEMORY_BASIC_INFORMATION mbi{};
			uintptr_t entity_list = RPM<uint64_t>(dwGameBase + offset::Address_entity_base);
			VirtualQueryEx(hProcess, (LPCVOID)entity_list, &mbi, sizeof(mbi));

			SectionSize = mbi.RegionSize;

			return dwGameBase;
		}

		inline bool GetGlobalKey() {
			static auto key_sig = (BYTE*)"\x00\x00\x00\x00\x80\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x3f";
			static auto key_mask = "x???xx?xx?????xxxxxx";
			while (true) {
				uint64_t Key = FindPatternExReg(key_sig, key_mask, 0x100000) - 0x70;
				if (Key && Key < 0xf000000000000000 && RPM<uint64_t>(Key + 0x10) > 0x1000000000000000 && RPM<uint64_t>(Key + 0x28) > 0x1000000000000000) {
					GlobalKey1 = RPM<uint64_t>(Key + 0x18);
					GlobalKey2 = RPM<uint64_t>(Key + 0x28);
					cout << endl;
					cout << "GlobalKey1: 0x" << hex << GlobalKey1 << endl;
					cout << "GlobalKey2: 0x" << hex << GlobalKey2 << endl;

					return true;
				}
				Sleep(1000);
				cout << ".";
			}
		}

		template <typename WPMType>
		__forceinline bool WPM(DWORD_PTR Address, WPMType Buffer)
		{
			return WriteProcessMemory(hProcess, LPVOID(Address), &Buffer, sizeof(Buffer), nullptr);
		}

		template <typename RPMType>
		__forceinline RPMType RPM(DWORD_PTR Address)
		{
			RPMType Buffer;
			ReadProcessMemory(hProcess, LPVOID(Address), &Buffer, sizeof(Buffer), nullptr);
			return Buffer;
		}
		__forceinline uintptr_t calc_relative(uintptr_t current, int32_t relative)
		{
			return current + RPM<int32_t>(current) + relative;
		}
		__forceinline void read_buf(const DWORD_PTR address, char* buffer, SIZE_T size) {
			ReadProcessMemory(hProcess, reinterpret_cast<LPVOID>(address), (void*)buffer, size, nullptr);
		}

		__forceinline bool UpdateMemoryQuery()
		{
			MEMORY_BASIC_INFORMATION64 mbi = { 0, };
			MEMORY_BASIC_INFORMATION64 old = { 0, };
			DWORD64 current_address = 0x7ffe0000;
			std::vector<MEMORY_BASIC_INFORMATION64> addresses;
			while (true)
			{
				if (!VirtualQueryEx(hProcess, (LPVOID)current_address, (PMEMORY_BASIC_INFORMATION)&mbi, sizeof(MEMORY_BASIC_INFORMATION64)))
					break;
				if ((mbi.State & 0x1000) != 0 && (mbi.Protect & 0x100) == 0)
				{
					if (old.BaseAddress + old.RegionSize == mbi.BaseAddress && CompressMbis)
						old.RegionSize += mbi.RegionSize;
					else
						addresses.push_back(mbi);

					old = mbi;
				}
				current_address = mbi.BaseAddress + mbi.RegionSize;
			}

			mbis = addresses;


			return (mbis.size() > 0);
		}

		__forceinline DWORD64 FindPattern(BYTE* buffer, BYTE* pattern, std::string mask, int bufSize)
		{
			int pattern_len = mask.length();
			for (int i = 0; i < bufSize - pattern_len; i++)
			{
				bool found = true;
				for (int j = 0; j < pattern_len; j++)
				{
					if (mask[j] != '?' && pattern[j] != buffer[(i + j)])
					{
						found = false;
						break;
					}
				}
				if (found)
					return i;
			}
			return -1;
		}

		__forceinline std::vector<DWORD64> FindPatternEx(DWORD64 start, DWORD64 end, BYTE* pattern, std::string mask, MEMORY_BASIC_INFORMATION64 mbi, DWORD64 RgSize)
		{
			DWORD64 current_chunk = start;
			std::vector<DWORD64> found;
			if ((end - current_chunk > RgSize && RgSize != 0) || (end - current_chunk < RgSize && RgSize != 0))
				return found;
			while (current_chunk < end)
			{
				int bufSize = (int)(end - start);
				BYTE* buffer = new BYTE[bufSize];
				if (!ReadProcessMemory(hProcess, (LPVOID)current_chunk, buffer, bufSize, nullptr))
				{
					current_chunk += bufSize;
					delete[] buffer;
					continue;
				}

				DWORD64 internal_address = FindPattern(buffer, pattern, mask, bufSize);
				if (internal_address != -1)
				{
					found.push_back(current_chunk + internal_address);
				}
				current_chunk += bufSize;
				delete[] buffer;

			}
			return found;
		}

		__forceinline std::vector<DWORD64> FindPatterns(BYTE* buffer, BYTE* pattern, std::string mask, int bufSize)
		{
			std::vector<DWORD64> ret;
			int pattern_len = mask.length();
			for (int i = 0; i < bufSize - pattern_len; i++)
			{
				bool found = true;
				for (int j = 0; j < pattern_len; j++)
				{
					if (mask[j] != '?' && pattern[j] != buffer[i + j])
					{
						found = false;
						break;
					}
				}
				if (found)
					ret.push_back(i);
			}
			return ret;
		}

		__forceinline DWORD64 FindPatternExReg(BYTE* pattern, std::string mask, DWORD64 RgSize)
		{
			if (!UpdateMemoryQuery())
				return 0;

			for (int i = 0; i < mbis.size(); i++) {
				MEMORY_BASIC_INFORMATION64 info = mbis[i];

				std::vector<DWORD64> arr = FindPatternEx(info.BaseAddress, info.RegionSize + info.BaseAddress, pattern, mask, info, RgSize);
				if (arr.size() > 0)
					return arr[0];
			}
			return 0;
		}

		__forceinline std::vector<DWORD64> FindPatternExRegs(BYTE* pattern, std::string mask, DWORD64 RgSize)
		{
			std::vector<DWORD64> Result;
			if (!UpdateMemoryQuery())
				return Result;

			for (int i = 0; i < mbis.size(); i++) {
				MEMORY_BASIC_INFORMATION64 info = mbis[i];

				std::vector<DWORD64> arr = FindPatternEx(info.BaseAddress, info.RegionSize + info.BaseAddress, pattern, mask, info, RgSize);
				if (arr.size() > 0)
					Result.push_back(arr[0]);
			}

			return Result;
		}

		__forceinline std::vector<DWORD64> FindPatternsExReg(BYTE* pattern, std::string mask, DWORD64 RgSize)
		{
			std::vector<DWORD64> Result;
			DWORD64 EntityStart = FindPatternExReg(pattern, mask, RgSize);
			if (EntityStart)
			{
				for (int i = 0; i < mbis.size(); i++) {
					if (mbis[i].BaseAddress < EntityStart && EntityStart - mbis[i].BaseAddress < mbis[i].RegionSize) {
						EntityStart = mbis[i].BaseAddress;
					}
				}

				BYTE* buf = new BYTE[RgSize];
				ReadProcessMemory(hProcess, LPVOID(EntityStart), buf, RgSize, nullptr);

				std::vector<DWORD64> Pointers = FindPatterns(buf, pattern, mask, RgSize);
				delete[] buf;

				for (int i = 0; i < Pointers.size(); i++)
					Pointers[i] += EntityStart;

				Result = Pointers;
			}

			return Result;
		}

		__forceinline std::vector<DWORD64> FindPatternsExRegs(BYTE* pattern, std::string mask, DWORD64 RgSize)
		{
			std::vector<DWORD64> Result;
			std::vector<DWORD64> StartPointers = FindPatternExRegs(pattern, mask, RgSize);

			for (int i = 0; i < StartPointers.size(); i++)
			{
				for (int j = 0; j < mbis.size(); j++) {
					if (mbis[j].BaseAddress < StartPointers[i] && StartPointers[i] - mbis[j].BaseAddress < mbis[j].RegionSize) {
						StartPointers[i] = mbis[j].BaseAddress;
					}
				}

				BYTE* buf = new BYTE[RgSize];
				ReadProcessMemory(hProcess, LPVOID(StartPointers[i]), buf, RgSize, nullptr);

				std::vector<DWORD64> Pointers = FindPatterns(buf, pattern, mask, RgSize);
				delete[] buf;

				for (int j = 0; j < Pointers.size(); j++)
					Pointers[j] += StartPointers[i];

				for (int j = 0; j < Pointers.size(); j++)
				{
					Result.push_back(Pointers[j]);
				}
			}
			return Result;
		}
	};
	inline auto SDK = std::make_unique<MemorySDK>();
}