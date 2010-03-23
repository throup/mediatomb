/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    url_request_handler.h - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.cc>,
                       Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>
    
    Copyright (C) 2006-2010 Gena Batyan <bgeradz@mediatomb.cc>,
                            Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>,
                            Leonhard Wimmer <leo@mediatomb.cc>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    version 2 along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
    
    $Id$
*/

/// \file url_request_handler.h
/// \brief Definition of the URLRequestHandler class.
#ifndef __URL_REQUEST_HANDLER_H__
#define __URL_REQUEST_HANDLER_H__

#include "common.h"
#include "request_handler.h"
#include "dictionary.h"

class URLRequestHandler : public RequestHandler
{
public:
    URLRequestHandler();
    virtual void get_info(IN const char *filename, OUT struct File_Info *info);
    virtual zmm::Ref<IOHandler> open(IN const char *filename, 
                                     OUT struct File_Info *info, 
                                     IN enum UpnpOpenFileMode mode);
};

#endif // __URL_REQUEST_HANDLER_H__
