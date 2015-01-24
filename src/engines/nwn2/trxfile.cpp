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

/** @file engines/nwn2/trxfile.cpp
 *  Loader for NWN2 baked terrain files (TRX).
 */

#include "common/error.h"
#include "common/stream.h"

#include "aurora/resman.h"

#include "engines/nwn2/trxfile.h"

namespace Engines {

namespace NWN2 {

TRXFile::TRXFile(const Common::UString &resRef) {
	Common::SeekableReadStream *trx = 0;

	try {
		if (!(trx = ResMan.getResource(resRef, Aurora::kFileTypeTRX)))
			throw Common::Exception("No such TRX");

		load(*trx);
	} catch (Common::Exception &e) {
		delete trx;

		e.add("Failed to load TRX \"%s\"", resRef.c_str());
		throw e;
	}

	delete trx;
}

TRXFile::~TRXFile() {
}

void TRXFile::load(Common::SeekableReadStream &trx) {
	uint32 magic = trx.readUint32BE();
	if (magic != MKTAG('N', 'W', 'N', '2'))
		throw Common::Exception("Invalid magic 0x%08X", magic);

	uint16 versionMajor = trx.readUint16LE();
	uint16 versionMinor = trx.readUint16LE();
	if ((versionMajor != 2) || (versionMinor != 3))
		throw Common::Exception("Invalid version %d.%d", versionMajor, versionMinor);

	uint32 packetCount = trx.readUint32LE();
	if ((uint)(trx.size() - trx.pos()) < (packetCount * 8))
		throw Common::Exception("TRX won't fit the packet packets");

	std::vector<Packet> packets;
	packets.resize(packetCount);

	loadDirectory(trx, packets);
	loadPackets(trx, packets);
}

void TRXFile::loadDirectory(Common::SeekableReadStream &trx, std::vector<Packet> &packets) {
	for (std::vector<Packet>::iterator p = packets.begin(); p != packets.end(); ++p) {
		p->type   = trx.readUint32BE();
		p->offset = trx.readUint32LE();

		if (p->offset >= (uint)trx.size())
			throw Common::Exception("Offset of 0x%08X packet too big (%d)", p->type, p->offset);
	}
}

void TRXFile::loadPackets(Common::SeekableReadStream &trx, std::vector<Packet> &packets) {
	for (std::vector<Packet>::iterator p = packets.begin(); p != packets.end(); ++p) {
		if (!trx.seek(p->offset))
			throw Common::Exception(Common::kSeekError);

		uint32 type = trx.readUint32BE();
		if (type != p->type)
			throw Common::Exception("Packet type mismatch (0x%08X vs 0x%08)", type, p->type);

		p->size = trx.readUint32LE();
		if ((uint)(trx.size() - trx.pos()) < p->size)
			throw Common::Exception("Size of 0x%8X packet too big (%d)", p->type, p->size);

		loadPacket(trx, *p);
	}
}

void TRXFile::loadPacket(Common::SeekableReadStream &trx, Packet &packet) {
	if      (packet.type == MKTAG('T', 'R', 'W', 'H'))
		loadTRWH(trx, packet);
	else if (packet.type == MKTAG('T', 'R', 'R', 'N'))
		loadTRRN(trx, packet);
	else if (packet.type == MKTAG('W', 'A', 'T', 'R'))
		loadWATR(trx, packet);
	else if (packet.type == MKTAG('A', 'S', 'W', 'M'))
		loadASWM(trx, packet);
	else
		throw Common::Exception("Unknown packet type 0x%08X", packet.type);
}

void TRXFile::loadTRWH(Common::SeekableReadStream &trx, Packet &packet) {
	if (packet.size != 12)
		throw Common::Exception("Invalid TRWH size (%d)", packet.size);

	_width  = trx.readUint32LE();
	_height = trx.readUint32LE();

	// trx.readUint32LE(); // Unknown
}

void TRXFile::loadTRRN(Common::SeekableReadStream &trx, Packet &packet) {
	Common::SeekableSubReadStream ttrn(&trx, trx.pos(), trx.pos() + packet.size);

	_terrain.push_back(Terrain());
	Terrain &terrain = _terrain.back();

	terrain.name.readFixedASCII(ttrn, 128);
	for (int i = 0; i < 6; i++)
		terrain.textures[i].readFixedASCII(ttrn, 32);

	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 3; j++)
			terrain.textureColors[i][j] = ttrn.readIEEEFloatLE();

	uint32 vCount = ttrn.readUint32LE();
	uint32 fCount = ttrn.readUint32LE();

	terrain.vertices.resize(vCount);
	for (std::vector<Vertex>::iterator v = terrain.vertices.begin(); v != terrain.vertices.end(); ++v) {
		v->coord[0] = ttrn.readIEEEFloatLE();
		v->coord[1] = ttrn.readIEEEFloatLE();
		v->coord[2] = ttrn.readIEEEFloatLE();

		v->normal[0] = ttrn.readIEEEFloatLE();
		v->normal[1] = ttrn.readIEEEFloatLE();
		v->normal[2] = ttrn.readIEEEFloatLE();

		v->color[0] = ttrn.readByte();
		v->color[1] = ttrn.readByte();
		v->color[2] = ttrn.readByte();
		v->color[3] = ttrn.readByte();

		ttrn.skip(16); // Some texture coordinates?
	}

	terrain.faces.resize(fCount);
	for (std::vector<Face>::iterator f = terrain.faces.begin(); f != terrain.faces.end(); ++f) {
		f->index[0] = ttrn.readUint16LE();
		f->index[1] = ttrn.readUint16LE();
		f->index[2] = ttrn.readUint16LE();

		if ((f->index[0] >= vCount) || ((f->index[1] >= vCount) || (f->index[2] >= vCount)))
			throw Common::Exception("Invalid vertix indices %u, %u, %u", f->index[0], f->index[1], f->index[2]);
	}

	/* TODO:
	 *   - uint32 dds1Size
	 *   - byte  *dds1
	 *   - uint32 dds2Size
	 *   - byte  *dds2
	 *   - uint32 grassCount
	 *   - Grass  grass
	 */
}

void TRXFile::loadWATR(Common::SeekableReadStream &UNUSED(trx), Packet &UNUSED(packet)) {
}

void TRXFile::loadASWM(Common::SeekableReadStream &UNUSED(trx), Packet &UNUSED(packet)) {
}

} // End of namespace NWN2

} // End of namespace Engines