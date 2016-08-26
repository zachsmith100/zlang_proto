static CClassDataContainer* GetClassInstance(const std::string& name)
{
[[cycle|classes|classFactoryItem.tpl]]
	return new CClassDataContainer();
}