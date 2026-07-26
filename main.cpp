#include "engine.hpp"
#include "components/file_refes.h"
#include "extensions/synth/synth.hpp"
#include "extensions/synth/resources/manager.hpp"

class Discrete_Camera : public Camera
{
public:
	Discrete_Camera ( )
	{
		Informations = { "Discrete_Camera", 1, "Cadssad" };
	}

	glm::mat4 Projkection
	( ) override {
		float X = ReKat::Graphik::_current_window->Screen_Ratio;
		if ( fb_scale != nullptr ) 
		{ X = *fb_scale; }
		X *= 500.0f * Scale;
		float Y = 500.0f * Scale;
		vec3 pos = obj->Get_Pos ( ); 
		// dicretize position
		pos.x = std::round(pos.x / (X*2)) * (X*2);
		pos.y = std::round(pos.y / (Y*2)) * (Y*2);

		return glm::ortho ( pos.x - X, pos.x + X, pos.y - Y, pos.y + Y, -1.0f, 1.0f );
	}
};

int load ( )
{
	int result = 0;

	Factory::Register_Directory ( "." );

	result += Manager::Defaults_Load ( );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD DEFAULTS" ); }

	ReKat::synth::Start ( );
	Manager::Buffer_Load ( "startup_sound", "Music/Game_Song.wav" );
	Manager::Source_Load ( "startup_source" );
	Source* dbg_source = Manager::Source_Get ( "startup_source" );
	Buffer* dbg_buffer = Manager::Buffer_Get ( "startup_sound" );

	alSourcei ( dbg_source->Get_Source ( ), AL_BUFFER, dbg_buffer->Get_Buffer ( ) );
	alSourcePlay ( dbg_source->Get_Source ( ) );
	// textures
	result += Manager::Make < Texture > ( "logo", "Logo.png" );
	result += Manager::Make < Texture > ( "noise", "noise.png", 1 );
	result += Manager::Make < Texture > ( "coco", "coconut.jpg" );
	result += Manager::Make < Texture > ( RUNNERS_SHEET, RUNNERS_SHEET_PATH ); // runners sprite sheet
	result += Manager::Make < Texture > ( WEAPONS_SHEET, WEAPONS_SHEET_PATH ); // weapons sprite sheet
	result += Manager::Make < Texture > ( OBSTACLES_SHEET, OBSTACLES_SHEET_PATH ); // obstacles sprite sheet
	result += Manager::Make < Texture > ( TILES_SHEET, TILES_SHEET_PATH ); // tiles sprite sheet
	result += Manager::Make < Texture > ( MENU_BLANK_SHEET, MENU_BLANK_SHEET_PATH );
	result += Manager::Make < Texture > ( MENU_SELECTED_SHEET, MENU_SELECTED_SHEET_PATH );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD TEXTURES" ); }
	else { DEBUG ( 4, "SUCCED TO LOAD TEXTURES" ); } 
	// shaders
	result += Manager::Make < Shader > ( NES_CRT_SHADER, NES_CRT_SHADER_VS_PATH, NES_CRT_SHADER_FS_PATH );
	result += Manager::Make < Shader > ( TILEMAP_SHADER, TILEMAP_SHADER_VS_PATH, TILEMAP_SHADER_FS_PATH );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD SHADERS" ); }
	else { DEBUG ( 4, "SUCCED TO LOAD SHADERS" ); } 
	// fonts
	result += Manager::Make < Font > ( AOVEL_SANS_ROUNDED_FONT, AOVEL_SANS_ROUNDED_FONT_PATH, 90 );
	if ( result ) { DEBUG ( 1, "FAILED TO LOAD FONT" ); }
	else { DEBUG ( 4, "SUCCED TO LOAD FONT" ); } 
	// camera
	result += Manager::Camera_Rename ( MAIN_CAMERA, Manager::Objekt_Load ( MAIN_CAMERA )->Add_Component < Discrete_Camera > ( ) );
	

	return result;
}

void __FreamBufferResize (GLFWwindow* window, int width, int heigth ) {
	Manager::Objekt_Get ( "FrameBuffer" )
		->Get_Component < Framebuffer > ( )
		->Set ( width, heigth );
}

int main ( )
{
	ReKat::Graphik::Start ( "Font Test", 800, 600, false, false, true, "Icon.png");
	ReKat::Graphik::_current_window->input._FreamBufferResize = __FreamBufferResize;
	ReKat::phisiks::Start ( 60 );
	ReKat::phisiks::Set_Active ( "MainMenu" );

	load ( );

	DEBUG ( 3, "Resources Loaded" );

	Manager::Objekt_Load ( "MainMenu" )->Add_Component ( "MainMenu" );
	Manager::Objekt_Load ( "Factory" )->Add_Component ( "Scenario" )->_Start();
	Manager::Objekt_Load ( "Credits" )->Add_Component ( "EndScreen" );

	Manager::Objekt_Load ( "FrameBuffer", {0,0,0}, {1333,1000,100} )->Add_Component < Framebuffer > ( )
		->Set ( 800,600 )
	// 	.Set ( NES_CRT_SHADER, "", true )
		.Set ( Manager::Objekt_Get ( "MainMenu" ) );

	Manager::Camera_Get ( MAIN_CAMERA )->fb_scale = &( Manager::Objekt_Get ( "FrameBuffer" )->Get_Component < Framebuffer > ( )->_aspect_ratio );

	Manager::Set_Active_Scene ( "FrameBuffer" );	

	Manager::Get < Shader > ( NES_CRT_SHADER )->setInt ( "screenTexture", 0 );
	Manager::Get < Shader > ( NES_CRT_SHADER )->setInt ( "noiseTexture", 1 );
	Manager::Get < Shader > ( NES_CRT_SHADER )->setFloat ( "time", 0 );

	Timer::Update ( );

	while ( ReKat::Graphik::Is_End ( ) )
	{
		ReKat::Graphik::Clear_Screen ( 0.0f );
		Manager::Update ( );

		Manager::Get < Texture > ( "noise" )->Use ( );
		Manager::Get < Shader > ( NES_CRT_SHADER )->setFloat ( "time", Timer::Get_Time ( ) );
		ReKat::phisiks::Update ( );
		ReKat::Graphik::Update ( );
		// Manager::Objekt_Get ( "Main menu" )->Print_Tree ( );
	}

	Manager::Free ( );
	Manager::Free_Objekts ( );
	
}