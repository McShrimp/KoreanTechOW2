#pragma once
#include "includes.hpp"
#include "rsa.h"
namespace OW {
	inline std::vector<string> split(std::string input, char delimiter) {
		std::vector<string> answer;
		std::stringstream ss(input);
		std::string temp;

		while (getline(ss, temp, delimiter)) {
			answer.push_back(temp);
		}

		return answer;
	}
	inline std::string exec(const char* cmd) {
		std::array<char, 128> buffer;
		std::string result;
		std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
		if (!pipe) {
			throw std::runtime_error(skCrypt("popen() failed!"));
		}
		while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
			result += buffer.data();
		}
		return result;
	}
	inline std::string HashHardwareComponents(std::string components) {
		std::stringstream in(components);
		std::stringstream out;
		std::string buff;
		std::hash<std::string> hasher;

		while (std::getline(in, buff, '\n')) {
			size_t hash = hasher(buff);
			out << std::hex << hash;
		}

		return out.str();
	}
	inline std::string GetHardwareID() {

		std::string hwComponents = exec(
			"wmic PATH Win32_VideoController GET PNPDeviceID    | more +1  | findstr /R \"[^\\n]\" &"
			"wmic PATH Win32_PhysicalMemory GET SerialNumber    | more +1  | findstr /R \"[^\\n]\" &"
			"wmic PATH Win32_LogicalDisk GET VolumeSerialNumber | more +1  | findstr /R \"[^\\n]\" &"
			"wmic PATH Win32_Processor GET ProcessorId          | more +1  | findstr /R \"[^\\n]\" &"
			"wmic PATH win32_computersystem GET name            | more +1  | findstr /R \"[^\\n]\"");


		return HashHardwareComponents(hwComponents);
	}
	inline std::string RandomString(int Count)
	{
		char AlphaNum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

		int stringLength = sizeof(AlphaNum) - 1;

		srand(clock());
		std::string Str;
		for (int i = 0; i < Count; ++i)
			Str += AlphaNum[rand() % stringLength];
		return Str;
	}
	inline void input(std::string text, std::string& str)
	{
		while (str.length() == 0 && str.length() > 5)
		{
			cout << text << skCrypt(" : ");
			cin >> str;
		}
	}
	inline std::string url_encode(const std::string& value)
	{
		ostringstream escaped;
		escaped.fill('0');
		escaped << hex;

		for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
			std::string::value_type c = (*i);

			// Keep alphanumeric and other accepted characters intact
			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
				escaped << c;
				continue;
			}

			// Any other characters are percent-encoded
			escaped << uppercase;
			escaped << '%' << setw(2) << int((unsigned char)c);
			escaped << nouppercase;
		}

		return escaped.str();
	}
	inline std::string get_system_uuid()
	{
		if (std::system(skCrypt("wmic csproduct get uuid > HWID.txt")) == 0)
		{
			auto file = ::fopen(skCrypt("HWID.txt"), skCrypt("rt, ccs=UNICODE")); // open the file for unicode input
			enum { BUFFSZ = 1000, UUID_SZ = 36 };
			wchar_t wbuffer[BUFFSZ]; // buffer to hold unicode characters
			if (file && ::fgetws(wbuffer, BUFFSZ, file) && ::fgetws(wbuffer, BUFFSZ, file))
			{
				char cstr[BUFFSZ];
				if (::wcstombs(cstr, wbuffer, BUFFSZ) > UUID_SZ)
				{
					std::string uuid = cstr;
					while (!uuid.empty() && std::isspace(uuid.back())) uuid.pop_back(); // discard trailing white space
					fclose(file);
					remove(skCrypt("HWID.txt"));
					return uuid;
				}
			}
		}
		return {};
	}
	inline std::wstring get_utf16(const std::string& str, int codepage)
	{
		if (str.empty())
			return std::wstring();
		int sz = MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), 0, 0);
		std::wstring res(sz, 0);
		MultiByteToWideChar(codepage, 0, &str[0], (int)str.size(), &res[0], sz);
		return res;
	}
	inline std::string WebWinhttp(std::string Domain, std::string Url)
	{
		DWORD dwSize = 0, dwDownloaded;
		LPSTR source;
		source = (char*)"";
		std::string responsed = "";
		HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
		BOOL bResults = FALSE;
		hSession = WinHttpOpen(L"", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
		if (hSession)
			hConnect = WinHttpConnect(hSession, get_utf16(Domain, CP_UTF8).c_str(), 80, 0);
		if (hConnect)
			hRequest = WinHttpOpenRequest(hConnect, skCrypt(L"POST"), get_utf16(Url, CP_UTF8).c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		if (hRequest)
			bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
		if (bResults)
			bResults = WinHttpReceiveResponse(hRequest, NULL);
		if (bResults)
		{
			do
			{
				dwSize = 0;
				if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
					printf(skCrypt("Error %u"), GetLastError());
				source = (char*)malloc(dwSize + 1);
				if (!source)
					dwSize = 0;
				else
				{
					ZeroMemory(source, dwSize + 1);
					if (!WinHttpReadData(hRequest, (LPVOID)source, dwSize, &dwDownloaded))
						printf(skCrypt("Error %u"), GetLastError());
					else
						responsed = responsed + source;
					free(source);
				}
			} while (dwSize > 0);
		}
		if (!bResults)
			return "";
		if (hRequest) WinHttpCloseHandle(hRequest);
		if (hConnect) WinHttpCloseHandle(hConnect);
		if (hSession) WinHttpCloseHandle(hSession);
		return responsed;
	}
	inline bool DisableProxies(void)
	{
		HKEY hKey;
		DWORD data = 0;

		if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT(skCrypt("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings")), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
			return false; // Key could not be opened
		else
			return true;

		if (RegSetValueExW(hKey, skCrypt(L"ProxyEnable"), 0, REG_DWORD, (LPBYTE)&data, sizeof(data)) != ERROR_SUCCESS)
			return false; // Failed to set the value
		else
			return true;

		if (RegCloseKey(hKey) != ERROR_SUCCESS)
			return false; // Could not close key.
		else
			return true;
	}
	inline bool hosts() {
		std::ifstream file(skCrypt("C:\\Windows\\System32\\drivers\\etc\\hosts"));
		std::istreambuf_iterator<char> begin(file), end;
		std::string host = std::string(begin, end);
		//std::cout <<host << endl;
		if (host.find(skCrypt("112.175.185.26")) != std::string::npos || host.find(skCrypt("core24.net")) != std::string::npos || host.find(skCrypt("core24.dothome.co.kr")) != std::string::npos || host.find(skCrypt("panel.core24.net")) != std::string::npos) {
			return false;
		}
		return true;
	}
}