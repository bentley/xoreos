/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010-2011 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file graphics/aurora/text.cpp
 *  A text object.
 */

#include "events/requests.h"

#include "graphics/graphics.h"
#include "graphics/font.h"

#include "graphics/aurora/text.h"

namespace Graphics {

namespace Aurora {

Text::Text(const FontHandle &font, const Common::UString &str, float align) :
	_font(font), _x(0.0), _y(0.0), _hasColor(false), _align(align) {

	set(str);

	_distance = -FLT_MAX;
}

Text::Text(const FontHandle &font, const Common::UString &str,
		float r, float g, float b, float a, float align) :
	_font(font), _x(0.0), _y(0.0), _hasColor(true), _r(r), _g(g), _b(b), _a(a), _align(align) {

	set(str);

	_distance = -FLT_MAX;
}

Text::~Text() {
	removeFromQueue();
}

void Text::set(const Common::UString &str) {
	GfxMan.lockFrame();

	_str = str;

	const Font &font = _font.getFont();

	_height = font.getHeight(_str);
	_width  = font.getWidth (_str);

	GfxMan.unlockFrame();
}

void Text::setColor(float r, float g, float b, float a) {
	GfxMan.lockFrame();

	_hasColor = true;
	_r = r;
	_g = g;
	_b = b;
	_a = a;

	GfxMan.unlockFrame();
}

void Text::unsetColor() {
	GfxMan.lockFrame();

	_hasColor = false;

	GfxMan.unlockFrame();
}

void Text::setPosition(float x, float y, float z) {
	GfxMan.lockFrame();

	_x = roundf(x);
	_y = roundf(y);

	_distance = z;

	GfxMan.unlockFrame();
}

void Text::show() {
	Renderable::addToQueue();
}

void Text::hide() {
	Renderable::removeFromQueue();
}

bool Text::isVisible() {
	return Renderable::isInQueue();
}

bool Text::isEmpty() {
	return _str.empty();
}

float Text::getWidth() const {
	return _width;
}

float Text::getHeight() const {
	return _height;
}

void Text::newFrame() {
}

void Text::render() {
	glTranslatef(_x, _y, 0.0);

	if (_hasColor)
		_font.getFont().draw(_str, _r, _g, _b, _a, _align);
	else
		_font.getFont().draw(_str, _align);
}

} // End of namespace Aurora

} // End of namespace Graphics
