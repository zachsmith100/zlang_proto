	private:
		std::map<ClassPropertyEnum, CUniVar*> m_vars;
	public:
		CUniVar& GetProperty__(const int callStatus, const ClassPropertyEnum propId)
		{
			std::map<ClassPropertyEnum, PropertyStruct>::iterator it = m_vars.find(propId); 
			if (it != m_vars.end()) 
			{ 
				if (callStatus >= ((*it).second).callStatus)
				{
					return *(((*it).second).var); 
				}
				else
				{
					static CUniVar nullVar;
					nullVar = 0;
					return nullVar;
				}
			} 
			else 
			{ 
				PropertyStruct newProp(new CUniVar(0), (callStatus == 2) ? 1 : 0);
				m_vars.insert(std::map<ClassPropertyEnum, PropertyStruct>::value_type(propId, newProp)); 
				return *(newProp.var); 
			} 
		}
		~[[className]]()
		{
			std::map<ClassPropertyEnum, PropertyStruct>::iterator it = m_vars.begin();
			std::map<ClassPropertyEnum, PropertyStruct>::iterator itEnd = m_vars.end();

			for ( ; it != itEnd; it++)
			{
				delete (*it).second.var;
			}
		}
