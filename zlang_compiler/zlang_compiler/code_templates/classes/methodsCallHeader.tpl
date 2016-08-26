CUniVar [[className]]Proto::CallMethod___([[className]]Proto* MY, int count, ...)
{
	va_list vl;
	va_start(vl, count);
	CUniVar result = CallMethod___(MY, count, vl);
	va_end(vl);	
	return result;
}

CUniVar [[className]]Proto::CallMethod___([[className]]Proto* MY, int count, va_list& vl)
{
	ClassMethodEnum method = (ClassMethodEnum)va_arg(vl, int);
	bool defaultResultNeeded = (va_arg(vl, int) != 0); 
	int callStatus = va_arg(vl, int); 
	return CallMethod___(MY, count, vl, method, callStatus, defaultResultNeeded);
}			

CUniVar [[className]]Proto::CallMethod___([[className]]Proto* MY, int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded)
{
	CUniVar result;
	if (CheckForBuiltInMethod(MY, method, count, vl, defaultResultNeeded, result))	
	{ 
		return result; 
	}
	