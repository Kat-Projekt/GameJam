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
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD TEXTURES" ); }
	else { DEBUG ( 4, "SUCCED TO LOAD TEXTURES" ); } 
	// shaders
	result += Manager::Make < Shader > ( "crt_effect", "Shaders/crt_effect.vs", "Shaders/crt_effect.fs" );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD SHADERS" ); }

	// fonts
	result += Manager::Make < Font > ( "Aovel", "AovelSansRounded.ttf", 90 );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD FONT" ); }

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

	load ( );

	DEBUG ( 3, "Resources Loaded" );

	Manager::Objekt_Load ( "Main menu", vec3{660,-475,0}, vec3{50,50,10} )->Add_Component ( "Scenario" );

	Manager::Objekt_Load ( "FrameBuffer", {0,0,0}, {1333,1000,100} )->Add_Component < Framebuffer > ( )
		->Set ( 800,600 )
		.Set ( "crt_effect", "", true )
		.Set ( Manager::Objekt_Get ( "Main menu" ) );

	// Manager::Objekt_Load ( "FrameBuffer", {0,0,0}, {1333,1000,100} )->Add_Component < Sprite > ( )->Set ( "logo", "crt_effect" ).Set ( true );

	Manager::Set_Active_Scene ( "FrameBuffer" );

	Manager::Get < Shader > ( "crt_effect" )->setInt ( "screenTexture", 0 );
	Manager::Get < Shader > ( "crt_effect" )->setInt ( "noiseTexture", 1 );
	Manager::Get < Shader > ( "crt_effect" )->setFloat ( "time", 0 );

	while ( ReKat::Graphik::Is_End ( ) )
	{
		ReKat::Graphik::Clear_Screen ( 0.0f );
		Manager::Update ( );

		Manager::Get < Texture > ( "noise" )->Use ( );
		Manager::Get < Shader > ( "crt_effect" )->setFloat ( "time", Timer::Get_Time ( ) );
		ReKat::phisiks::Update ( );
		ReKat::Graphik::Update ( );
	}

	Manager::Free ( );
	Manager::Free_Objekts ( );
}