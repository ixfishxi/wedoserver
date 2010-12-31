////////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
////////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////

#include "otpch.h"
#include "mounts.h"

bool Mount::isTamed(Player* player) const
{
        if(!player)
                return false;
                
	if(player->hasCustomFlag(PlayerCustomFlag_CanUseAllMounts))
                return true;


        uint8_t tmpId = id - 1;

        std::string value = "";
	int key = PSTRG_MOUNTS_RANGE_START + (tmpId / 31);
	if(!player->getStorage(boost::lexical_cast<std::string>(key), value))
                return false;

        int32_t tmp = static_cast<int32_t>(pow(2, tmpId % 31));
        return (tmp & atoi(value.c_str())) == tmp;
}
void Mounts::clear()
{
	for(MountList::iterator it = mounts.begin(); it != mounts.end(); it++)
		delete *it;

	mounts.clear();
}

bool Mounts::reload()
{
	clear();
	return loadFromXml();
}

bool Mounts::loadFromXml()
{
	xmlDocPtr doc = xmlParseFile(getFilePath(FILE_TYPE_XML, "mounts.xml").c_str());
	if(!doc)
	{
		std::clog << "[Warning - Mounts::loadFromXml] Cannot load mounts file." << std::endl;
		std::clog << getLastXMLError() << std::endl;
		return false;
	}

	xmlNodePtr p, root = xmlDocGetRootElement(doc);
	if(xmlStrcmp(root->name,(const xmlChar*)"mounts"))
	{
		std::clog << "[Error - Mounts::loadFromXml] Malformed mounts file." << std::endl;
		xmlFreeDoc(doc);
		return false;
	}

	p = root->children;
	while(p)
	{
		parseMountNode(p);
		p = p->next;
	}

	xmlFreeDoc(doc);
	return true;
}

bool Mounts::parseMountNode(xmlNodePtr p)
{
	if(xmlStrcmp(p->name, (const xmlChar*)"mount"))
		return false;

	int32_t intValue;
	std::string strValue;

	uint8_t mountId = 0;
	if(readXMLInteger(p, "id", intValue))
		mountId = intValue;

	std::string name;
	if(readXMLString(p, "name", strValue))
		name = strValue;

	uint16_t clientId = 0;
	if(readXMLInteger(p, "clientid", intValue))
		clientId = intValue;

	int32_t speed = 0;
	if(readXMLInteger(p, "speed", intValue))
		speed = intValue;

	if(!clientId || !mountId) {
		std::clog << "[Error - Mounts::parseMountNode] Entry without clientId and/or mountId" << std::endl;
		return false;
	}
	Mount* mount = new Mount(name, mountId, clientId, speed);
	if(!mount)
		return false;

	mounts.push_back(mount);

	mountCount++;
	return true;
}
Mount* Mounts::getMountById(uint16_t id) const
{
	if(id)
		for(MountList::const_iterator it = mounts.begin(); it != mounts.end(); it++)
		{
			if((*it)->getId() == id)
				return (*it);
		}

	return NULL;
}
Mount* Mounts::getMountByCid(uint16_t id) const
{
	if(id)
		for(MountList::const_iterator it = mounts.begin(); it != mounts.end(); it++)
		{
			if((*it)->getClientId() == id)
				return (*it);
		}

	return NULL;
}
