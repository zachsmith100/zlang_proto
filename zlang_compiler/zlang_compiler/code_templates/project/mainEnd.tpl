
int [[entryPointName]](int argc, char* argv[])
{
	Log.SetCommandLine(argc, argv);
	[[entryPointCreatingCode]]

	if ((argc == 5) && (strcmp(argv[1], "execute") == 0))
	{
		std::string className = argv[2];
		std::string methodName = argv[3];
		CArrayDataContainer* adt = new CArrayDataContainer();
		adt->UpdateStateFromJson(argv[4]);
		if (className != ":global:")
		{
			CClassDataContainer* cdt = GetClassInstance(argv[2]);
			if (cdt)
			{
				if (cdt->IsMethodExist(methodName))
				{
					println(cdt->CallMethodByName__(methodName, adt));
				}
				else
				{
					println(CUniVar("Method cannot be found"));
				}
			}
			else
			{
				println(CUniVar("Class cannot be found"));
			}
		}
		else
		{
			if (IsGlobalFunctionExist(methodName))
			{
				CClassDataContainer cdt;
				println(CallGlobalFunctionByName__(&cdt, methodName, adt));				
			}
			else
			{
				println(CUniVar("Global function cannot be found"));
			}
		}
		delete adt;
	}
	else
	{
		CArrayDataContainer* cmdLineParams = new CArrayDataContainer();
		for (int i = 1; i < argc; i++)
		{       	
			cmdLineParams->Push(argv[i]);
		}

		std::string exeDir, exeName;
		ParseExePath(argv[0], exeDir, exeName);		
		entryPoint->GetProperty__(1, __PROP_ENUM__processName) = CUniVar(exeName);  
		entryPoint->GetProperty__(1, __PROP_ENUM__processFolder) = CUniVar(exeDir);  
		entryPoint->GetProperty__(1, __PROP_ENUM__processPath) = CUniVar(argv[0]);  
		entryPoint->GetProperty__(1, __PROP_ENUM__commandLineParams) = CUniVar(cmdLineParams);
		entryPoint->CallMethod___(entryPoint, 3, __FUNC_ENUM__run, true, 0); 
	}
	return 0;
}