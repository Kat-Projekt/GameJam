#define DIAGNOSTIC
#include "engine.hpp"

int load ( )
{
	int result = 0;

	Factory::Register_Directory ( "." );

	result += Manager::Defaults_Load ( );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD DEFAULTS" ); }

	// textures
	result += Manager::Make < Texture > ( "logo", "Logo.png" );
	result += Manager::Make < Texture > ( "noise", "noise.png", 1 );
	result += Manager::Make < Texture > ( "coco", "coconut.jpg" );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD TEXTURES" ); }
	else { DEBUG ( 4, "SUCCED TO LOAD TEXTURES" ); } 
	// shaders
	result += Manager::Make < Shader > ( "crt_effect", "Shaders/crt_effect.vs", "Shaders/crt_effect.fs" );
	result += Manager::Make < Shader > ( "crt_effect_nes", "Shaders/crt_effect.vs", "Shaders/crt_effect_nes.fs" );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD SHADERS" ); }

	// fonts
	result += Manager::Make < Font > ( "Aovel", "AovelSansRounded.ttf", 90 );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD FONT" ); }

	Manager::Register("Aovel" , Manager::Get< Font > ("Aovel")->Get_Texture());
	return result;
}

void __FreamBufferResize (GLFWwindow* window, int width, int heigth ) {
	Manager::Objekt_Get ( "FrameBuffer" )
		->Get_Component < Framebuffer > ( )
		->Set ( width, heigth );
}

int main ( )
{
	ReKat::Graphik::Start ( "Font Test", 800, 600, false, false, true );
	ReKat::Graphik::_current_window->input._FreamBufferResize = __FreamBufferResize;
	ReKat::phisiks::Start ( 60 );
	ReKat::phisiks::Set_Active ( "Main menu" );

	load ( );

	DEBUG ( 3, "Resources Loaded" );

	Manager::Objekt_Load ( "Main menu" )->Add_Component ( "Scenario" );
	//Manager::Objekt_Load ( "Main menu", vec3{0,0,0}, vec3{550,500,10} )->Add_Component <Sprite> ( )
	//-> Set ( "Aovel" ).Set(true);
	

	Manager::Objekt_Load ( "FrameBuffer", {0,0,0}, {1333,1000,100} )->Add_Component < Framebuffer > ( )
		->Set ( 800,600 )
		.Set ( "crt_effect_nes", "", true )
		.Set ( Manager::Objekt_Get ( "Main menu" ) );

	// Manager::Objekt_Load ( "FrameBuffer", {0,0,0}, {1333,1000,100} )->Add_Component < Sprite > ( )->Set ( "logo", "crt_effect" ).Set ( true );

	Manager::Set_Active_Scene ( "FrameBuffer" );

	Manager::Get < Shader > ( "crt_effect_nes" )->setInt ( "screenTexture", 0 );
	Manager::Get < Shader > ( "crt_effect_nes" )->setInt ( "noiseTexture", 1 );
	Manager::Get < Shader > ( "crt_effect_nes" )->setFloat ( "time", 0 );

	Timer::Update ( );
	while ( ReKat::Graphik::Is_End ( ) )
	{
		ReKat::Graphik::Clear_Screen ( 0.0f );
		Manager::Update ( );

		Manager::Get < Texture > ( "noise" )->Use ( );
		Manager::Get < Shader > ( "crt_effect_nes" )->setFloat ( "time", Timer::Get_Time ( ) );
		ReKat::phisiks::Update ( );
		ReKat::Graphik::Update ( );
		Manager::Objekt_Get ( "Main menu" )->Print_Tree ( );
	}

	Manager::Free ( );
	Manager::Free_Objekts ( );
}