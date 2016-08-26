
private:
	[[className]]()
	{
	};
	~[[className]]()
	{
	};
public:
	static [[className]]* GetInstance()
	{
		static [[className]]* obj = NULL;
		if (!obj) obj = new [[className]]();
		return obj;
	}