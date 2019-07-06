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

void MineField::DrawNeighborData(Graphics& gfx, Vei2& gridPos)
{
	//TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);
	switch (TileAt(gridPos).GetNBombsAround())
	{
	case 0: SpriteCodex::DrawTile0(gridPos * SpriteCodex::tileSize, gfx); break;
	case 1: SpriteCodex::DrawTile1(gridPos * SpriteCodex::tileSize, gfx); break;
	case 2: SpriteCodex::DrawTile2(gridPos * SpriteCodex::tileSize, gfx); break;
	case 3: SpriteCodex::DrawTile3(gridPos * SpriteCodex::tileSize, gfx); break;
	case 4: SpriteCodex::DrawTile4(gridPos * SpriteCodex::tileSize, gfx); break;
	case 5: SpriteCodex::DrawTile5(gridPos * SpriteCodex::tileSize, gfx); break;
	case 6: SpriteCodex::DrawTile6(gridPos * SpriteCodex::tileSize, gfx); break;
	case 7: SpriteCodex::DrawTile7(gridPos * SpriteCodex::tileSize, gfx); break;
	case 8: SpriteCodex::DrawTile8(gridPos * SpriteCodex::tileSize, gfx); break;
	}
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

void MineField::Peek(const Vei2& screenPos)
{
	Vei2 const gridPos = ScreenToGrid(screenPos);
	Tile& tile = TileAt(gridPos);
	//tile.ScanNeighbors(gridPos);
	int nNeighbors = 0;
	int nBombsAround = 0;
	for (int x = std::max(0, gridPos.x - 1); x <= std::min(width - 1, gridPos.x + 1); x++)
	{
		for (int y = std::max(0, gridPos.y - 1); y <= std::min(height - 1, gridPos.y + 1); y++)
		{
			if (!(Vei2(x, y) == gridPos))
			{
				nNeighbors++;
				if (TileAt(Vei2(x, y)).hasBomb) 
				{
					nBombsAround++;
				}
			}
		}
	}
	tile.SetNeighborData(nNeighbors, nBombsAround);
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

void MineField::Tile::ScanNeighbors(const Vei2& gridPos)
{
	
}

void MineField::Tile::SetNeighborData(const int nNeighbors_in, const int nBombsAround_in)
{
	nNeighbors = nNeighbors_in;
	nBombsAround = nBombsAround_in;
}

int MineField::Tile::GetNBombsAround()
{
	return nBombsAround;
}
