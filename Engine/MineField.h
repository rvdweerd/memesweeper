#pragma once
#include "Graphics.h"

class MineField {
		
	class Tile {
	public:
		enum class State {
			Hidden,
			Flagged,
			Revealed
		};
		bool HasBomb();
		void SpawnBomb();
		bool IsRevealed();
		void Reveal();
		void Draw(const Vei2& screenPos, Graphics& gfx);
		int GetNBombsAround();

	public:
		State state = State::Hidden;
		bool hasBomb = false;
		int nBombsAround=-1;
	};

public:
	//MineField() = default;
	MineField(int nBombs);
	void Draw(Graphics& gfx);
	void DrawNeighborData(Graphics& gfx, Vei2& gridPos);
	void OnRevealClick(const Vei2& screenPos);
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos) const;
private:
	static constexpr int width = 20;
	static constexpr int height = 15;
	Tile tileField[height * width];

};