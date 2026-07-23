#define DIAGNOSTIC
#include <engine.hpp>

void ContinueGame ( )
{
	DEBUG ( 1,"END" );
}

class MainMenu : public Behaviour
{
private:
	Sprite* sprite = nullptr;
public:
	MainMenu ( )
	{
		Informations = { "MainMenu", 1.0, "The Chat interface" };
	}

	void Start ( )
	{
		std::shared_ptr < Objekt > title = std::make_shared < Objekt > (
			"MainMenuTitle",
			vec3{0,300,0}
		);
		title->Add_Component < Text > ( )
			->Set ( "Killing Time" )
			->Set ( "Aovel", "" );
		
		std::shared_ptr < Objekt > continue_button = std::make_shared < Objekt > (
			"MainMenuContinue",
			vec3{0,-100,0},
			vec3{300,100,0}
		);
		continue_button->Add_Component < Button > ( )
			->OnClick ( ContinueGame )
			->OnHover ( this, &MainMenu::Hover )
			->NotHover ( this, &MainMenu::NotHover );
		
		sprite = continue_button->Add_Component < Sprite > ( );
		sprite->Set ( "logo", "", "", {2,1} ).Set ( true );

		obj->Add_Child ( title );
		obj->Add_Child ( continue_button );

		DEBUG ( 5, obj );
		DEBUG ( 5, obj->Get_Child ( "MainMenuTitle" )->Get_Transform ( ) );
		DEBUG ( 5, obj->Get_Child ( "MainMenuContinue" )->Get_Transform ( )  );	
	}

	void Update ( )
	{
		DEBUG ( 5, obj->Get_Child ( "MainMenuTitle" )->Get_Transform ( ) );
		DEBUG ( 5, obj->Get_Child ( "MainMenuContinue" )->Get_Transform ( )  );
		DEBUG ( 3, obj->Get_Child ( "MainMenuTitle" )->Get_Model_Mat ( ) );
		DEBUG ( 4, ReKat::Graphik::_current_window->input.mouse_pos );
	}

	void Hover ( )
	{
		sprite->Set ( 1 );
	}

	void NotHover ( )
	{
		sprite->Set ( 0 );
	}
};