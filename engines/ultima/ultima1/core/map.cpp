/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "ultima/ultima1/core/map.h"
#include "ultima/ultima1/core/people.h"
#include "ultima/ultima1/core/transports.h"
#include "ultima/ultima1/core/resources.h"
#include "ultima/ultima1/game.h"
#include "ultima/shared/core/file.h"

namespace Ultima {
namespace Ultima1 {

using Shared::File;

void SurroundingTotals::load(Ultima1Map *map) {
	U1MapTile mapTile;
	_water = _woods = _grass = 0;
	
	// Iterate through the surrounding tiles relative to the player
	for (int deltaY = -1; deltaY <= 1; ++deltaY) {
		for (int deltaX = -1; deltaX <= 1; ++deltaX) {
			Point delta(deltaX * map->_tilesPerOrigTile.x, deltaY * map->_tilesPerOrigTile.y);
			map->getTileAt(map->getDeltaPosition(delta), &mapTile);

			if (mapTile.isOriginalWater())
				++_water;
			else if (mapTile.isOriginalGrass())
				++_grass;
			else if (mapTile.isOriginalWoods())
				++_woods;
		}
	}
}

/*------------------------------------------------------------------------*/

Ultima1Map::Ultima1Map(Ultima1Game *game) : Shared::Map(), _game(game), _mapType(MAP_OVERWORLD), _mapStyle(0), _mapIndex(0) {
	_currentTransport = new TransportOnFoot(game, this);
	addWidget(_currentTransport);
}

void Ultima1Map::loadMap(int mapId, uint videoMode) {
	Map::loadMap(mapId, videoMode);

	if (mapId == MAPID_OVERWORLD)
		loadOverworldMap();
	else
		loadTownCastleMap();
}

void Ultima1Map::loadOverworldMap() {
	_mapType = MAP_OVERWORLD;
	_mapStyle = _mapIndex = 0;
	_fixed = false;
	_name.clear();

	_size = Point(168, 156);
	_tilesPerOrigTile = Point(1, 1);
	_data.resize(_size.x * _size.y);

	File f("map.bin");
	byte b;
	for (int y = 0; y < _size.y; ++y) {
		for (int x = 0; x < _size.x; x += 2) {
			b = f.readByte();
			_data[y * _size.x + x] = b >> 4;
			_data[y * _size.x + x + 1] = b & 0xf;
		}
	}
}

void Ultima1Map::loadTownCastleMap() {
	_size = Point(38, 18);
	_tilesPerOrigTile = Point(1, 1);
	_data.resize(_size.x * _size.y);
	_fixed = true;

	// Set up properties for the map
	if (_mapId < 33) {
		// Town/city
		_mapType = MAP_CITY;
		_mapStyle = (_mapId % 8) + 2;
		_mapIndex = _mapId;
		setPosition(Common::Point(width() / 2, height() - 1));		// Start at bottom center edge of map
		_name = Common::String::format("%s %s", _game->_res->THE_CITY_OF, _game->_res->LOCATION_NAMES[_mapId - 1]);
	} else {
		// Castle
		_mapType = MAP_CASTLE;
		_mapStyle = _mapId % 2;
		_mapIndex = _mapId - 33;
		setPosition(Common::Point(0, height() / 2));		// Start at center left edge of map
		_name = _game->_res->LOCATION_NAMES[_mapId - 1];
	}

	// Load the contents of the map
	File f("tcd.bin");
	f.seek(_mapStyle * 684);
	for (int y = 0; y < _size.y; ++y) {
		for (int x = 0; x < _size.x; ++x)
			_data[y * _size.x + x] = f.readByte();
	}

	// Load up the people that are present in the map
	loadLocationWidgets();
}

void Ultima1Map::getTileAt(const Point &pt, Shared::MapTile *tile) {
	Shared::Map::getTileAt(pt, tile);

	// Special handling for one of the city/town tile numbers
	if (tile->_tileNum >= 51 && (_mapType == MAP_CITY || _mapType == MAP_CASTLE))
		tile->_tileNum = 1;

	// Extended properties to set if an Ultima 1 map tile structure was passed in
	U1MapTile *mapTile = dynamic_cast<U1MapTile *>(tile);
	if (mapTile) {
		GameResources *res = _game->_res;

		// Check for a location at the given position
		mapTile->_locationNum = -1;
		if (_mapType == MAP_OVERWORLD) {
			for (int idx = 0; idx < LOCATION_COUNT; ++idx) {
				if (pt.x == res->LOCATION_X[idx] && pt.y == res->LOCATION_Y[idx]) {
					mapTile->_locationNum = idx + 1;
					break;
				}
			}
		}
	}
}

void Ultima1Map::loadLocationWidgets() {
	_widgets.clear();

	if (_mapType == MAP_CITY || _mapType == MAP_CASTLE) {
		for (int idx = 0; idx < 15; ++idx) {
			LocationPerson &lp = _game->_res->LOCATION_PEOPLE[_mapStyle * 15 + idx];
			if (lp._id == -1)
				break;

			Person *person = new Person(_game, this, lp._id, lp._hitPoints);
			person->_position = Point(lp._x, lp._y);
			addWidget(person);
		}
	}
}

} // End of namespace Ultima1
} // End of namespace Ultima