#include <engine.hpp>

class Camera_Manager : public Behaviour
{
private:
	List < Camera* > _cameras;
	Camera* _current_camera;

	bool IsInCamera ( vec3 pos )
	{
		vec2 bounds = vec2 {500,500};
		return false;
	}
	Camera* FindCamera ( vec3 pos ) { return nullptr; }
public:
	Camera_Manager ( )
	{
		Informations = { "Camera_Manager", 1.0, "This is the Clubs information" };
	}

	void Start ( )
	{
		_cameras = obj->Get_Component_Recursive < Camera > ( );
	}

	void Update ( ) {
		vec3 pos = Manager::Objekt_Get ( "Player" )->Get_Pos ( );
		if ( IsInCamera ( pos ) )
		{ return; }

		// find new camera
		Camera* new_camera = FindCamera ( pos );

		if ( new_camera == nullptr ){
			DEBUG ( 3, "Hit Camera Border" );
		} else {
			Manager::Camera_Rename ( "Fabbrica", new_camera );
		}
	}
};