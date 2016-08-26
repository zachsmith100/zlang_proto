		
		if (defaultResultNeeded)
		{
			result.PermitChangeType();
			result = result.GetProperty__(1, __PROP_ENUM__[[fieldName]]);
			result.ProhibitChangeType();
		}