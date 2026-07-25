#include <engine.hpp>
#include "file_refes.h"
#include "Runner.cpp"

class Runner_Manager : public Behaviour
{
private:
	std::vector < Behaviour* > _runners;
	std::vector < Behaviour* > _weapons;
public:
	Runner_Manager ( )
	{
		Informations = { "Runner_Manager", 1.0, "The Count down / life manager" };
	}

	Runner_Manager* Register (
		std::string name,
		int head,
		int legs,
		vec3 spawn_point,
		std::string controller
	) {
		auto _runner = Manager::Objekt_Load ( name, spawn_point );

		_runner->Add_Component < Runner >( )->Set ( head, legs );
		_runner->Add_Component ( controller );

		obj->Add_Child ( _runner );

		return this;
	}

	template < class Weap > 
	Runner_Manager* Register_weapon (
		std::string weapon_name,
		int sprite_number,
		vec3 spawn_point,
		vec3 collider_size
	) {
		auto _weapon = Manager::Objekt_Load ( weapon_name, spawn_point );

		_weapon->Add_Component < Box_Collider > ( )
			->Set_Size ( collider_size )
			->Set_Trigger ( true );
		
		_weapon->Add_Component < Sprite > ( )
			->Set ( WEAPONS_SHEET, "", "", {16,1}, sprite_number )
			.Set ( true );

		_weapon->Add_Component < Weap > ( );

		Manager::Objekt_Get ( "Weapons" )->Add_Child ( _weapon );

		return this;
	}
};