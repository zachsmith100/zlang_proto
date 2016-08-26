if ((method == __FUNC_ENUM__[[methodName]]) && (count == [[paramCount]]) && (callStatus >= [[minCallStatus]]))
	{
[[cycle|initVar|methodsCallVar.tpl]]		result = [[methodName]](MY[[cycle|param|methodsCallParam.tpl]]);[[customReturnCode]]
	}
	else 