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
		void Draw(Vei2& screenPos, State state, Graphics& gfx);

	public:
		State state = State::Hidden;
		bool hasBomb = false;
	};

public:
	MineField() = default;
	MineField(int nBombs);
	void Draw(Graphics& gfx);
	//Vei2 TileAt(int index);
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
private:
	static constexpr int width = 20;
	static constexpr int height = 15;
	Tile tileField[height * width];

};