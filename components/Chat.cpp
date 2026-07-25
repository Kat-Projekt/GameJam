#pragma once
#include <engine.hpp>
#include <fstream>
#include <random>
#include "file_refes.h"

class Chat : public Behaviour
{
private:
	List < std::string > chat;
	List < std::string > super_chat;
	Text* _text = nullptr;
	Text* super_text = nullptr;
	std::vector < std::string > names;
	std::vector < std::string > random_messages;

	double next_time = 0;

	std::vector<std::string> LoadShit ( const std::string& file )
	{
		std::vector<std::string> _messages;
		std::ifstream input(file);

		if (!input.is_open())
		{
			DEBUG ( 2, "CANNOT OPEN ", file );
			return _messages;
		}

		std::string line;
		while (std::getline(input, line))
		{
			if (!line.empty())
			_messages.push_back(line);
		}

		return _messages;
	}

	int RandomInt ( int min, int max )
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());

		std::uniform_int_distribution<> dist(min, max);
		return dist(gen);
	}

public:
	Chat ( )
	{
		Informations = { "Chat", 1.0, "The Chat interface" };
	}

	void Start ( )
	{
		_text = obj->Add_Component < Text > ( );
		_text->Set ( AOVEL_SANS_ROUNDED_FONT, "" )->Set ( "", Text::ALIGNMENT::LEFT, Text::ALIGNMENT::BOTTOM, false );

		super_text = obj->Add_Component < Text > ( );
		super_text->Set ( AOVEL_SANS_ROUNDED_FONT, "" )
			->Set ( "", Text::ALIGNMENT::LEFT, Text::ALIGNMENT::BOTTOM, false )
			->Set ( vec4{0.5f,0.5f,0.5f,1.0f} );

		names = LoadShit ( CHAT_NAMES_PATH );
		random_messages = LoadShit ( CHAT_MESSAGES_PATH );

		DEBUG ( 4, names );
		DEBUG ( 4, random_messages );

		Post ( names[ RandomInt(0,names.size( )-1 ) ], random_messages[RandomInt(0,random_messages.size( )-1 )]);
	}

	void Update ( )
	{
		if ( Timer::Get_Time ( ) > next_time )
		{
			next_time = Timer::Get_Time ( ) + RandomInt(0,random_messages.size( ) ) / 10;
			
			Post ( names[ RandomInt(0,names.size( )-1 ) ], random_messages[RandomInt(0,random_messages.size( )-1 )]);

			if ( !RandomInt(0,5) )
			{
				Post ( names[ RandomInt(0,names.size( )-1 ) ], random_messages[RandomInt(0,random_messages.size( )-1 )],true);
			}
		}
	}

	void PlayerDeath ( std::string player )
	{
		Post ( "PLAYER_DEATH", player );
	}
	void Post ( std::string user, std::string message, bool super = false ) {
		int padd = 0;

		if ( chat.size ( ) > 4 )
		{
			padd = chat.size ( ) - 4;
		}

		if ( user == "PLAYER_DEATH" )
		{
			super_chat.append ( message + " is dead" );
			chat.append ( "" );
		}
		else if ( super )
		{
			super_chat.append (  user + ": " + message );
			chat.append ( "" );
		}
		else
		{
			super_chat.append ( "" );
			chat.append ( user + ": " + message );
		}

		std::string total_chat = chat[padd];
		for ( size_t i = padd+1; i < chat.size ( ); i++ )
		{
			total_chat += "\n" + chat[i];
		}

		std::string total_super_chat = super_chat[padd];
		for ( size_t i = padd+1; i < super_chat.size ( ); i++ )
		{
			total_super_chat += "\n" + super_chat[i];
		}
		
		_text->Set ( total_chat );
		super_text->Set ( total_super_chat );
	}
};