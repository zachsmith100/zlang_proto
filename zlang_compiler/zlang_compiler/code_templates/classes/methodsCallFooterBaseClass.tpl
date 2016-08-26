if (count == 1)
	{
		return MY->GetProperty__(callStatus, (ClassPropertyEnum)method);
	}			
	else
	{
		Log.Warnings.AddLine2s("Attempt to call unknown method %s which must use %s parameters", methodNames[(int)method], count - 3); 
	}
	return result;
}
