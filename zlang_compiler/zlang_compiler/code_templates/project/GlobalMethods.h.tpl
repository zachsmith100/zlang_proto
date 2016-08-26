#pragma once

extern CUniVar CallMethodByName__(const std::string& className, void* MY, const std::string& name, CArrayDataContainer* adt);
extern bool IsGlobalFunctionExist(CUniVar _name);
extern CUniVar CallGlobalFunctionByName__(CClassDataContainer* MY, const std::string& name, CArrayDataContainer* adt);