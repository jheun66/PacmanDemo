#include "Framework.h"
#include "FileStream.h"
#include "TileMap/Tile.h"

FileStream::FileStream(string path, UINT flags)
{
	isOpen = false;
	this->flags = flags;

	int iosFlags = ios::binary;
	iosFlags |= this->flags & FILE_STREAM_WRITE ? ios::out : 0;
	iosFlags |= this->flags & FILE_STREAM_READ ? ios::in : 0;


	if (this->flags & FILE_STREAM_WRITE)
	{
		out.open(path, iosFlags);
		if (out.fail())
		{
			assert(false);
			return;
		}
	}
	else if (this->flags & FILE_STREAM_READ)
	{
		in.open(path, iosFlags);
		if (in.fail())
		{
			assert(false);
			return;
		}
	}

	isOpen = true;
}

FileStream::~FileStream()
{
	Close();
}

void FileStream::Close()
{
	if (flags & FILE_STREAM_WRITE)
	{
		out.flush();
		out.close();
	}
	else if (flags & FILE_STREAM_READ)
	{
		in.clear();
		in.close();
	}
}

void FileStream::WriteString(string value)
{
	Write(value.length());
	out.write((const char*)value.c_str(), value.length());
}

string FileStream::ReadString()
{
	int length = Read<uint>();
	string temp = "";
	temp.resize(length);
	in.read((char*)&temp[0], length);
	return temp;
}

/*
	Vector3 position = Values::ZeroVector;
	Color color = Values::Black;
	Vector2 startUV = Vector2(0, 0);
	Vector2 endUV = Vector2(0, 0);
	string spriteName = "";

	bool isWalkable = true;
*/
void FileStream::Write(Tile * tile)
{
	Write(tile->GetPosition());
	Write(tile->GetStartUV());
	Write(tile->GetEndUV());
	Write(tile->GetColor());
	Write(tile->GetIndex());
	Write(tile->GetIsWalkable());
	WriteString(tile->GetSpriteName());
}

void FileStream::Read(Tile & value)
{
	size_t size = sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector2) + sizeof(Color) + sizeof(uint) + sizeof(bool);
	in.read((char*)&value, size);
}

Tile FileStream::ReadTile()
{
	Tile t;
	Read(t);
	t.SetSpriteName(ReadString());
	return t;
}
