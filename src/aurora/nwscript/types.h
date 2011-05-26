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

/** @file aurora/nwscript/types.h
 *  NWScript types.
 */

#ifndef AURORA_NWSCRIPT_TYPES_H
#define AURORA_NWSCRIPT_TYPES_H

#include <vector>

#include "boost/function.hpp"

namespace Aurora {

namespace NWScript {

enum Type {
	kTypeVoid       = 0,
	kTypeInt           ,
	kTypeFloat         ,
	kTypeString        ,
	kTypeObject        ,
	kTypeVector        , // TODO
	kTypeLocation      , // TODO
	kTypeStruct        , // TODO
	kTypeEngineType      // TODO
};

// Index 0 is the return type, following indices are the arguments
typedef std::vector<Type> Signature;

typedef std::vector<class Variable> Parameters;

typedef boost::function<void (class FunctionContext &ctx)> Function;

} // End of namespace NWScript

} // End of namespace Aurora

#endif // AURORA_NWSCRIPT_TYPES_H
