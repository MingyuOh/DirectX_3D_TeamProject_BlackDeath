#pragma once


class LAHero;
class LAKeyStateCommand
{
public:
	virtual void execute(LAHero* pObject, int ikeyState) = 0;
public:
	LAKeyStateCommand();
	virtual ~LAKeyStateCommand();
};

