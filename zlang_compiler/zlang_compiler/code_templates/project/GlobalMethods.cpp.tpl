 
#include "ClassDeclarations.h" 
#include "GlobalMethods.h" 
#include "ArraySupport.h" 
 
bool IsGlobalFunctionExist(CUniVar _name) 
{ 
	std::string name = _name.ToString(); 
 
	return false; 
}	 
 
CUniVar CallGlobalFunctionByName__(CClassDataContainer* MY, const std::string& name, CArrayDataContainer* adt) 
{ 
	CUniVar result = getResultObject(false); 
	 
	{ 
		return false; 
	} 
	return result; 
} 
 

 
CUniVar CallMethodByName__(const std::string& className, void* MY, const std::string& name, CArrayDataContainer* adt) 
{ 
	CUniVar result = getResultObject(false); 
	if (className == "EntryPoint") 
	{ 
		result = EntryPointProto::CallMethodByName___((EntryPointProto*)MY, name, adt); 
	} 
	else  
	{ 
		return result; 
	} 
	return result; 
}	
