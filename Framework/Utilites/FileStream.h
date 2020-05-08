#pragma once
enum EFileStream : UINT
{
	FILE_STREAM_READ = 1U << 0,
	FILE_STREAM_WRITE = 1U << 1,
};

class Tile;

class FileStream
{
public:
	FileStream(string path, UINT flags);
	~FileStream();

	void Close();

	template <typename T, typename = typename enable_if<
		is_same<T, bool>::value ||
		is_same<T, char>::value ||
		is_same<T, byte>::value ||
		is_same<T, int>::value ||
		is_same<T, D3DXVECTOR3>::value ||
		is_same<T, D3DXVECTOR2>::value ||
		is_same<T, D3DXMATRIX>::value ||
		is_same<T, D3DXCOLOR>::value ||
		is_same<T, UINT>::value 
	>::type>
		void Write(T value)
	{
		out.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

	template <typename T, typename = typename enable_if<
		is_same<T, bool>::value ||
		is_same<T, char>::value ||
		is_same<T, byte>::value ||
		is_same<T, int>::value ||
		is_same<T, D3DXVECTOR3>::value ||
		is_same<T, D3DXVECTOR2>::value ||
		is_same<T, D3DXMATRIX>::value ||
		is_same<T, D3DXCOLOR>::value ||
		is_same<T, UINT>::value 
	>::type>
		void Read(T& value)
	{
		in.read(reinterpret_cast<char*>(&value), sizeof(T));
	}

	template <typename T, typename = typename enable_if<
		is_same<T, bool>::value ||
		is_same<T, char>::value ||
		is_same<T, byte>::value ||
		is_same<T, int>::value ||
		is_same<T, D3DXVECTOR3>::value ||
		is_same<T, D3DXVECTOR2>::value ||
		is_same<T, D3DXMATRIX>::value ||
		is_same<T, D3DXCOLOR>::value ||
		is_same<T, UINT>::value 
	>::type>
		T Read()
	{
		T value;
		Read(value);

		return value;
	}

	void WriteString(string value);
	string ReadString();

	void Write(class Tile* tile);
	void Read(Tile& value);
	class Tile ReadTile();

private:
	ofstream out;
	ifstream in;

	UINT flags = 0;
	bool isOpen = false;

};