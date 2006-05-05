/*  upnp_cds_subscriptions.cc - this file is part of MediaTomb.
                                                                                
    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
                       Sergey Bostandzhyan <jin@deadlock.dhs.org>
                                                                                
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
                                                                                
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                                                                                
    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "upnp_cds.h"
#include "upnp_xml.h"
#include "ixml.h"
#include "server.h"

using namespace zmm;
using namespace mxml;

void ContentDirectoryService::process_subscription_request(zmm::Ref<SubscriptionRequest> request)
{
    int err;
    IXML_Document *event = NULL;

    Ref<Element> propset, property;
   
    //log_info(("CDS::process_subscription_request - start\n"));
   
    propset = UpnpXML_CreateEventPropertySet();
    property = propset->getFirstChild();
    property->appendTextChild(_("SystemUpdateID"), _("") + systemUpdateID);

    String xml = propset->print();
    err = ixmlParseBufferEx(xml.c_str(), &event);
    if (err != IXML_SUCCESS)
    {
        throw UpnpException(UPNP_E_SUBSCRIPTION_FAILED, _("Could not convert property set to ixml"));
    }

    UpnpAcceptSubscriptionExt(server->getDeviceHandle(),
                              ConfigManager::getInstance()->getOption(_("/server/udn")).c_str(),
                              serviceID.c_str(), event, request->getSubscriptionID().c_str());

    ixmlDocument_free(event);
    //log_info(("CDS::process_subscription_request - end\n"));
}

void ContentDirectoryService::subscription_update(String containerUpdateIDs_CSV)
{
    int err;
    IXML_Document *event = NULL;

    Ref<Element> propset, property;
    
    //log_info(("CDS::subscription_update - start\n"));

    systemUpdateID++;

    propset = UpnpXML_CreateEventPropertySet();
    property = propset->getFirstChild();
    property->appendTextChild(_("ContainerUpdateIDs"), containerUpdateIDs_CSV); 
    property->appendTextChild(_("SystemUpdateID"), _("") + systemUpdateID);

    String xml = propset->print();

    err = ixmlParseBufferEx(xml.c_str(), &event);
    if (err != IXML_SUCCESS)
    {
        /// \todo add another error code
        throw UpnpException(UPNP_E_SUBSCRIPTION_FAILED, _("Could not convert property set to ixml"));
    }

    UpnpNotifyExt(server->getDeviceHandle(),
                  ConfigManager::getInstance()->getOption(_("/server/udn")).c_str(),
                  serviceID.c_str(), event);

    ixmlDocument_free(event);

    //log_info(("CDS::subscription_update - end\n"));
}
