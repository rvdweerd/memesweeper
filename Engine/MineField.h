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
		bool ToggleFlag();
		void Draw(const Vei2& screenPos, Graphics& gfx, bool fieldExploded) const;
		int GetNBombsAround() const;
		void SetNBombsAround(const int n);

	private:
		State state = State::Hidden;
		bool hasBomb = false;
		int nBombsAround=-1;
	};

public:
	MineField(Vei2& centerPos, const int nBombs);
	void Draw(Graphics& gfx) const;
	void OnRevealClick(const Vei2& gridPos);
	int CountNeighborBombs(const Vei2& gridPos) const;
	void ToggleFlag(Vei2& screenPos);
	bool Exploded() const;
	bool CheckIsCleared() const;
	bool IsCleared() const;
	Vei2 ScreenToGrid(const Vei2& screenPos) const;
	RectI GetRect() const;
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
public:
	static constexpr int width  = 9;
	static constexpr int height = 5;
	static constexpr int nBombs = int(width*height*0.15f);
private:
	Vei2 topLeft;
	Tile tileField[height * width];
	bool fieldHasExploded = false;
	bool isCleared = false;
	Sound endOfGameSound;
	Sound flagSound;
	Sound revealSound;

};