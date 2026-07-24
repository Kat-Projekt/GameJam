#define DIAGNOSTIC 
#include <engine.hpp>
#include "Weapon.cpp"
#include "Runner.cpp"

class Player : public Behaviour
{
private:
	Runner* _puppet;
public:
	Player ( )
	{
		Informations = { "Player", 1.0, "The player controller" };
	}

	void Start ( )
	{
		_puppet = obj->Get_Component < Runner > ( );
	}

	void Update ( )
	{
		glm::vec2 dpos = {0,0};
		if ( ReKat::Graphik::Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
		if ( ReKat::Graphik::Key_Pressed( "W" ) ) { dpos += glm::vec2{0,1};  }
		if ( ReKat::Graphik::Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
		if ( ReKat::Graphik::Key_Pressed( "S" ) ) { dpos += glm::vec2{0,-1}; }

		if ( dpos == vec2{0,0} )
		{ _puppet->Stay ( ); }
		else
		{ _puppet->SetDirection ( vec3{dpos,0.0f} ); }
	}

	void Collision_Trigger ( Objekt * _obj )
	{
		if ( _obj->Has_Component < Weapon > ( ) )
		{
			// this is a weapon you can get it
		}
	}
};