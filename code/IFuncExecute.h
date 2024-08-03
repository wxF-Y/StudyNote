#pragma once

class IFuncExecute
{
public:
	virtual void __stdcall InitBindFunc() = 0;
	virtual void __stdcall UninitBindFunc() = 0;

	template<class... Args>
	std::wstring __stdcall Exec(LPCWSTR szFuncName, Args&&... args)
	{
		auto argCount = sizeof...(args);
		std::vector<std::wstring> tmpVec;
		(tmpVec.emplace_back(ToStdWStr(args)), ...);
		std::vector<LPCWSTR> paramVec;
		for (auto& ws : tmpVec)
		{
			paramVec.emplace_back(ws.c_str());
		}

		LPWSTR szResult = NULL;
		LPCWSTR * pszArguments = (paramVec.empty() ? NULL : &paramVec.front());
		auto it = bind_func_map_.find(szFuncName);
		if (it != bind_func_map_.end())
		{
			it->second(pszArguments, argCount, szResult);
		}

		ASSERT(it == bind_func_map_.end());

		std::wstring strResult;
		if (szResult)
		{
			strResult = std::wstring{ szResult };
			FreeResult(szResult);
		}

		return strResult;
	}

protected:
	template<typename T>
	std::wstring __stdcall ToStdWStr(T&& data)
	{
		std::wstringstream	message;
		message << data;

		std::wstring out;
		message >> out;

		return out;
	}

	virtual void __stdcall FreeResult(LPWSTR szResult) = 0;

private:
	typedef std::function<void(LPCWSTR* pszArguments, DWORD dwArgumentCount, LPWSTR& szResult)> BindFunc;
	std::unordered_map<std::wstring, BindFunc>	bind_func_map_;
};

