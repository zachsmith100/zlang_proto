class [[className]]Proto : public [[baseClassName]]
{
[[classContents]]
};

class [[className]] : public CUniClass<[[className]]Proto> 
{[[singletonCode]]
};

