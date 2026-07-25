#include <engine.hpp>
#include "file_refes.h"

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
		int sprite,
		int temp,
		vec3 spawn_point,
		std::string controller
	) {
		auto _testa = Manager::Objekt_Load ( name, spawn_point );
		auto _gambe = std::make_shared < Objekt > ( "gambe" );

		_gambe->Add_Component < Sprite > ( )
<<<<<<< HEAD
			->Set ( RUNNER_SHEET, "", "", {4,1}, sprite_gambe )
=======
			->Set ( RUNNERS_SHEET, "", "", {4,1}, sprite )
>>>>>>> 74f9e453464425ca0c6d57ab90ae7a5486434e9f
			.Set ( true ); // 4 animation frames

		_testa->Add_Component < Sprite > ( )
			->Set ( RUNNERS_SHEET, "", "", {2,2}, sprite )
			.Set ( true ); // only the head

		_testa->Add_Component ( "Runner" );
		_testa->Add_Component ( controller );

		_testa->Add_Child ( _gambe );
		obj->Add_Child ( _testa );

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
			->Set ( WEAPONS_SHEET, "", "", {2,2}, sprite_number )
			.Set ( true );

		_weapon->Add_Component < Weap > ( );

		Manager::Objekt_Get ( "Weapons" )->Add_Child ( _weapon );

		return this;
	}
};