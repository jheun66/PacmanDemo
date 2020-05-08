#include "stdafx.h"
#include "JsonDemo.h"
#include "Utilites/Json.h"

void JsonDemo::Init()
{
	//Json::Value root;
	//Json::Value spriteData;
	//string imageName = "tiles.png";
	//Json::SetValue(spriteData, "ImageName", imageName);

	//Json::Value uvData;
	//float u = 1.0f;
	//float v = 1.0f;

	//Json::SetValue(uvData, "u", u);
	//Json::SetValue(uvData, "v", v);

	//Json::Value position;
	//Vector3 pos = Vector3(10, 20, 30);
	//Json::SetValue(position, "pos", pos);

	//spriteData["uvData"] = uvData;
	//spriteData["position"] = position;

	//root["SpriteData"] = spriteData;

	//Json::WriteData(L"./text.json", &root);

	Json::Value root;
	Json::ReadData(L"./text.json", &root);

	Json::Value spriteData = root["SpriteData"];
	string imageName;
	Json::GetValue(spriteData, "ImageName", imageName);

	Json::Value position = spriteData["position"];
	Vector3 pos;
	Json::GetValue(position, "pos", pos);

	Json::Value uvData = spriteData["uvData"];
	float u;
	float v;
	
	Json::GetValue(uvData, "u", u);
	Json::GetValue(uvData, "v", v);


	int a = 10;


}

void JsonDemo::Destroy()
{


}