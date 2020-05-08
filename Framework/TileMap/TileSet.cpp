#include "Framework.h"
#include "TileSet.h"

void TileSet::GUI()
{
	ImGui::Begin("TileSet");
	{
		if (ImGui::Button("Load", ImVec2(100, 40)))
			OpenSpriteImage();

		if (ImGui::Button("SaveInfos", ImVec2(100, 40)))
			SaveSpriteInfos();
		
		if (ImGui::Button("OpenInfos", ImVec2(100, 40)))
			OpenSpriteInfos();

		if (ImGui::BeginTabBar("SpriteTabs", ImGuiTabBarFlags_AutoSelectNewTabs))
		{
			for (auto& sprite : spriteMap)
			{
				if (ImGui::BeginTabItem(sprite.first.c_str()))
				{
					if (ImGui::SliderInt("TileXCount", &sprite.second.tileXCount, 1, 30))
						sprite.second.CalculateTexelTileSize();
					if (ImGui::SliderInt("TileYCount", &sprite.second.tileYCount, 1, 30))
						sprite.second.CalculateTexelTileSize();

					CreateImageButton(sprite);
				
					ImGui::EndTabItem();
				}//if(tabItem)
			}//for(spriteMap)
			ImGui::EndTabBar();
		}//if(beginTabBar)
	}//ImGui::Begin
	ImGui::End();
}

void TileSet::CreateImageButton(pair<string, SpriteData> sprite)
{
	ID3D11ShaderResourceView* tileSRV = sprite.second.sprite->GetSRV();
	vector<TileData> tileDatas = sprite.second.tileDatas;

	if (tileDatas.size() == 0)
	{
		int index = 0;
		for (uint y = 0; y < sprite.second.tileYCount; y++)
		{
			for (uint x = 0; x < sprite.second.tileXCount; x++)
			{
				float startX = x * sprite.second.texelTileSize.x;
				float startY = y * sprite.second.texelTileSize.y;

				ImGui::PushID(index);
				if (ImGui::ImageButton(tileSRV, ImVec2(40, 40),
					ImVec2(startX, startY),
					ImVec2(startX + sprite.second.texelTileSize.x, startY + sprite.second.texelTileSize.y)))
				{
					SelectedStartUV = Vector2(startX, startY);
					SelectedtexelTileSize = sprite.second.texelTileSize;
					selectedSpriteName = sprite.first;
				}
				index++;
				ImGui::PopID();

				if (index % 4 != 0)
					ImGui::SameLine();
			}
		}
	}
	else
	{
		int index = 0;
		for (uint i = 0; i < tileDatas.size(); i++)
		{
			ImGui::PushID(index);
			if (ImGui::ImageButton(tileSRV, ImVec2(40, 40),
				ImVec2(tileDatas[i].tilePos.x, tileDatas[i].tilePos.y),
				ImVec2(tileDatas[i].tilePos.x + tileDatas[i].tileSize.x, tileDatas[i].tilePos.y + tileDatas[i].tileSize.y)))
			{
				SelectedStartUV = tileDatas[i].tilePos;
				SelectedtexelTileSize = tileDatas[i].tileSize;
				selectedSpriteName = sprite.first;
			}
			index++;
			ImGui::PopID();
			if (index % 4 != 0)
				ImGui::SameLine();
		}
	}
	
}

void TileSet::OpenSpriteImage(wstring filePath)
{
	if (filePath.length() < 1)
	{
		function<void(wstring)> func = bind(&TileSet::OpenSpriteImage, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::SpriteFilter, L".\\", func, handle);
	}
	else
	{
		wstring ext = Path::GetExtension(filePath);
		SpriteData data;
		if (ext != L"json")
		{
			data.sprite = new Texture2D(filePath);
			string spriteName = Path::GetFileNameWithoutExtension(String::ToString(filePath));
			spriteMap.insert(make_pair(spriteName, data));
		}
		else
		{
			Json::Value root;
			Json::ReadData(filePath, &root);

			Json::Value meta = root["meta"];
			string imageName;

			Json::GetValue(meta, "image", imageName);
			data.sprite = new Texture2D(TexturePath + String::ToWstring(imageName));

			Json::Value size = meta["size"];
			int sizeX, sizeY;
			Json::GetValue(size, "w", sizeX);
			Json::GetValue(size, "h", sizeY);
			Vector2 imageSize = Vector2((float)sizeX, (float)sizeY);
			////////////////////////////////////////////////////////////////////////////

			Json::Value frames = root["frames"];

			vector<string> imageNames = frames.getMemberNames();
			for (string name : imageNames)
			{
				TileData tileData;
				tileData.tileName = String::ToWstring(name);
				Json::Value uvData = frames[name]["frame"];

				int posX, posY;
				sizeX = 0, sizeY = 0;

				Json::GetValue(uvData, "x", posX);
				Json::GetValue(uvData, "y", posY);
				Json::GetValue(uvData, "w", sizeX);
				Json::GetValue(uvData, "h", sizeY);

				tileData.tilePos = Vector2(posX / imageSize.x, posY / imageSize.y);
				tileData.tileSize = Vector2(sizeX / imageSize.x, sizeY / imageSize.y);

				data.tileDatas.push_back(tileData);

			}
			string keyValue = Path::GetFileNameWithoutExtension(imageName);
			spriteMap.insert(make_pair(keyValue, data));

		}
	}
}

void TileSet::SaveSpriteInfos()
{
	FileStream* write = nullptr;
	string filePath = "";
	for (auto& spriteData : spriteMap)
	{
		filePath = String::ToString(TileMapDataPath) + spriteData.first + ".sif";
		write = new FileStream(filePath, FILE_STREAM_WRITE);

		write->WriteString(spriteData.first);
		write->WriteString(String::ToString(spriteData.second.sprite->GetFilePath()));
		write->Write(spriteData.second.tileXCount);
		write->Write(spriteData.second.tileYCount);
		write->Write(spriteData.second.texelTileSize);

		vector<TileData> tileDatas = spriteData.second.tileDatas;
		uint size = tileDatas.size();
		write->Write(size);
		for (auto data : tileDatas)
		{
			write->WriteString(String::ToString(data.tileName));
			write->Write(data.tilePos);
			write->Write(data.tileSize);
		}


		SAFE_DELETE(write);
		
	}

}

void TileSet::OpenSpriteInfos(wstring filePath)
{
	if (filePath.length() < 1)
	{
		function<void(wstring)> func = bind(&TileSet::OpenSpriteInfos, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::SpriteInfoFilter, L".\\", func, handle);
	}
	else
	{
		FileStream* read = new FileStream(String::ToString(filePath), FILE_STREAM_READ);

		SpriteData temp;
		string keyValue = read->ReadString();

		temp.sprite = new Texture2D(String::ToWstring(read->ReadString()));
		temp.tileXCount = read->Read<int>();
		temp.tileYCount = read->Read<int>();
		temp.texelTileSize = read->Read<Vector2>();

		uint size = read->Read<uint>();
		if (size > 0)
		{
			for (uint i = 0; i < size; i++)
			{
				TileData data;
				data.tileName = String::ToWstring(read->ReadString());
				data.tilePos = read->Read<Vector2>();
				data.tileSize = read->Read<Vector2>();

				temp.tileDatas.push_back(data);
			}
		}

		// 똑같은 키값이 있으면 제거
		if (spriteMap.find(keyValue) != spriteMap.end())
		{
			SAFE_DELETE(spriteMap.find(keyValue)->second.sprite);
			spriteMap.erase(keyValue);
		}

		spriteMap.insert(make_pair(keyValue, temp));

		SAFE_DELETE(read);
		
	}
}

TileSet::TileSet()
{
	
}

TileSet::~TileSet()
{
	SaveSpriteInfos();

	for (auto sprite : spriteMap)
		SAFE_DELETE(sprite.second.sprite);
}
