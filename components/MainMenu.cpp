#define DIAGNOSTIC
#include <engine.hpp>
#include "file_refes.h"
void ContinueGame ( )
{
	Manager::Objekt_Get ( "FrameBuffer" )
		->Get_Component < Framebuffer > ( )
		->Set ( Manager::Objekt_Get ( "Factory" ) );
	ReKat::phisiks::Set_Active ( "Factory" );
}
class MainMenu : public Behaviour
{
private:
	Sprite* button_visual_sprite = nullptr;
public:
	MainMenu ( )
	{
		Informations = { "MainMenu", 1.0, "The Chat interface" };
	}
	void Start ( )
	{
		// oggetto interattivo: rileva mouse/click sull'area del bottone reale
		std::shared_ptr < Objekt > continue_button = std::make_shared < Objekt > (
			"MainMenuContinue",
			vec3{380,-280,0},
			vec3{300,100,0}
		);
		continue_button->Add_Component < Button > ( )
			->OnClick ( ContinueGame )
			->OnHover ( this, &MainMenu::Hover )
			->NotHover ( this, &MainMenu::NotHover );

		// oggetto separato: sprite a schermo intero, mostra blank/selected
		std::shared_ptr < Objekt > continue_visual = std::make_shared < Objekt > (
			"MainMenuContinueVisual",
			vec3{0,0,0},
			vec3{1333,650,0}
		);
		button_visual_sprite = continue_visual->Add_Component < Sprite > ( );
		button_visual_sprite->Set ( MENU_BLANK_SHEET, "", "", {1,1} )->Set ( true );

		obj->Add_Child ( continue_button );
		obj->Add_Child ( continue_visual );
	}
	void Hover ( )
	{
		button_visual_sprite->Set ( MENU_SELECTED_SHEET );
	}
	void NotHover ( )
	{
		button_visual_sprite->Set ( MENU_BLANK_SHEET );
	}
};