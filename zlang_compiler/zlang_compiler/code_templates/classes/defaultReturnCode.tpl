if (defaultResultNeeded)
		{
			result.PermitChangeType();
			result = result->GetProperty__(0, __PROP_ENUM__[[defaultField]]);
			result.ProhibitChangeType();
		}