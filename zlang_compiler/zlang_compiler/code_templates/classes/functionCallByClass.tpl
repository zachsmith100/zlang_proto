
CUniVar CallMethodByName__(const std::string& className, void* MY, const std::string& name, CArrayDataContainer* adt)
{
	CUniVar result = getResultObject(false);
	[[cycle|classes|functionCallByClassItem.tpl]]
	{
		return result;
	}
	return result;
}	