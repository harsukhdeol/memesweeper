#pragma once

#include "Graphics.h"
#include "Sound.h"

class MemeField
{
public:
	enum class State { ded, won, memeing };
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Draw( const Vei2& screenPos,MemeField::State gameState,Graphics& gfx ) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborMemeCount( int memeCount );
		bool HasNoNeighboringMemes()const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
	};
public:
	MemeField( int nMemes, const Vei2& center );
	void Draw( Graphics& gfx ) const;
	RectI GetRect() const;
	void OnRevealClick( const Vei2& screenPos );
	void OnFlagClick( const Vei2& screenPos );
	bool GameWon()const;
private:
	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid( const Vei2& screenPos );
	int CountNeighborMemes( const Vei2& gridPos );
	void RevealTile(const Vei2& gridPos);

private:
	
	static constexpr int width = 20;
	static constexpr int height = 20;
	static constexpr int thickness =10;
	static constexpr Color c = {255,200,200};
	State state = State::memeing;
	Tile field[width * height];
	Vei2 topLeft;
	Sound LostSound = Sound(L"spayed.wav");
};