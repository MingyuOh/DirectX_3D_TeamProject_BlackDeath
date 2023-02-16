#pragma once

#include "LAKeyStateCommand.h"

#include "LAHero.h"

class LAUp : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->Up(ikeyState);

	}
public:
	LAUp() { ; }
	virtual ~LAUp() { ; }
};

class LADown : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->Down(ikeyState);
	}
public:
	LADown() { ; }
	virtual ~LADown() { ; }
};

class LALeft : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->Left(ikeyState);
	}
public:
	LALeft() { ; }
	virtual ~LALeft() { ; }
};

class LARight : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->Right(ikeyState);
	}
public:
	LARight() { ; }
	virtual ~LARight() { ; }
};

class LAIdle : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->Idle(ikeyState);
	}
public:
	LAIdle() { ; }
	virtual ~LAIdle() { ; }
};

class LAWork : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->Work(ikeyState);
	}
public:
	LAWork() { ; }
	virtual ~LAWork() { ; }
};

class LARun : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->Run(ikeyState);
	}
public:
	LARun() { ; }
	virtual ~LARun() { ; }
};

class LAJump : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		//pObject->Jump(ikeyState);
	}
public:
	LAJump() { ; }
	virtual ~LAJump() { ; }
};

class LABulletReLoad : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->BulletReLoad(ikeyState);
	}
public:
	LABulletReLoad() { ; }
	virtual ~LABulletReLoad() { ; }
};

class LAAttack : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->Attack(ikeyState);
	}
public:
	LAAttack() { ; }
	virtual ~LAAttack() { ; }
};

class LARifle : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->SetWeaponRifle(ikeyState);
	}
public:
	LARifle() { ; }
	virtual ~LARifle() { ; }
};

class LAHandgun : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->SetWeaponHandgun(ikeyState);
	}
public:
	LAHandgun() { ; }
	virtual ~LAHandgun() { ; }
};

class LAKnife : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->SetWeaponKnife(ikeyState);
	}
public:
	LAKnife() { ; }
	virtual ~LAKnife() { ; }
};

class LABomb : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->SetWeaponBomb(ikeyState);
	}
public:
	LABomb() { ; }
	virtual ~LABomb() { ; }
};

class LAMedic : public LAKeyStateCommand
{
public:
	void execute(LAHero* pObject, int ikeyState)
	{
		pObject->SetWeaponMedic(ikeyState);
	}
public:
	LAMedic() { ; }
	virtual ~LAMedic() { ; }
};

