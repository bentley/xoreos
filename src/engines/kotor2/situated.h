/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  A situated object in a Star Wars: Knights of the Old Republic II - The Sith Lords area.
 */

#ifndef ENGINES_KOTOR2_SITUATED_H
#define ENGINES_KOTOR2_SITUATED_H

#include "src/aurora/types.h"

#include "src/graphics/aurora/types.h"

#include "src/engines/kotor2/object.h"

namespace Engines {

namespace KotOR2 {

class Situated : public Object {
public:
	~Situated();

	// Basic visuals

	void show(); ///< Show the situated object's model.
	void hide(); ///< Hide the situated object's model.

	// Positioning

	/** Set the situated object's position. */
	void setPosition(float x, float y, float z);
	/** Set the situated object's orientation. */
	void setOrientation(float x, float y, float z, float angle);

protected:
	Common::UString _modelName; ///< The model's resource name.

	uint32 _appearanceID; ///< The index within the situated appearance 2DA.

	Graphics::Aurora::Model *_model; ///< The situated object's model.


	Situated(ObjectType type);

	/** Load the situated object from an instance and its blueprint. */
	void load(const Aurora::GFF3Struct &instance, const Aurora::GFF3Struct *blueprint = 0);

	/** Load object-specific properties. */
	virtual void loadObject(const Aurora::GFF3Struct &gff) = 0;
	/** Load appearance-specific properties. */
	virtual void loadAppearance() = 0;


private:
	void loadProperties(const Aurora::GFF3Struct &gff);
	void loadPortrait(const Aurora::GFF3Struct &gff);
};

} // End of namespace KotOR2

} // End of namespace Engines

#endif // ENGINES_KOTOR2_SITUATED_H
