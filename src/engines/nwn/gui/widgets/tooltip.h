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
 *  A tooltip.
 */

#ifndef ENGINES_NWN_GUI_WIDGETS_TOOLTIP_H
#define ENGINES_NWN_GUI_WIDGETS_TOOLTIP_H

#include "src/common/ustring.h"
#include "src/common/mutex.h"

#include "src/graphics/aurora/types.h"

#include "src/events/notifyable.h"

#include "src/events/timerman.h"

namespace Engines {

class Widget;

namespace NWN {

class Portrait;

/** A tooltip. */
class Tooltip : public Events::Notifyable {
public:
	enum Type {
		kTypeHelp,
		kTypeFeedback
	};

	Tooltip(Type type);
	Tooltip(Type type, Widget &parent);
	Tooltip(Type type, Graphics::Aurora::Model &parent);
	~Tooltip();

	void clearLines();
	void clearPortrait();

	void addLine(const Common::UString &text, float r, float g, float b, float a);
	void setPortrait(const Common::UString &image);

	void setAlign(float align);

	void updatePosition();
	void setPosition(float x, float y, float z);

	void show();
	void hide();


protected:
	void notifyCameraMoved();


private:
	struct Line {
		float r, g, b, a;
		Common::UString line;

		Graphics::Aurora::Text *text;
	};

	Type _type;

	Widget *_parentWidget;
	Graphics::Aurora::Model *_parentModel;

	bool _empty;
	bool _visible;

	float _align;

	bool _showText;
	bool _showBubble;
	bool _showPortrait;

	Graphics::Aurora::Model *_bubble;

	Portrait *_portrait;

	std::vector<Line> _lines;

	bool _offscreen;

	float _x;
	float _y;
	float _z;

	float _lineHeight;
	float _lineSpacing;

	float _width;
	float _height;

	Common::UString _font;

	Events::TimerHandle _timer;

	bool _needCamera;
	bool _detectEdge;

	Common::Mutex _mutex;


	void getSize(float &width, float &height);

	void checkEmpty();
	void redoLines();
	void redoBubble();
	void redoLayout();

	uint32 doShow(uint32 oldInterval);

	void doShow();
	void doHide();

	void getFeedbackMode(bool &showBubble, bool &showText, bool &showPortrait) const;

	bool getParentPosition(float &x, float &y, float &z) const;

	static Common::UString getFontName();
	static Common::UString getBubbleModel(uint32 lines, float width);
};

} // End of namespace NWN

} // End of namespace Engines

#endif // ENGINES_NWN_GUI_WIDGETS_TOOLTIP_H
