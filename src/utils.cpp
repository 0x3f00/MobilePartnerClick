
#include "stdafx.h"
#include "utils.h"



FileUtils::FileUtils(void) 
{ 
	m_strIniFile = L""; 
}

FileUtils::~FileUtils(void) 
{
}


std::wstring FileUtils::GetIniValue(const std::wstring & strKey)
{
	CheckCreateIni();
	wchar_t buf[1024];
	GetPrivateProfileString(L"SaimaClick", strKey.c_str(), L"", buf, 1024, m_strIniFile.c_str());
	return buf;
}

void FileUtils::SetIniValue(const std::wstring & strKey, const std::wstring &strValue)
{
	CheckCreateIni();
	WritePrivateProfileString(L"SaimaClick", strKey.c_str(), strValue.c_str(), m_strIniFile.c_str());
}


void FileUtils::CheckCreateIni()
{
	if (!m_strIniFile.empty())
		return;

	wchar_t path[1024] = {0};
	std::wstring strPath;
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
	strPath = path;

	m_strIniFile = strPath;
	m_strIniFile += L"\\SaimaClick\\Settings.ini";

	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(m_strIniFile.c_str()))
	{
		strPath += L"\\SaimaClick";
		::CreateDirectory(strPath.c_str(), NULL);

		strPath += L"\\Settings.ini";
		HANDLE hFile = ::CreateFile(strPath.c_str(),
			GENERIC_WRITE,          // open for writing
			0,                      // do not share
			NULL,                   // default security
			CREATE_NEW,             // create new file only
			FILE_ATTRIBUTE_NORMAL,  // normal file
			NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			DWORD dwWritten;
			char buf[] = "[SaimaClick]\ncommands=find\nfind_Cmd=findWindow\n";
			::WriteFile(hFile, buf, sizeof(buf), &dwWritten, NULL);
			::CloseHandle(hFile);
		}
	}
}



namespace my_utils
{
	std::vector<std::wstring> wtokenize(const std::wstring& str, const std::wstring& delimiters)
	{
		std::vector<std::wstring> tokens;
		std::wstring::size_type delimPos = 0, tokenPos = 0, pos = 0;

		if (str.length() < 1)  return tokens;
		while (1) {
			delimPos = str.find_first_of(delimiters, pos);
			tokenPos = str.find_first_not_of(delimiters, pos);

			if (std::wstring::npos != delimPos) {
				if (std::wstring::npos != tokenPos) {
					if (tokenPos < delimPos) {
						tokens.push_back(str.substr(pos, delimPos - pos));
					}
					else {
						tokens.push_back(L"");
					}
				}
				else {
					tokens.push_back(L"");
				}
				pos = delimPos + 1;
			}
			else {
				if (std::wstring::npos != tokenPos) {
					tokens.push_back(str.substr(pos));
				}
				else {
					tokens.push_back(L"");
				}
				break;
			}
		}
		return tokens;
	}
};
