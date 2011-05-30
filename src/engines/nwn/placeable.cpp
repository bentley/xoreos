/* eos - A reimplementation of BioWare's Aurora engine
 *
 * eos is the legal property of its developers, whose names can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
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
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 */

/** @file engines/nwn/placeable.cpp
 *  NWN placeable.
 */

#include "common/util.h"

#include "aurora/gfffile.h"
#include "aurora/2dafile.h"
#include "aurora/2dareg.h"

#include "graphics/aurora/model.h"

#include "engines/aurora/util.h"

#include "engines/nwn/placeable.h"

#include "engines/nwn/gui/widgets/tooltip.h"

namespace Engines {

namespace NWN {

Placeable::Placeable() : Situated(kObjectTypePlaceable), _tooltip(0) {
}

Placeable::~Placeable() {
	delete _tooltip;
}

void Placeable::load(const Aurora::GFFStruct &placeable) {
	Common::UString temp = placeable.getString("TemplateResRef");

	Aurora::GFFFile *utp = 0;
	if (!temp.empty()) {
		try {
			utp = loadGFF(temp, Aurora::kFileTypeUTP, MKID_BE('UTP '));
		} catch (...) {
		}
	}

	Situated::load(placeable, utp ? &utp->getTopLevel() : 0);

	if (!utp)
		warning("Placeable \"%s\" has no blueprint", _tag.c_str());

	delete utp;
}

void Placeable::hide() {
	leave();

	Situated::hide();
}

void Placeable::loadObject(const Aurora::GFFStruct &gff) {
}

void Placeable::loadAppearance() {
	const Aurora::TwoDAFile &twoda = TwoDAReg.get("placeables");

	_modelName = twoda.getRow(_appearanceID).getString("ModelName");
}

void Placeable::enter() {
	highlight(true);
}

void Placeable::leave() {
	highlight(false);
}

void Placeable::highlight(bool enabled) {
	if (_model)
		_model->drawBound(enabled);

	if (enabled)
		showTooltip();
	else
		hideTooltip();
}

void Placeable::createTooltip() {
	if (_tooltip)
		return;

	_tooltip = new Tooltip(Tooltip::kTypeFeedback, *_model);

	_tooltip->setAlign(0.5);
	_tooltip->addLine(_name, 0.5, 0.5, 1.0, 1.0);
	_tooltip->setPortrait(_portrait);
}

void Placeable::showTooltip() {
	createTooltip();
	_tooltip->show();
}

void Placeable::hideTooltip() {
	if (!_tooltip)
		return;

	_tooltip->hide();
}

} // End of namespace NWN

} // End of namespace Engines
