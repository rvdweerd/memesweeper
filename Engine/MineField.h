#pragma once
#include "Graphics.h"
#include "SpriteCodex.h"
#include "Sound.h"

class MineField {
		
	class Tile {
	public:
		enum class State {
			Hidden,
			Flagged,
			Revealed
		};
		bool HasBomb() const;
		void SpawnBomb();
		bool IsRevealed() const;
		void Reveal();
		bool IsFlagged() const;
		void ToggleFlag();
		void Draw(const Vei2& screenPos, Graphics& gfx, bool fieldExploded, Vei2 delta) const;
		int GetNBombsAround() const;
		void SetNBombsAround(const int n);

	private:
		State state = State::Hidden;
		bool hasBomb = false;
		int nBombsAround=-1;
	};

public:
	MineField(const int nBombs);
	void Draw(Graphics& gfx) const;
	void OnRevealClick(const Vei2& screenPos);
	int CountNeighborBombs(const Vei2& gridPos) const;
	void ToggleFlag(Vei2& screenPos);
	bool Exploded() const;
	bool CheckIsCleared() const;
	bool IsCleared() const;
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos) const;
public:
	static constexpr int width  = 30;
	static constexpr int height = 20;
	static constexpr int nBombs = int(width*height*0.03f);
private:
	Vei2 topLeft = { Graphics::ScreenWidth/2-width/2*SpriteCodex::tileSize, 
					 int (float(Graphics::ScreenHeight) / 2 - float(height) / 2 * float(SpriteCodex::tileSize)) };
	Tile tileField[height * width];
	bool fieldHasExploded = false;
	bool isCleared = false;
	Sound endOfGame;

};