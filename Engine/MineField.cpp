#include "MineField.h"
#include <random>
#include <assert.h>
#include "SpriteCodex.h"
bool MineField::Tile::HasBomb() const
{
	return hasBomb;
}

void MineField::Tile::SpawnBomb()
{
	assert(!hasBomb);
	hasBomb = true;
}

bool MineField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

void MineField::Tile::Reveal()
{
	state = State::Revealed;
}

bool MineField::Tile::IsFlagged() const
{
	return state == State::Flagged;
}

bool MineField::Tile::ToggleFlag()
{
	switch (state)
	{
	case State::Hidden: 
		state = State::Flagged;
		return true;
		break;
	case State::Flagged:
		state = State::Hidden;
		return false;
		break;
	}
	return false;
}

void MineField::Tile::Draw(const Vei2& screenPos, Graphics& gfx, bool fieldExploded) const
{
	if (!fieldExploded)
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
				SpriteCodex::DrawTileNumber(screenPos, nBombsAround, gfx);
				break;
			}
		}
	}
	else // field has exploded
	{
		switch (state)
		{
			case State::Hidden:
			{
				if (hasBomb)
				{
					SpriteCodex::DrawTileBomb(screenPos, gfx);
				}
				else
				{
					SpriteCodex::DrawTileButton(screenPos, gfx);
				}
				break;
			}
			case State::Flagged:
			{
				if (hasBomb)
				{
					SpriteCodex::DrawTileBomb(screenPos, gfx);
					SpriteCodex::DrawTileFlag(screenPos, gfx);
				}
				else
				{
					SpriteCodex::DrawTileBomb(screenPos, gfx);
					SpriteCodex::DrawTileCross(screenPos, gfx);
				}
				break;
			}
			case State::Revealed:
			{
				if (hasBomb)
				{
					SpriteCodex::DrawTileBombRed(screenPos, gfx);
				}
				else
				{
					SpriteCodex::DrawTileNumber(screenPos, nBombsAround, gfx);
				}
				break;
			}
		}
	}
}

int MineField::Tile::GetNBombsAround() const
{
	return nBombsAround;
}

void MineField::Tile::SetNBombsAround(const int n)
{
	nBombsAround = n;
}

MineField::MineField(Vei2& centerPos, const int nBombs)
	:
	topLeft( centerPos - Vei2(width,height)*SpriteCodex::tileSize/2 ),
	endOfGameSound(L"spayed.wav"), //unicode wide character string
	flagSound(L"fart1.wav"),
	revealSound(L"coin.wav")
{
	std::random_device rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<int> iDistr(0, width * height);

	//put bombs in place
	assert(nBombs > 0 && nBombs < width * height);
	assert(width * SpriteCodex::tileSize <= Graphics::ScreenWidth && height * SpriteCodex::tileSize <= Graphics::ScreenHeight);
	int index;
	for (int i = 0; i < nBombs; i++)
	{
		do
		{
			index = iDistr(rng);
		} while ( tileField[index].HasBomb() );
		tileField[index].SpawnBomb();
	}

	//test and assign number of bombs around to each cell
	for (int i = 0; i < width * height; i++)
	{
		Vei2 gridPos = { i % width , (i - i % width) / width };
		Tile& tile = TileAt(gridPos);
		if (tile.GetNBombsAround() == -1)
		{
			tile.SetNBombsAround( CountNeighborBombs(gridPos) );
		}
	}
}

void MineField::Draw(Graphics& gfx) const
{
	gfx.DrawRect( GetRect() , SpriteCodex::baseColor);
	
	for (Vei2 grid = { 0,0 }; grid.y < height; grid.y++)
		for (grid.x = 0; grid.x < width; grid.x++)
		{
			TileAt(grid).Draw(topLeft + grid * SpriteCodex::tileSize, gfx, fieldHasExploded);
		}
	/*
	for (int i=0; i<width*height;i++)
	{
		tileField[i].Draw( TileAt(i), gfx);
	}
	*/
}

void MineField::OnRevealClick(const Vei2& gridPos)
{
	{
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed() && !tile.IsFlagged())
		{
			tile.Reveal();
			if (tile.HasBomb() && !fieldHasExploded)
			{
				fieldHasExploded = true;
				flagSound.Play(); Sleep(300);
				endOfGameSound.Play();
			}

			if (tile.GetNBombsAround() == 0)
			{
				for (int x = std::max(0, gridPos.x - 1); x <= std::min(width-1,gridPos.x + 1); x++)
				{
					for (int y = std::max(0,gridPos.y - 1); y <= std::min(height-1,gridPos.y + 1); y++)
					{
						if (!(Vei2(x, y) == gridPos))
						{
							OnRevealClick(Vei2(x, y));
						}
					}
				}
			}

		}
	}
	if (!isCleared && CheckIsCleared() )
	{
		isCleared = true;
		endOfGameSound.Play();
	}
	//return isCleared;
}

int MineField::CountNeighborBombs(const Vei2& gridPos) const
{
	int bombCount = 0;
	for (int x = std::max(0, gridPos.x - 1); x <= std::min(width - 1, gridPos.x + 1); x++)
	{
		for (int y = std::max(0, gridPos.y - 1); y <= std::min(height - 1, gridPos.y + 1); y++)
		{
			//if (!(Vei2(x, y) == gridPos))
			{
				if ( TileAt(Vei2(x, y)).HasBomb() )
				{
					bombCount++;
				}
			}
		}
	}
	return bombCount;
}

void MineField::ToggleFlag(Vei2& gridPos)
{
	{
		if (TileAt(gridPos).ToggleFlag())
		{
			flagSound.Play();
		}
	}
}

bool MineField::Exploded() const
{
	return fieldHasExploded;
}

bool MineField::CheckIsCleared() const
{
	int nRevealed = 0;
	for (Vei2 gridSweep = { 0,0 }; gridSweep.x < width; gridSweep.x++)
	{
		for (gridSweep.y = 0; gridSweep.y < height; gridSweep.y++)
		{
			if ( TileAt(gridSweep).IsRevealed() )
			{
				nRevealed++;
			}
		}
	}
	return (nRevealed == width*height - nBombs);
}

bool MineField::IsCleared() const
{
	return isCleared;
}

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
	return (screenPos-topLeft) / SpriteCodex::tileSize;
}

RectI MineField::GetRect() const
{
	return RectI(topLeft, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize);
}
