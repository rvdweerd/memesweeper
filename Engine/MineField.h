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
		bool HasBomb() const;
		void SpawnBomb();
		bool IsRevealed() const;
		void Reveal();
		bool IsFlagged() const;
		void ToggleFlag();
		void Draw(const Vei2& screenPos, Graphics& gfx, bool fieldExploded) const;
		int GetNBombsAround() const;
		void SetNBombsAround(const int n);

	private:
		State state = State::Hidden;
		bool hasBomb = false;
		int nBombsAround=-1;
	};

public:
	//MineField() = default;
	MineField(const int nBombs);
	void Draw(Graphics& gfx) const;
	void DrawNeighborData(Graphics& gfx, Vei2& gridPos) const;
	void OnRevealClick(const Vei2& screenPos);
	int CountNeighborBombs(const Vei2& gridPos) const;
	void ToggleFlag(Vei2& gridPos);
	bool Exploded() const;
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos) const;
private:
	static constexpr int width = 20;
	static constexpr int height = 15;
	Tile tileField[height * width];
	bool fieldHasExploded = false;

};