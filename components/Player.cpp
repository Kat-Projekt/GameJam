#define DIAGNOSTIC 
#include <engine.hpp>
#include "Weapon.cpp"

class Player : public Behaviour
{
private:
	float speed = 200.0f;
public:
	Player ( )
	{
		Informations = { "Player", 1.0, "The player controller" };
	}

	void Start ( )
	{

	}

	void Update ( )
	{
		glm::vec2 dpos = {0,0};
		if ( ReKat::Graphik::Key_Pressed( "A" ) ) { dpos += glm::vec2{-1,0}; }
		if ( ReKat::Graphik::Key_Pressed( "W" ) ) { dpos += glm::vec2{0,1};  }
		if ( ReKat::Graphik::Key_Pressed( "D" ) ) { dpos += glm::vec2{1,0};  }
		if ( ReKat::Graphik::Key_Pressed( "S" ) ) { dpos += glm::vec2{0,-1}; }

		if ( dpos != vec2{0,0} ) { dpos = normalize ( dpos ); }
		vec2 new_move = dpos * speed * (float)Timer::delta_time;

		// obj->Get_Component < Rigidbody > ( )->velocity = ( vec3 ( dpos * speed, 0 ) );
		obj->Inc_Pos ( vec3 {new_move.x,new_move.y,0} );

		DEBUG ( 4, obj->Get_Pos ( ), new_move, (float)Timer::Get_Delta ( ), Timer::Get_Time ( ) );
	}

	void Collision_Trigger ( Objekt * _obj )
	{
		if ( _obj->Has_Component < Weapon > ( ) )
		{
			// this is a weapon you can get it
		}
	}
};