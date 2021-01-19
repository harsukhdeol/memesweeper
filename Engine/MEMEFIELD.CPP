#include "MemeField.h"
#include <assert.h>
#include <random>
#include "SpriteCodex.h"

void MemeField::Tile::SpawnMeme()
{
    assert(!hasMeme);
    hasMeme = true;
}

bool MemeField::Tile::HasMeme()
{
    return hasMeme;
}

void MemeField::Tile::Draw(Vei2 screenPos, Graphics& gfx) const
{
    switch (state)
    {
    case State::Hidden:
        SpriteCodex::DrawTileButton(screenPos, gfx);
        break;

    case State::Flagged:
        SpriteCodex::DrawTileButton(screenPos, gfx);
        SpriteCodex::DrawTileFlag(screenPos, gfx);
        break;
    case State::Revealed:
        if (!hasMeme) {
             SpriteCodex::DrawTile(screenPos,nNeighbourMemes, gfx);
        }
        else {
            SpriteCodex::DrawTileBomb(screenPos, gfx);
        }

    };
}

void MemeField::Tile::Reveal()
{
    assert(state == State::Hidden);  
    state = State::Revealed;
}

bool MemeField::Tile::isRevealed() const
{
    return state== State::Revealed;
}

void MemeField::Tile::ToggleFlag()
{
    assert(!isRevealed());
    if (isFlagged()) {
        state = State::Hidden;
    }
    else {
        state = State::Flagged;
    }
}

bool MemeField::Tile::isFlagged() const
{
    return state ==State::Flagged;
}

void MemeField::Tile::SetNeighbourMemes(int memeCount)
{
    assert(nNeighbourMemes == -1);
    nNeighbourMemes = memeCount;
}

MemeField::MemeField(int nMemes)
{
    assert(nMemes > 0 && nMemes < width* height);
    std::random_device rnd;
    std::mt19937 rng(rnd());
    std::uniform_int_distribution <int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);

    for (int nSpawned = 0; nSpawned < nMemes; nSpawned++)
    {
        Vei2 spawnPos;
        do {
            spawnPos = { xDist(rng), yDist(rng) };
        } while (TileAt(spawnPos).HasMeme());
        TileAt(spawnPos).SpawnMeme();
    }
    for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
    {
        for (gridPos.x = 0; gridPos.x < width; gridPos.x++) {
           TileAt(gridPos).SetNeighbourMemes( FindSurroundingMemes(gridPos));
        }
    }
}

void MemeField::Draw(Graphics& gfx) const
{
    gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
    for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
    {
        for (gridPos.x=0; gridPos.x < width; gridPos.x++) {
            TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);
        }
    }
}

void MemeField::onRevealClick(const Vei2& screenPos)
{
    Vei2 gridPos = ScreenToGrid(screenPos);
    assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y>=0 && gridPos.y < height);
    
    Tile& tile = TileAt(gridPos);
    if (!tile.isRevealed() && !tile.isFlagged())
    {
        tile.Reveal();
    }
}

void MemeField::onFlagClick(const Vei2& screenPos)
{
    Vei2 gridPos = ScreenToGrid(screenPos);
    assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y >= 0 && gridPos.y < height);

    Tile& tile = TileAt(gridPos);
    if (!tile.isRevealed()) {
    tile.ToggleFlag();

    }
}

Vei2 MemeField::ScreenToGrid(const Vei2& screenPos) const
{
    return screenPos/ SpriteCodex::tileSize;
}

MemeField::Tile&  MemeField::TileAt(const Vei2& gridPos)
{
    return field[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2& gridPos) const
{
    return field[gridPos.y * width + gridPos.x];
}

int MemeField::FindSurroundingMemes(Vei2& gridPos) 
{
    int count = 0;
    const int top = std::max(gridPos.y - 1, 0);
    const int left = std::max(gridPos.x - 1, 0);
    const int bottom = std::min(gridPos.y + 1, height-1);
    const int right = std::min(gridPos.x + 1, width-1);

    for (Vei2 pos = { top,left }; pos.y <= bottom; pos.y++) {
        for (pos.x = left; pos.x <= right; pos.x++) {
            if (TileAt(pos).HasMeme())
            {
                count++;
            }
        }
    }
    return count;
}

RectI MemeField::GetRect() const 
{
    return RectI(Vei2 ( 0,0 ), Vei2(width,height) * SpriteCodex::tileSize );
}
