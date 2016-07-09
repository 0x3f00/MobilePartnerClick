#ifndef UTILS_H
#define UTILS_H


class FileUtils
{
public:
	FileUtils(void);
	~FileUtils(void);

	std::wstring GetIniValue(const std::wstring & strKey);
	void SetIniValue(const std::wstring & strKey, const std::wstring &strValue);

	void CheckCreateIni();

protected:
	std::wstring m_strIniFile;
};


namespace my_utils
{
	std::vector<std::wstring> wtokenize(const std::wstring& str, const std::wstring& delimiters);
};

#endif // UTILS_H