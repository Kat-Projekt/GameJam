#include <engine.hpp>

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
		int sprite_testa,
		int sprite_gambe,
		vec3 spawn_point,
		std::string controller
	) {
		auto _testa = Manager::Objekt_Load ( name, spawn_point );
		auto _gambe = std::make_shared < Objekt > ( "gambe" );

		_gambe->Add_Component < Sprite > ( )
			->Set ( "legs", "", "", {4,1}, sprite_gambe )
			.Set ( true ); // 4 animation frames

		_testa->Add_Component < Sprite > ( )
			->Set ( "head", "", "", {2,2}, sprite_testa )
			.Set ( true ); // only the head

		_testa->Add_Component ( "Runner" );
		_testa->Add_Component ( controller );

		_testa->Add_Child ( _gambe );
		obj->Add_Child ( _testa );

		return this;
	}

	Runner_Manager* Register_weapon (
		std::string weapon_name,
		int sprite_number,
		vec3 spawn_point,
		vec3 collider_size,
		std::string component_name )
	{
		auto _weapon = Manager::Objekt_Load ( weapon_name, spawn_point );

		_weapon->Add_Component < Box_Collider > ( )
			->Set_Size ( collider_size )
			->Set_Trigger ( true );
		
		_weapon->Add_Component < Sprite > ( )
			->Set ( "weapons", "", "", {2,2}, sprite_number )
			.Set ( true );

		_weapon->Add_Component ( component_name );

		Manager::Objekt_Get ( "Weapons" )->Add_Child ( _weapon );

		return this;
	}
};