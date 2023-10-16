#pragma once
#include "includes.hpp"
#include "SDK.hpp"
#include "Overwatch.hpp"


void MainThread() {
	using namespace OW;
	if (!SDK->Initialize() || !SDK->GetGlobalKey())
		exit(0);


	_beginthread((_beginthread_proc_type)entity_scan_thread, 0, 0);
	_beginthread((_beginthread_proc_type)entity_thread, 0, 0);
	_beginthread((_beginthread_proc_type)viewmatrix_thread, 0, 0);
	_beginthread((_beginthread_proc_type)aimbot_thread, 0, 0);
	_beginthread((_beginthread_proc_type)overlay_thread, 0, 0);

	while (FindWindowA(skCrypt("TankWindowClass"), NULL))
	{
		auto viewMatrixVal = SDK->RPM<uint64_t>(SDK->dwGameBase + offset::Address_viewmatrix_base) ^ offset::offset_viewmatrix_xor_key;
		viewMatrixVal = SDK->RPM<uint64_t>(viewMatrixVal + 0x5B0);
		viewMatrixVal = SDK->RPM<uint64_t>(viewMatrixVal + 0x118);

		viewMatrix_xor_ptr = viewMatrixVal + 0x130;

		auto view = SDK->RPM<uint64_t>(SDK->dwGameBase + offset::Address_viewmatrix_base_test) + offset::offset_viewmatrix_ptr; 
		viewMatrixPtr = view;

		Sleep(100);
	}
}

int main() {
	using namespace OW;

	bool is_login = false;
	std::string license{};
	MainThread();

}