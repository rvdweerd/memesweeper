/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	mineField( Vei2(gfx.ScreenWidth,gfx.ScreenHeight)/2, MineField::nBombs )
	//endOfGame(L"spayed.wav") //unicode wide character string
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if ( !mineField.Exploded() && !mineField.IsCleared() )
	{
		if (wnd.mouse.LeftIsPressed())
		{
			Vei2 mousePos = wnd.mouse.GetPos();
			if (mineField.GetRect().Contains(mousePos))
			{
				mineField.OnRevealClick(mineField.ScreenToGrid(mousePos));
			}
		}

		while (!wnd.mouse.IsEmpty())
		{
			const Mouse::Event e = wnd.mouse.Read();
			if (e.GetType() == Mouse::Event::Type::RPress)
			{
				Vei2 mousePos = e.GetPos();
				if (mineField.GetRect().Contains(mousePos))
				{
					mineField.ToggleFlag(mineField.ScreenToGrid(mousePos));
				}
			}
		}
	}
}
	
void Game::ComposeFrame()
{
	mineField.Draw(gfx);

	if (mineField.Exploded())
	{
		//SpriteCodex::DrawGameOver();
	}

	if ( mineField.IsCleared() )
	{
		SpriteCodex::DrawWin(Vei2(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2), gfx);
		
		/* TEST DRAW GAMESPRITES
		int i = 400;
		SpriteCodex::DrawTile0({ i += 20,100 }, gfx);
		SpriteCodex::DrawTile1({ i += 20,100 }, gfx);
		SpriteCodex::DrawTile2({ i += 20,100 }, gfx);
		SpriteCodex::DrawTile3({ i += 20,100 }, gfx);
		SpriteCodex::DrawTile4({ i += 20,100 }, gfx);
		SpriteCodex::DrawTile5({ i += 20,100 }, gfx);
		SpriteCodex::DrawTile6({ i += 20,100 }, gfx);
		SpriteCodex::DrawTile7({ i += 20,100 }, gfx);
		SpriteCodex::DrawTile8({ i ,100 }, gfx);
		i = 400;
		SpriteCodex::DrawTileButton({ i += 20 ,120 }, gfx);
		SpriteCodex::DrawTileCross({ i += 20 ,120 }, gfx);
		SpriteCodex::DrawTileFlag({ i += 20 ,120 }, gfx);
		SpriteCodex::DrawTileBomb({ i += 20 ,120 }, gfx);
		SpriteCodex::DrawTileBombRed({ i += 20 ,120 }, gfx);
		*/
	}

}
