 
#pragma once 
 
#include "ImportHeaders.h" 
#include "ClassSupport.h" 
#include "GlobalMethods.h" 
#include <stdarg.h> 
 
CUniVar getResultObject(bool resultFlag); 
 
CUniVar logInfo(CClassDataContainer* MY, CUniVar& , CUniVar& ); 
CUniVar logInfoProxy(CClassDataContainer* MY, CUniVar* , CUniVar* ); 
 
class EntryPointProto : public CClassDataContainer 
{ 
private: 
	static CUniVar makeFloatLeftComparison(EntryPointProto* MY, CUniVar& var___1); 
	static CUniVar makeFloatRightComparison(EntryPointProto* MY, CUniVar& var___1); 
	static CUniVar makeIntLeftComparison(EntryPointProto* MY, CUniVar& var___1); 
	static CUniVar makeIntRightComparison(EntryPointProto* MY, CUniVar& var___1); 
	static CUniVar makeStringLeftComparison(EntryPointProto* MY, CUniVar& var___1); 
	static CUniVar makeStringRightComparison(EntryPointProto* MY, CUniVar& var___1); 
	static CUniVar makeVarComparison(EntryPointProto* MY, CUniVar& var___1, CUniVar& var___2); 
	static CUniVar run(EntryPointProto* MY); 
 
public: 
 
	EntryPointProto(); 
	static CUniVar CallMethod___(EntryPointProto* MY, int count, ...); 
	static CUniVar CallMethod___(EntryPointProto* MY, int count, va_list& vl); 
	static CUniVar CallMethod___(EntryPointProto* MY, int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded); 
	static CUniVar CallMethodByName___(EntryPointProto* MY, const std::string& name, CArrayDataContainer* adt); 
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
}; 
 
class EntryPoint : public CUniClass<EntryPointProto>  
{ 
}; 
 
static CClassDataContainer* GetClassInstance(const std::string& name) 
{ 
	if ("EntryPoint" == name) 
	{ 
		return new EntryPointProto(); 
	} 
 
	return new CClassDataContainer(); 
}
