#include <engine.hpp>
#include "Weapon.cpp"

class Club : public Weapon
{
private:
	/* data */
public:
	virtual void Swing ( ) { }

	virtual int Throw ( ) { return 0; }
};