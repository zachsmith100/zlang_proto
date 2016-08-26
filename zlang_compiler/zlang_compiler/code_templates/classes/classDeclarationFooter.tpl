	[[name]]Proto();
	static CUniVar CallMethod___([[name]]Proto* MY, int count, ...);
	static CUniVar CallMethod___([[name]]Proto* MY, int count, va_list& vl);
	static CUniVar CallMethod___([[name]]Proto* MY, int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded);
	static CUniVar CallMethodByName___([[name]]Proto* MY, const std::string& name, CArrayDataContainer* adt);
	virtual CUniVar CallMethod__(int count, ...)
	{
		va_list vl; 
		va_start(vl, count); 
		CUniVar result = CallMethod___(this, count, vl); 
		va_end(vl);	 
		return result;
	}
	virtual CUniVar CallMethod__(int count, va_list& vl)
	{
		return CallMethod___(this, count, vl);
	}
	virtual CUniVar CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded)
	{
		return CallMethod___(this, count, vl, method, callStatus, defaultResultNeeded);
	}
	virtual CUniVar CallMethodByName__(const std::string& name, CArrayDataContainer* adt)
	{
		return CallMethodByName___(this, name, adt);
	}
	bool IsMethodExist(CUniVar name);