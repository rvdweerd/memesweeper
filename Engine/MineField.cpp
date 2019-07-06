#include "MineField.h"
#include <random>
#include <assert.h>
#include "SpriteCodex.h"

MineField::MineField(int nBombs)
{
	std::random_device rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<int> iDistr(0, width * height);

	assert(nBombs < width * height);
	int index;
	for (int i = 0; i < nBombs; i++)
	{
		do
		{
			index = iDistr(rng);
		} while ( tileField[index].HasBomb() );
		tileField[index].SpawnBomb();
	}
}

void MineField::Draw(Graphics& gfx)
{
	gfx.DrawRect(RectI({ 0,0 }, Vei2(width, height)*SpriteCodex::tileSize), SpriteCodex::baseColor);
	
	for (Vei2 grid = { 0,0 }; grid.y < height; grid.y++)
		for (grid.x = 0; grid.x < width; grid.x++)
		{
			TileAt(grid).Draw(grid * SpriteCodex::tileSize, gfx);
		}
		
	
	/*
	for (int i=0; i<width*height;i++)
	{
		tileField[i].Draw( TileAt(i), gfx);
	}
	*/
}

void MineField::OnRevealClick(const Vei2& screenPos)
{
	//TileAt(ScreenToGrid(screenPos)).Reveal();
	const Vei2 gridPos = ScreenToGrid(screenPos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed())
	{
		tile.Reveal();
	}


}

/*Vei2 MineField::TileAt(int index)
{
	return Vei2( index%width , (index - index%width)/width );
}*/

MineField::Tile& MineField::TileAt(const Vei2& gridPos)
{
	return tileField[gridPos.y * width + gridPos.x];
}

const MineField::Tile& MineField::TileAt(const Vei2& gridPos) const
{
	return tileField[gridPos.y*width+gridPos.x];
}

Vei2 MineField::ScreenToGrid(const Vei2& screenPos) const
{
	return screenPos / SpriteCodex::tileSize;
}

bool MineField::Tile::HasBomb()
{
	return hasBomb;
}

void MineField::Tile::SpawnBomb()
{
	hasBomb = true;
}

bool MineField::Tile::IsRevealed()
{
	return state == State::Revealed;
}

void MineField::Tile::Reveal()
{
	state = State::Revealed;
}

void MineField::Tile::Draw(const Vei2& screenPos, Graphics& gfx)
{
	switch (state)
	{
		case State::Hidden:
		{
			SpriteCodex::DrawTileButton(screenPos, gfx);
			break; 
		}
		case State::Flagged:
		{
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
			break;
		}
		case State::Revealed:
		{
			if (hasBomb)
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTile0(screenPos, gfx);
			}
		}

	}
}
