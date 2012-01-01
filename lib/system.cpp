/* 
** Copyright (C) 2011 Uselabs and/or its subsidiary(-ies).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Uselabs
*/

#include "core.h"
#include "system.h"

void system_hooks()
{
	_HOOKS["system_schema"] = system_schema;
	_HOOKS["system_controllers"] = system_controllers;
	_HOOKS["system_help"] = system_help;
	_PLUGINS_STRINGS["model_system"]["on"] = _TRUE;
	_PLUGINS_STRINGS["system"]["perm"] = "access administration pages, administer site configuration";
	_PLUGINS_STRINGS["system"]["cat"] = "core - required";
	_PLUGINS_STRINGS["system"]["desc"] = "Handles general site configuration for administrators.";
	_PLUGINS_STRINGS["system"]["ver"] = "1.3.1";
}

void system_controllers()
{
	map <string, string> item;

	item["path"] = "install";
	item["title"] = "Install";
	item["type"] = str( ~MENU_VISIBLE_IN_TREE );
	item["callback"] = _MAP_FORM( install );
	poke( controllers, item );

	item["path"] = "robots.txt";
	item["title"] = "Robots";
	item["type"] = str( ~MENU_VISIBLE_IN_TREE );
	item["callback"] = _MAP( system_robots );
	poke( controllers, item );
	
	item["path"] = "admin";
	item["title"] = "Administer";
	item["callback"] = _MAP( watchdog_overview );
	item["access arguments"] = "access administration pages";
	poke( controllers, item );

	item["path"] = "cron";
	item["title"] = "Cron";	
	item["type"] = str( ~MENU_VISIBLE_IN_TREE );
	item["callback"] = _MAP( system_cron );
	poke( controllers, item );

	item["path"] = "admin/models";
	item["title"] = "Models";
	item["callback"] = _MAP( system_models );
	item["access arguments"] = "administer site configuration";
	poke( controllers, item );

	item["path"] = "admin/settings";
	item["title"] = "Settings";
	item["callback"] = _MAP_FORM( system_site_settings );
	item["access arguments"] = "administer site configuration";
	poke( controllers, item );

	item["path"] = "admin/themes";
	item["title"] = "Themes";
	item["callback"] = _MAP( system_themes );
	item["access arguments"] = "administer site configuration";
	poke( controllers, item );

	item["path"] = "admin/build";
	item["title"] = "Build";
	item["callback"] = _MAP( system_build );
	item["access arguments"] = "administer site configuration";
	poke( controllers, item );

	item["path"] = "admin/build/make";
	item["title"] = "Make";
	item["type"] = str( ~MENU_VISIBLE_IN_TREE );
	item["callback"] = _MAP( system_build_make );
	item["access arguments"] = "administer site configuration";
	poke( controllers, item );

	item["path"] = "admin/build/ping";
	item["title"] = "Ping";
	item["type"] = str( ~MENU_VISIBLE_IN_TREE );
	item["callback"] = _MAP( system_build_ping );
	item["access arguments"] = "administer site configuration";
	poke( controllers, item );

	item["path"] = "admin/build/core";
	item["title"] = "Build BinaryTiers Core";
	item["type"] = str( ~MENU_VISIBLE_IN_TREE );
	item["callback"] = _MAP( system_build_core );
	item["parent tab"] = "admin/build/core";
	item["access arguments"] = "administer site configuration";
	poke( controllers, item );

	item["path"] = "connect/mediamanager";
	item["title"] = "Ping";
	item["type"] = str( ~MENU_VISIBLE_IN_TREE );
	item["callback"] = _MAP( system_connect_mediamanager );
	item["access arguments"] = "access administration pages";
	poke( controllers, item );
}

void system_help()
{
	string	path = _GET["q"];

	if( path == "admin/build" )
	{
		help = "<p>Your BinaryTiers Application and BinaryTiers Core might be compiled using this option. Make sure the <b>web server</b> has writting permissions in the project's root and lib directory.</p>";
	}
}

string system_robots()
{
	string out;
	out = "\
User-agent: *\n\
Allow: /\n\
";
	print (out);
	return "";
}

void system_schema()
{
	map <string, map<string, string> > sessions;
	sessions["#spec"]["name"] = "sessions";
	sessions["#spec"]["description"] = "Each record represents a user session, either anonymous or authenticated.";
	sessions["uid"]["description"] = "The {users}.uid corresponding to a session, or 0 for anonymous user.";
	sessions["uid"]["type"] = "int";
	sessions["uid"]["unsigned"] = _TRUE;
	sessions["uid"]["not null"] = _TRUE;
	sessions["sid"]["description"] = "Primary key: A session ID. The value is generated by PHP's Session API.";
	sessions["sid"]["type"] = "varchar";
	sessions["sid"]["length"] = "64";
	sessions["sid"]["not null"] = _TRUE;
	sessions["sid"]["default"] = "";
	sessions["hostname"]["description"] = "The IP address that last used this session ID (sid).";
	sessions["hostname"]["type"] = "varchar";
	sessions["hostname"]["length"] = "128";
	sessions["hostname"]["not null"] = _TRUE;
	sessions["hostname"]["default"] = "";
	sessions["timestamp"]["description"] = "The Unix timestamp when this session last requested a page. Old records are purged by PHP automatically.";
	sessions["timestamp"]["type"] = "int";
	sessions["timestamp"]["not null"] = _TRUE;
	sessions["timestamp"]["default"] = "0";
	sessions["cache"]["description"] = "The time of this user's last post. This is used when the site has specified a minimum_cache_lifetime. See cache_get().";
	sessions["cache"]["type"] = "int";
	sessions["cache"]["not null"] = _TRUE;
	sessions["cache"]["default"] = "0";
	sessions["session"]["description"] = "Not used.";
	sessions["session"]["type"] = "text";
	sessions["session"]["not null"] = _FALSE;
	sessions["session"]["size"] = "big";
	sessions["#primary key"]["sid"] = "sid";
	sessions["#indexes"]["timestamp"] = "timestamp";
	sessions["#indexes"]["uid"] =  "uid";
	schema.push_back( sessions );

	map <string, map<string, string> > variable;
	variable["#spec"]["name"] = "variable";
	variable["#spec"]["description"] = "Named variable/value pairs created by BinaryTiers or any other models or themes. All variables are cached in memory at the start of every request so developers should not be careless about what is stored here.";
	variable["name"]["description"] = "The name of the variable.";
	variable["name"]["type"] = "varchar";
	variable["name"]["length"] = "128";
	variable["name"]["not null"] = _TRUE;
	variable["name"]["default"] = "";
	variable["value"]["description"] = "The value of the variable.";
	variable["value"]["type"] = "text";
	variable["value"]["not null"] = _TRUE;
	variable["value"]["size"] = "big";
	variable["#primary key"]["name"] = "name";
	schema.push_back( variable );

	map <string, map<string, string> > files;
	files["#spec"]["name"] = "files";
	files["#spec"]["description"] = "Stores information for uploaded files.";
	files["fid"]["description"] = "Primary Key: Unique files ID.";
    files["fid"]["type"] = "serial";
    files["fid"]["unsigned"] = _TRUE;
    files["fid"]["not null"] = _TRUE;
    files["uid"]["description"] = "The users.uid of the user who is associated with the file.";
    files["uid"]["type"] = "int";
    files["uid"]["unsigned"] = _TRUE;
    files["uid"]["not null"] = _TRUE;
    files["uid"]["default"] = "0";
    files["filename"]["description"] = "Name of the file.";
    files["filename"]["type"] = "varchar";
    files["filename"]["length"] = "255";
    files["filename"]["not null"] = _TRUE;
    files["filename"]["default"] = "";
	files["filepath"]["description"] = "Path of the file relative to Binary Tiers root.";
	files["filepath"]["type"] = "varchar";
    files["filepath"]["length"] = "255";
    files["filepath"]["not null"] = _TRUE;
    files["filepath"]["default"] = "";      
    files["filemime"]["description"] = "The file MIME type.";
    files["filemime"]["type"] = "varchar";
    files["filemime"]["length"] = "255";
    files["filemime"]["not null"] = _TRUE;
    files["filemime"]["default"] = "";
    files["filesize"]["description"] = "The size of the file in bytes.";
    files["filesize"]["type"] = "int";
    files["filesize"]["unsigned"] = _TRUE;
    files["filesize"]["not null"] = _TRUE;
    files["filesize"]["default"] = "0";
    files["status"]["description"] = "A flag indicating whether file is temporary (0) or permanent (1).";
    files["status"]["type"] = "int";
    files["status"]["not null"] = _TRUE;
    files["status"]["default"] = "0";
	files["width"]["description"] = "With of the image";
    files["width"]["type"] = "int";
    files["width"]["not null"] = _TRUE;
    files["width"]["default"] = "0";  
	files["height"]["description"] = "Height of the image";
    files["height"]["type"] = "int";
    files["height"]["not null"] = _TRUE;
    files["height"]["default"] = "0";
    files["timestamp"]["description"] = "UNIX timestamp for when the file was added.";
    files["timestamp"]["type"] = "int";
    files["timestamp"]["unsigned"] = _TRUE;
    files["timestamp"]["not null"] = _TRUE;
    files["timestamp"]["default"] = "0";
    files["#indexes"]["uid"] = "uid";
	files["#indexes"]["status"] = "status";
	files["#indexes"]["timestamp"] = "timestamp";
    files["#primary key"]["fid"] = "fid";
	schema.push_back( files );
}

void install_model_tables( string model )
{
	if( DB_TYPE == 2 )
	{
		schema.clear();

		for( map <string, void (*)(void)>::iterator i = _HOOKS.begin(), end = _HOOKS.end();  i != end;  i++ )
		{	
			if( i->first == model + "_schema" )
			{
				i->second(); // Call schema function
			}
		}

		for(size_t i=0; i<schema.size(); i++)
		{
			string table_name = schema[i]["#spec"]["name"];

			if ( !db_num_rows( db_querya( "SHOW TABLES LIKE '%s'", table_name.c_str() ) ) )
			{
				string out = db_create_table_sql( schema[i] );
				db_querya( out.c_str() );
			}
		}
	}
}

string install_submit()
{
	if( DB_TYPE == 1 )
	{
		redis_command("MSET users:0 name %s mail %s", "", "");
		
		redis_command("SET users:ids 1");
		redis_command("SADD users 1");
		redis_command("HSET users:name %s 1", _POST["admin_user"].c_str() );
		
		redis_command("HMSET users:1 name %s mail %s created %d pass %s status 1 theme admin",
			_POST["admin_user"].c_str(), _POST["admin_mail"].c_str(), time(), md5( _POST["admin_pass1"] ).c_str() );
		
		redis_command("SET role:ids 2");		
		redis_command("ZADD role 1 %s", "anonymous user");
		redis_command("ZADD role 2 %s", "authenticated user");
		
		redis_command("HSET permission 1 %s", "access content");
		redis_command("HSET permission 2 %s", "access comments, access content, post comments, post comments without approval");

		redis_command("HSET variable theme_default %s", "s:5:\"admin\";" );
	}
	if( DB_TYPE == 2 )
	{
		schema.clear();

		for( map <string, void (*)(void)>::iterator i = _HOOKS.begin(), end = _HOOKS.end();  i != end;  i++ )
		{	
			if( i->first.find( "_schema" ) == i->first.size() - 7 )
			{
				i->second(); // Call schema function
			}
		}

		for(size_t i=0; i<schema.size(); i++)
		{
			string out = db_create_table_sql( schema[i] );

			db_querya( out.c_str() );
		}

		db_querya("INSERT INTO users (name, mail) VALUES('%s', '%s')", string("").c_str(), string("").c_str());
		db_querya("UPDATE users SET uid = uid - uid WHERE name = '%s'", string("").c_str() );
		db_querya("ALTER TABLE users AUTO_INCREMENT=1");

		db_querya("INSERT INTO users (name, mail, created, pass, data, timezone, theme, status) VALUES('%s', '%s', %d, '%s', '', 0, 'admin', 1)",
			_POST["admin_user"].c_str(), _POST["admin_mail"].c_str(), str(time()).c_str(), md5( _POST["admin_pass1"] ).c_str() );

		db_querya("INSERT INTO role (name) VALUES ('%s')", "anonymous user");
		db_querya("INSERT INTO role (name) VALUES ('%s')", "authenticated user");

		db_querya("INSERT INTO permission (rid, perm, tid) VALUES (%d, '%s', %d)", "1", "access content", "0");
		db_querya("INSERT INTO permission (rid, perm, tid) VALUES (%d, '%s', %d)", "2", "access comments, access content, post comments, post comments without approval", "0");

		db_querya("INSERT INTO variable (name, value) VALUES ('%s', '%s')", "theme_default", "s:5:\"admin\";");
	}

	for( map <string, void (*)(void)>::iterator i = _HOOKS.begin(), end = _HOOKS.end();  i != end;  i++ )
	{	
		if( i->first.find( "_install" ) == i->first.size() - 8 )
		{
			i->second(); // Call install function
		}
	}

	return "";
}

bool install_validate()
{
	return true;
}

string install()
{
	map <string, map<string,string> > form;

	if( !load_form_only )
	{
		bool is_installed = false;
		if( isset( variable_get("theme_default") ) )
			is_installed = true;
		
		string filename = tempfile();
		FILE * fp = fopen( filename.c_str(), "wb");
		if( !is_dir( file_directory_temp() ) || !fp ) {
			if( is_installed )
			{
				set_page_message("The Temporary directory <em>"+file_directory_temp()+"</em> is invalid or not writable. You can configure this directory on Settings or you can specify it the variable TEMP_PATH on your settings.h file");

			}
			else
			{
				set_page_message("The Temporary directory <em>"+file_directory_temp()+"</em> is invalid or not writable. Once BinaryTiers is installed you can configure this directory on Settings or you can specify it the variable TEMP_PATH on your settings.h file before proceeding with the installation.");
			}
		}
		else {
			fclose(fp);
		}

		if( is_installed ) return "BinaryTiers has been installed successfully!";
	}

	form["admin_user"]["#type"] = "textfield";
	form["admin_user"]["#title"] = "Administrator Username";
	form["admin_user"]["#weight"] = "1";
	form["admin_user"]["#size"] = "14";
	form["admin_user"]["#value"] = _POST["user"];
	form["admin_user"]["#description"] = "Enter your username for the administrator account.";
	form["admin_user"]["#required"] = str( true );
	
	form["admin_pass"]["#type"] = "password_confirm";
	form["admin_pass"]["#title"] = "Administrator Password";
	form["admin_pass"]["#weight"] = "2";
	form["admin_pass"]["#size"] = "14";
	form["admin_pass"]["#description"] = "Enter the password that accompanies the administrator username.";
	form["admin_pass"]["#required"] = str( true );

	form["admin_mail"]["#type"] = "textfield";
	form["admin_mail"]["#title"] = "Administrator e-mail";
	form["admin_mail"]["#weight"] = "3";
	form["admin_mail"]["#value"] = _POST["user"];
	form["admin_mail"]["#description"] = "Enter your e-mail address for the administrator account.";
	form["admin_mail"]["#required"] = str( true );
		
	form["submit"]["#type"] = "submit";
	form["submit"]["#value"] = "Continue Install";
	form["submit"]["#weight"] = "13";

	return get_form("user", form);
}

string system_cron()
{
	for( map <string, void (*)(void)>::iterator i = _HOOKS.begin(), end = _HOOKS.end();  i != end;  i++ )
	{	
		if( i->first.find( "_cron" ) == i->first.size() - 5 )
		{
			i->second(); // Call cron function
		}
	}
	return "";
}

string watchdog_overview()
{
	return " ";
}

string system_build_core()
{
	string out, objects, filename;
	string dir = "lib/";

	int handle = diropen( dir );
	while ( handle = dirread( handle, filename ) )
	{
		if( match_right(filename, ".cpp") )
		{
			size_t pos = filename.find_last_of('.');
			string name = filename.substr(0, pos);

			if( name != "core" )
			{
			objects += " " + name + ".o";

			out += name + ".o : ../lib/" + filename + "\n";
			out += "\t$(CXX) $(CFLAGS) $(CCFLAGS) $(INCLUDES) -c ../lib/" + filename + "\n";
			}
		}
	}
	dirclose( handle );

	string ldflags = variable_get("build_ldflags", "-lfcgi -lgd -lz -lpcre -lexpat -lhiredis");

	out = "\
CC = gcc\n\
CXX = g++\n\
CFLAGS = -Os\n\
CCFLAGS = \n\
INCLUDES = -I/usr/include/mysql -I../lib/pcre\n\
LDFLAGS = -L/usr/lib/mysql -lmysqlclient "+ldflags+"\n\
OBJECTS = core.o parsedate.o json.o md5c.o sha1.o" + objects + "\n\
\n\
libbt.a : $(OBJECTS)\n\
	ar ru libbt.a $(OBJECTS)\n\
	mv ./libbt.a ../lib/libbt.a\n\
	sleep 5\n\
	rm -f result.txt\n\
\n\
core.o : ../lib/core.cpp ../lib/core.h\n\
	$(CXX) $(CFLAGS) $(CCFLAGS) $(INCLUDES) -c ../lib/core.cpp\n\
\n\
" + out + "\
parsedate.o : ../lib/gnu/parsedate.cpp\n\
	$(CXX) $(CFLAGS) $(CCFLAGS) $(INCLUDES) -c ../lib/gnu/parsedate.cpp\n\
json.o :  ../lib/json/json.c\n\
	$(CC) $(CFLAGS) $(INCLUDES) -c ../lib/json/json.c\n\
md5c.o :  ../lib/md5/md5c.c\n\
	$(CC) $(CFLAGS) $(INCLUDES) -c ../lib/md5/md5c.c\n\
sha1.o :  ../lib/md5/sha1.c\n\
	$(CC) $(CFLAGS) $(INCLUDES) -c ../lib/md5/sha1.c\n\
\n\
clean:\n\
	rm *.o\n";

	string make_file = file_directory_temp() + "/Makefile";

	FILE * fp;
	fp = fopen(make_file.c_str(), "wb");
	fwrite( out.c_str(), 1, out.length(), fp );
	fclose( fp );

	add_js("etc/build.js");

	string button = "";
	
	return "<script type=\"text/javascript\">\n\
		   var pos1=0;\n\
		   var ping1=1;\n\
		   function buildping() {\n\
		   checkstatus(\""+url("admin/build/ping")+"?pos=\"+pos1);\n\
		   }\n\
		   checkstatus(\""+url("admin/build/make")+"\");\n\
		   setTimeout('buildping();', 2000);\n\
		   </script>\n\
		   <div style=\"height:36px;\">\n\
		   <div id=\"progress1\" class=\"progress\"><div class=\"bar\"><div class=\"filled\"></div></div><div class=\"percentage\"></div><div class=\"message\">Compiling...</div></div>\n\
		   </div>\n\
		   <code id=\"msg\" style=\"font-size:12px;\"></code>\n\
		   ";
}

string system_build()
{
	string out, objects, filename;
	string dir = "src/";

	int handle = diropen( dir );
	while ( handle = dirread( handle, filename ) )
	{
		if( match_right(filename, ".cpp") )
		{
			size_t pos = filename.find_last_of('.');
			string name = filename.substr(0, pos);

			if( name != "main" )
			{
			objects += " " + name + ".o";

			out += name + ".o : main.o ../src/" + filename + "\n";
			out += "\t$(CXX) $(CFLAGS) $(CCFLAGS) $(INCLUDES) -c ../src/" + filename + "\n";
			}
		}
	}
	dirclose( handle );

	vector <string> themes;
	dir = "themes/";
	handle = diropen( dir );
	while ( handle = dirread( handle, filename ) )
	{
		if( filename != "." && filename != ".." )
		{
			themes.push_back(filename);
		}
	}
	dirclose( handle );

	for( size_t i=0; i<themes.size(); i++)
	{
		string curdir = dir + themes[i] + "/";
		handle = diropen( curdir );
		while ( handle = dirread( handle, filename ) )
		{
			if( match_right(filename, ".cpp") )
			{
				size_t pos = filename.find_last_of('.');
				string name = filename.substr(0, pos);

				objects += " " + name + ".o";

				out += name + ".o : main.o ../" + curdir + filename + "\n";
				out += "\t$(CXX) $(CFLAGS) $(CCFLAGS) $(INCLUDES) -c ../" + curdir + filename + "\n";
			}
		}
		dirclose( handle );
	}

	string ldflags = variable_get("build_ldflags", "-lfcgi -lgd -lz -lpcre -lexpat");

	out = "\
CC = gcc\n\
CXX = g++\n\
CFLAGS = -Os\n\
CCFLAGS = \n\
INCLUDES = -I/usr/include/mysql -I../src/pcre\n\
LDFLAGS = -L/usr/lib/mysql -lmysqlclient -L../lib -lbt "+ldflags+"\n\
OBJECTS = main.o" + objects + "\n\
\n\
bt.build.bin : $(OBJECTS)\n\
	$(CXX) $(CFLAGS) $(CCFLAGS) $(OBJECTS) $(LDFLAGS) -o ./bt.build.bin\n\
	mv ./bt.build.bin ../bt.bin\n\
	sleep 5\n\
	rm -f result.txt\n\
\n\
main.o : ../src/main.cpp ../src/settings.h\n\
	$(CXX) $(CFLAGS) $(CCFLAGS) $(INCLUDES) -c ../src/main.cpp\n\
\n\
" + out + "\
\n\
clean:\n\
	rm *.o\n";

	string make_file = file_directory_temp() + "/Makefile";

	FILE * fp;
	fp = fopen(make_file.c_str(), "wb");
	fwrite( out.c_str(), 1, out.length(), fp );
	fclose( fp );

	add_js("etc/build.js");

	string button = "";

	if( file_exists("lib/core.cpp") )
		button = "<a href=\""+url("admin/build/core")+"\">Build Core</a>";

	return button + "<script type=\"text/javascript\">\n\
		   var pos1=0;\n\
		   var ping1=1;\n\
		   function buildping() {\n\
		   checkstatus(\""+url("admin/build/ping")+"?pos=\"+pos1);\n\
		   }\n\
		   checkstatus(\""+url("admin/build/make")+"\");\n\
		   setTimeout('buildping();', 2000);\n\
		   </script>\n\
		   <div style=\"height:36px;\">\n\
		   <div id=\"progress1\" class=\"progress\"><div class=\"bar\"><div class=\"filled\"></div></div><div class=\"percentage\"></div><div class=\"message\">Compiling...</div></div>\n\
		   </div>\n\
		   <code id=\"msg\" style=\"font-size:12px;\"></code>\n\
		   ";
}
string system_build_ping()
{
	string	out, html;

	string result_file = file_directory_temp() + "/result.txt";
		
	if( file_exists( result_file ) )
	{
		out = file_get_contents( result_file );
		if( out.find("***") != string::npos )
		{
			unlink( result_file.c_str() );
		}

		size_t pos = intval( _GET["pos"] );
		size_t len = out.length();
		out = out.substr( pos, len-pos );
		out = str_replace("\n","<br>", out);

		html += "document.getElementById(\"msg\").innerHTML += unescape(\"" + rawurlencode( out ) + "\");\n";
		html += "pos1+="+str(len-pos)+";\n";
	}
	else
	{
		string msg;

		string end_file = file_directory_temp() + "/end.txt";
		
		if( file_get_contents( end_file ) == "0" )
			msg = "<div style=\"margin-top: 0.2em; color: green\">The build process has finished successfully!</div><div style=\"margin-top: 0.2em;\">See details below:</div>";
		else
			msg = "<div style=\"margin-top: 0.2em; color: red\">The build process has finished with errors.</div><div style=\"margin-top: 0.2em;\">See details below:</div>";

		html += "document.getElementById(\"progress1\").innerHTML = unescape(\"" + rawurlencode( msg ) + "\");ping1=0;";
	}

	print (html);

	return "";
}

string system_build_make()
{
	chdir( file_directory_temp().c_str() );

	if( !file_exists("result.txt") )
	{
		int res = system("make &>result.txt");
		FILE * fp = fopen("end.txt", "wb");
		fprintf(fp, "%i", res);
		fclose( fp );
	}	
	return "";
}

string system_models()
{
	string	out;
	size_t i;
	
	map <string, map <string, string> > form;

	if( isset(_POST["op"]) )
	{
		map <string, string>::iterator curr, end;
		string models = "";
		for( curr = _POST.begin(), end = _POST.end();  curr != end;  curr++ )
		{
			if( curr->first.find("sel-") != curr->first.npos )
			{
				models += curr->second+",";
				install_model_tables( curr->second );
			}
		}
		variable_set("bt_models", models);
		redirect( url("admin/models") );
		return "";
	}

	help = "<p>Models in BinaryTiers encapsulate the functionalities and group the data objects of your web application allowing an independent development, testing and manteinance. Enable models by selecting the Enabled checkboxes below and clicking the Save configuration button.</p>";
	
	form["models"]["#type"] = "table";
	form["models"]["#header"] = "Enabled,Name,Version,Description";
	form["models"]["#weight"] = "1";

	for( i = 0; i < _MODELS.size(); i++ )
	{
		form["stat_"+str(i)]["#type"] = "checkbox";
		form["stat_"+str(i)]["#name"] = "sel-"+_MODELS[i];
		form["stat_"+str(i)]["#value"] = _MODELS[i];
		form["stat_"+str(i)]["#table"] = "models";

		if( _PLUGINS_STRINGS[ _MODELS[i] ]["cat"] == "core - required" )
		{
			form["stat_"+str(i)]["#disabled"] = _TRUE;
			form["stat_"+str(i)]["#checked"] = _TRUE;
		}

		if( _PLUGINS_STRINGS[ "model_"+_MODELS[i] ]["on"] == _TRUE )
			form["stat_"+str(i)]["#checked"] = _TRUE;

		form["name_"+str(i)]["#type"] = "markup";
		form["name_"+str(i)]["#value"] = _MODELS[i];
		form["name_"+str(i)]["#table"] = "models";

		form["ver_"+str(i)]["#type"] = "markup";
		form["ver_"+str(i)]["#value"] = _PLUGINS_STRINGS[ _MODELS[i] ]["ver"];
		form["ver_"+str(i)]["#table"] = "models";

		form["desc_"+str(i)]["#type"] = "markup";
		form["desc_"+str(i)]["#value"] = _PLUGINS_STRINGS[ _MODELS[i] ]["desc"];
		form["desc_"+str(i)]["#table"] = "models";

		form["models"]["#row"+str(i)] = "stat_"+str(i)+",name_"+str(i)+",ver_"+str(i)+",desc_"+str(i);
	}

	form["submit"]["#type"] = "submit";
	form["submit"]["#value"] = "Save configuration";
	form["submit"]["#weight"] = "2";

	out += get_form("models", form);

	return out;
}

string system_themes()
{
	string	out;
	size_t i;

	map <string, map<string,string> > form;

	help = "<p>Select which themes are available to your users and specify the default theme. To configure site-wide display settings, click the \"configure\" task above. Alternatively, to override these settings in a specific theme, click the \"configure\" link for that theme. Note that different themes may have different regions available for displaying content; for consistency in presentation, you may wish to enable only one theme.</p>";

	if( isset( _POST["op"] ) )
	{
		map <string, string>::iterator curr, end;
		string themes = "";

		for( curr = _POST.begin(), end = _POST.end();  curr != end;  curr++ )
		{
			if( curr->first.find("sel-") != curr->first.npos )
			{
				themes += curr->second+",";
			}
		}
		variable_set("bt_themes", themes);
		variable_set("theme_default", _POST["theme_default"]);
		redirect( url("admin/themes") );
		return "";
	}
	if( isset( _POST["reset"] ) )
	{

	}

	form["themes"]["#type"] = "table";
	form["themes"]["#header"] = "Screenshot,Name,Enabled,Default";
	form["themes"]["#weight"] = "1";
	
	string default_theme = variable_get("theme_default", "admin");
	
	for( i = 0; i < _THEMES.size(); i++ )
	{
		form["screen_"+str(i)]["#type"] = "markup";
		form["screen_"+str(i)]["#value"] = "<img style=\"border: 1px solid #ccc;\" src=\""+string(BASE_URL)+"/themes/"+_THEMES[i]+"/screenshot.png\">";
		form["screen_"+str(i)]["#table"] = "themes";

		form["name_"+str(i)]["#type"] = "markup";
		form["name_"+str(i)]["#value"] = _THEMES[i];
		form["name_"+str(i)]["#table"] = "themes";

		form["stat_"+str(i)]["#type"] = "checkbox";
		form["stat_"+str(i)]["#attributes"] = "align=\"center\"";
		form["stat_"+str(i)]["#name"] = "sel-"+_THEMES[i];
		form["stat_"+str(i)]["#value"] = _THEMES[i];
		form["stat_"+str(i)]["#table"] = "themes";
		if( _PLUGINS_STRINGS[ "theme_"+_THEMES[i] ]["on"] == _TRUE ) {
			form["stat_"+str(i)]["#checked"] = _TRUE;
		}
		
		form["def_"+str(i)]["#type"] = "radio";
		form["def_"+str(i)]["#attributes"] = "align=\"center\"";
		form["def_"+str(i)]["#name"] = "theme_default";
		form["def_"+str(i)]["#value"] = _THEMES[i];
		form["def_"+str(i)]["#table"] = "themes";
		if(_THEMES[i]==default_theme) {
			form["def_"+str(i)]["#checked"] = _TRUE;
		}

		form["themes"]["#row"+str(i)] = "screen_"+str(i)+",name_"+str(i)+",stat_"+str(i)+",def_"+str(i);
	}

	form["submit"]["#type"] = "submit";
	form["submit"]["#value"] = "Save configuration";
	form["submit"]["#weight"] = "2";

	form["reset"]["#type"] = "submit";
	form["reset"]["#value"] = "Reset to defaults";
	form["reset"]["#weight"] = "3";
	
	out += get_form("themes", form);

	return out;
}

string system_site_settings()
{
	string	out;
	map <string, map<string,string> > form;

	if( !load_form_only )
	{
		string filename = tempfile();
		FILE * fp = fopen( filename.c_str(), "wb");
		if( !fp ) {
			set_page_message( str_replace("%2", file_directory_temp(), "Your TEMP_PATH is not writable. Make sure the TEMP_PATH [%2] is valid and has write permissions."), "error" );
		}
		else {
			fclose(fp);
		}
	}

	form["general"]["#type"] = "fieldset";
	form["general"]["#weight"] = "1";
    form["general"]["#title"] = "General settings";
    form["general"]["#collapsible"] = _TRUE;
    form["general"]["#collapsed"] = _FALSE;

		form["site_name"]["#type"] = "textfield";
		form["site_name"]["#fieldset"] = "general";
		form["site_name"]["#weight"] = "1";
		form["site_name"]["#title"] = "Name";
		form["site_name"]["#value"] = variable_get("site_name", "BinaryTiers");
		form["site_name"]["#description"] = "The name of this web site.";
		form["site_name"]["#required"] = _TRUE;

	form["site_mail"]["#type"] = "textfield";
	form["site_mail"]["#fieldset"] = "general";
	form["site_mail"]["#weight"] = "2";
	form["site_mail"]["#title"] = "E-mail address";
	form["site_mail"]["#value"] = variable_get("site_mail", SENDMAIL_FROM);
	form["site_mail"]["#description"] = "A valid e-mail address for this website, used by the auto-mailer during registration, new password requests, notifications, etc.";

		form["site_slogan"]["#type"] = "textfield";
		form["site_slogan"]["#fieldset"] = "general";
		form["site_slogan"]["#weight"] = "3";
		form["site_slogan"]["#title"] = "Slogan";
		form["site_slogan"]["#value"] = variable_get("site_slogan", "");
		form["site_slogan"]["#description"] = "The slogan of this website. Some themes display a slogan when available.";

		form["site_mission"]["#type"] = "textarea";
		form["site_mission"]["#fieldset"] = "general";
		form["site_mission"]["#weight"] = "4";
		form["site_mission"]["#title"] = "Mission";
		form["site_mission"]["#value"] = variable_get("site_mission", "");
		form["site_mission"]["#description"] = "Your site's mission statement or focus.";


		form["files"]["#type"] = "fieldset";
		form["files"]["#weight"] = "2";
		form["files"]["#title"] = "File system settings";
		form["files"]["#collapsible"] = _TRUE;
		form["files"]["#collapsed"] = _FALSE;

		form["file_directory_path"]["#type"] = "textfield";
		form["file_directory_path"]["#fieldset"] = "files";
		form["file_directory_path"]["#title"] = "File system path";
		form["file_directory_path"]["#size"] = "60";
		form["file_directory_path"]["#value"] = file_directory_path();
		form["file_directory_path"]["#description"] = "The File system path where the uploaded files will be stored. This directory has to exist and be writable by BinaryTiers.";

		form["file_directory_temp"]["#type"] = "textfield";
		form["file_directory_temp"]["#fieldset"] = "files";
		form["file_directory_temp"]["#title"] = "Temporary directory";
		form["file_directory_temp"]["#size"] = "60";
		form["file_directory_temp"]["#value"] = file_directory_temp();
		form["file_directory_temp"]["#description"] = "The Temporary directory where the temporary objects will be allocated when uploading files, compiling code or other tasks. This directory has to exist and be writable by BinaryTiers.";

	form["recaptcha"]["#type"] = "fieldset";
	form["recaptcha"]["#weight"] = "3";
    form["recaptcha"]["#title"] = "Recaptcha settings";
    form["recaptcha"]["#collapsible"] = _TRUE;
    form["recaptcha"]["#collapsed"] = _FALSE;

		form["public"]["#type"] = "textfield";
		form["public"]["#fieldset"] = "recaptcha";
		form["public"]["#weight"] = "1";
		form["public"]["#title"] = "Public Key";
		form["public"]["#value"] = variable_get("recaptcha_public", "");
		form["public"]["#description"] = "Use this in the JavaScript code that is served to your users";

		form["private"]["#type"] = "textfield";
		form["private"]["#fieldset"] = "recaptcha";
		form["private"]["#weight"] = "2";
		form["private"]["#title"] = "Private Key";
		form["private"]["#value"] = variable_get("recaptcha_private", "");
		form["private"]["#description"] = "Use this when communicating between your server and our server. Be sure to keep it a secret.";

	form["build"]["#type"] = "fieldset";
	form["build"]["#weight"] = "4";
    form["build"]["#title"] = "Building options";
    form["build"]["#collapsible"] = _TRUE;
    form["build"]["#collapsed"] = _FALSE;

		form["build_ldflags"]["#type"] = "textfield";
		form["build_ldflags"]["#fieldset"] = "build";
		form["build_ldflags"]["#weight"] = "1";
		form["build_ldflags"]["#title"] = "LDFlags";
		form["build_ldflags"]["#value"] = variable_get("build_ldflags", "-lfcgi -lgd -lz -lpcre -lexpat");
		form["build_ldflags"]["#description"] = "Extra library dependences";

	form["mediamanager"]["#type"] = "fieldset";
	form["mediamanager"]["#weight"] = "5";
    form["mediamanager"]["#title"] = "Media Manager settings";
    form["mediamanager"]["#collapsible"] = _TRUE;
    form["mediamanager"]["#collapsed"] = _FALSE;

		form["mm_email"]["#type"] = "textfield";
		form["mm_email"]["#fieldset"] = "mediamanager";
		form["mm_email"]["#weight"] = "1";
		form["mm_email"]["#title"] = "Console Email";
		form["mm_email"]["#value"] = variable_get("mediamanager_email", "");
		form["mm_email"]["#description"] = "Email account you use to have access to the Media Manager console";

		form["mm_pass"]["#type"] = "textfield";
		form["mm_pass"]["#fieldset"] = "mediamanager";
		form["mm_pass"]["#weight"] = "2";
		form["mm_pass"]["#title"] = "Console Password";
		form["mm_pass"]["#value"] = variable_get("mediamanager_pass", "");
		form["mm_pass"]["#description"] = "Password to access the Media Manager console. Be sure to keep it a secret.";

	form["s3files"]["#type"] = "fieldset";
	form["s3files"]["#weight"] = "5";
    form["s3files"]["#title"] = "S3 Settings";
    form["s3files"]["#collapsible"] = _TRUE;
    form["s3files"]["#collapsed"] = _FALSE;

		form["s3files_url"]["#type"] = "textfield";
		form["s3files_url"]["#fieldset"] = "s3files";
		form["s3files_url"]["#weight"] = "1";
		form["s3files_url"]["#title"] = "URL Prefix";
		form["s3files_url"]["#value"] = variable_get("s3files_url", "");
		form["s3files_url"]["#description"] = "URL prefix for all S3 files.";

		form["s3files_bucket"]["#type"] = "textfield";
		form["s3files_bucket"]["#fieldset"] = "s3files";
		form["s3files_bucket"]["#weight"] = "2";
		form["s3files_bucket"]["#title"] = "Bucket";
		form["s3files_bucket"]["#value"] = variable_get("s3files_bucket", "");
		form["s3files_bucket"]["#description"] = "Bucket for all S3 files.";

	form["submit"]["#type"] = "submit";
	form["submit"]["#value"] = "Save configuration";
	form["submit"]["#weight"] = "12";

	form["reset"]["#type"] = "submit";
	form["reset"]["#value"] = "Reset to defaults";
	form["reset"]["#weight"] = "13";

	out += get_form("system_site_settings", form);

	return out;
}

string system_site_settings_submit()
{
	variable_set("site_name", _POST["site_name"]);
	variable_set("site_mail", _POST["site_mail"]);
	variable_set("site_slogan", _POST["site_slogan"]);
	variable_set("site_mission", _POST["site_mission"]);

	variable_set("file_directory_path", _POST["file_directory_path"]);
	variable_set("file_directory_temp", _POST["file_directory_temp"]);

	variable_set("build_ldflags", _POST["build_ldflags"]);

	variable_set("recaptcha_public", _POST["public"]);
	variable_set("recaptcha_private", _POST["private"]);

	variable_set("mediamanager_email", _POST["mm_email"]);
	variable_set("mediamanager_pass", _POST["mm_pass"]);

	if( _POST["s3files_url"].length() > 0)
	{
		if( _POST["s3files_url"].substr(_POST["s3files_url"].length()-1) != "/" )
		{
			_POST["s3files_url"] += "/";
		}
	}

	if( _POST["s3files_bucket"].length() > 0)
	{
		if( _POST["s3files_bucket"].substr(_POST["s3files_bucket"].length()-1) != "/" )
		{
			_POST["s3files_bucket"] += "/";
		}
	}

	variable_set("s3files_url", _POST["s3files_url"]);
	variable_set("s3files_bucket", _POST["s3files_bucket"]);

	redirect( url("admin/settings") );

	return "";
}

bool system_site_settings_validate()
{
	rtrim( _POST["file_directory_path"], "\\/" );
	rtrim( _POST["file_directory_temp"], "\\/" );

	if( !is_dir( _POST["file_directory_path"] ) )
	{
		form_set_error("file_directory_path", "The File system path <em>"+_POST["file_directory_path"]+"</em> is not a valid directory.");
		return false;
	}
	else
	{
		string filename = tempfile( _POST["file_directory_path"].c_str() );
		FILE * fp = fopen( filename.c_str(), "wb");
		if( !fp ) {
			form_set_error("file_directory_path", "The File system path <em>"+_POST["file_directory_path"]+"</em> is not writable.");
			return false;
		}
		else {
			fclose(fp);
		}
	}

	if( !is_dir( _POST["file_directory_temp"] ) )
	{
		form_set_error("file_directory_temp", "The Temporary directory <em>"+_POST["file_directory_temp"]+"</em> is not a valid directory.");
		return false;
	}
	else
	{
		string filename = tempfile( _POST["file_directory_temp"].c_str() );
		FILE * fp = fopen( filename.c_str(), "wb");
		if( !fp ) {
			form_set_error("file_directory_temp", "The Temporary directory <em>"+_POST["file_directory_temp"]+"</em> is not writable.");
			return false;
		}
		else {
			fclose(fp);
		}
	}

	return true;
}

string system_connect_mediamanager()
{
	string out;

	out = variable_get("mediamanager_email", "") +","+ variable_get("mediamanager_pass", "");

	print( out );

	return "";
}