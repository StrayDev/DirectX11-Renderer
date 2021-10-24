#pragma once

class Buffer
{
public:
	Buffer() = default;
	virtual ~Buffer() = default;

	virtual void BindToPipeline() = 0;

private:

};
