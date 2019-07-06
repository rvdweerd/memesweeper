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
		void ScanNeighbors(const Vei2& gridPos);
		void SetNeighborData(const int nNeighbors_in, const int nBombsAround_in);
		int GetNBombsAround();
	private:
		Vei2 neighbors[8];
		int nNeighbors=0;
		int nBombsAround=0;
	public:
		State state = State::Hidden;
		bool hasBomb = false;
	};

public:
	MineField() = default;
	MineField(int nBombs);
	void Draw(Graphics& gfx);
	void DrawNeighborData(Graphics& gfx, Vei2& gridPos);
	void OnRevealClick(const Vei2& screenPos);
	void Peek(const Vei2& screenPos);
private:
	//Vei2 TileAt(int index);
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos) const;
private:
	static constexpr int width = 20;
	static constexpr int height = 15;
	Tile tileField[height * width];

};