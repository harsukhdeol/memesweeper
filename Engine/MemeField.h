#pragma once
#include "Vei2.h"
#include "Graphics.h"

class MemeField
{
public:
private:
	class Tile
	{
	public:
		enum class State {
			Hidden, Flagged, Revealed};
	public:
		void SpawnMeme();
		bool HasMeme();
		void Draw(Vei2 screenPos, Graphics& gfx) const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const; //returns const reference to a tile
	RectI GetRect() const;
private:
	static constexpr int width = 20;
	static constexpr int height = 16;
	Tile field[width * height];
};