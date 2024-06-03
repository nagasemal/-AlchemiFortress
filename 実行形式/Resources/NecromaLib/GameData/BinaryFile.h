//--------------------------------------------------------------------------------------
// File: BinaryFile.h
//
//　バイナリファイル読み込みクラス
// 
// Author: Hirohumi Ito
//--------------------------------------------------------------------------------------

#pragma once
class BinaryFile
{
public:

	// ファイル名を指定してロード
	static BinaryFile LoadFile(const wchar_t* fileName);

	// ムーブコンストラクタ
	BinaryFile(BinaryFile&& in)noexcept;

	BinaryFile() noexcept;

	// アクセサ
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }

private:

	// データ
	std::unique_ptr<char[]> m_data;

	// サイズ
	unsigned int m_size;

};