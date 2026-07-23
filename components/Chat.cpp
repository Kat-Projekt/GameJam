#include <engine.hpp>

class Chat : public Behaviour
{
private:
	List < std::string > messagges;
	Text* _text = nullptr;
public:
	Chat ( )
	{
		Informations = { "Chat", 1.0, "The Chat interface" };
	}

	void Start ( )
	{
		_text = obj->Add_Component < Text > ( );
		_text->Set ( "Aovel", "" )->Set ( "", Text::ALIGNMENT::LEFT, Text::ALIGNMENT::BOTTOM );
		Post ( "Pipoo", "France" );
	}

	void Update ( )
	{
		Post ( "Pipoo", "France " + std::to_string ( Timer::Get_Time_d ( ) ) );
	}

	void Post ( std::string user, std::string message ) {
		if ( messagges.size ( ) >= 5 )
		{
			messagges.remove ( 0 );
		}

		messagges.append ( user + ": " + message );

		std::string total_messages = messagges[0];
		for ( size_t i = 1; i < messagges.size ( ); i++ )
		{
			total_messages += "\n" + messagges[i];
		}
		
		_text->Set ( total_messages );
	}
};