if ((name == "[[methodName]]") && (adt->GetLength() == [[paramCount]]))
	{
[[cycle|initVar|methodsCallByNameVar.tpl]]		result = [[methodName]](MY[[cycle|param|methodsCallByNameParam.tpl]]);
	}
	else 