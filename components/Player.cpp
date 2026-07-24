#define DIAGNOSTIC 
#include <engine.hpp>
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
		glm::vec2 dlook = {0,0};
		bool _throw = 0;
		bool _attack = 0;
		bool _pick = 0;

		if ( ReKat::Graphik::Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
		if ( ReKat::Graphik::Key_Pressed( "W" ) ) { dpos += glm::vec2{0,1};  }
		if ( ReKat::Graphik::Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
		if ( ReKat::Graphik::Key_Pressed( "S" ) ) { dpos += glm::vec2{0,-1}; }

		if ( ReKat::Graphik::Key_Pressed( "Left" ) ) { dlook += glm::vec2{-1,0}; }
		if ( ReKat::Graphik::Key_Pressed( "Up" ) ) { dlook += glm::vec2{0,1};  }
		if ( ReKat::Graphik::Key_Pressed( "Right" ) ) { dlook += glm::vec2{1,0};  }
		if ( ReKat::Graphik::Key_Pressed( "Down" ) ) { dlook += glm::vec2{0,-1}; }
		
		if ( ReKat::Graphik::Key_Pressed( "E" ) ) { _puppet->PickWeapon ( ); }
		if ( ReKat::Graphik::Key_Pressed( "Q" ) ) { _puppet->Throw ( ); }
		if ( ReKat::Graphik::Key_Pressed( "Shift" ) ) { _puppet->Throw ( ); }
		if ( ReKat::Graphik::Key_Pressed( "Space" ) ) { _puppet->Swing ( ); }

		_puppet->SetDirection ( vec3{dpos,0.0f} );
		if ( dlook != vec2(0) )
		{
			_puppet->SetAttackDirection ( vec3{dlook,0.0f} );
		}
	}

	void Collision_Trigger ( Objekt * _obj )
	{
		if ( _obj->Has_Component < Weapon > ( ) )
		{
			// this is a weapon you can get it
		}
	}
};