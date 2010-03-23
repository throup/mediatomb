/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    dictionary.h - this file is part of MediaTomb.
    
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

/// \file dictionary.h
/// \brief Definiton of the DictionaryElement and Dictionary classes.
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "zmmf/zmmf.h"
#include "sync.h"

/// \brief This class should never be used directly, it is being used by the Dictionary class.
class DictionaryElement : public zmm::Object
{
public:
    /// \brief Constructor, stores the key and the value.
    DictionaryElement(zmm::String key, zmm::String value);

    /// \brief Changes the name of the key.
    /// \param key new key name.
    void setKey(zmm::String key);

    /// \brief Changes the key value.
    /// \param value new value.
    void setValue(zmm::String value);

    /// \brief Returns the key for this DictionaryElement.
    zmm::String getKey();

    /// \brief Returns the value for this DictionaryElement.
    zmm::String getValue();

protected:
    zmm::String key;
    zmm::String value;
};

/// \brief This class stores key:value pairs of String data and provides functions to access them.
class Dictionary : public zmm::Object
{
protected:
    /// \brief Array of DictionaryElements, representing our Dictionary.
    zmm::Ref<zmm::Array<DictionaryElement> > elements;
    /// \brief Allow to specify encoding separators
    zmm::String _encode(char sep1, char sep2);
public:

    /// \brief Constructor, initializes the dictionary.
    Dictionary();

    /// \brief Adds a new key:value pair to the dictionary.
    void put(zmm::String key, zmm::String value);

    /// \brief Returns the value for a given key.
    zmm::String get(zmm::String key);

    /// \brief Returns the number of elements in the dictinary.
    int size();
    
    /// \brief Deletes a key value pair
    void remove(zmm::String key);
    
    /// \brief Returns an url encoded version of the whole dictionary.
    zmm::String encode();

    /// \brief It seems that a lot of devices can not cope with a param=value
    /// encoded URL, so we will use a simplified encoding scheme.
    zmm::String encodeSimple();

    /// \brief Removes all elements from the dictionary.
    void clear();
    
    /// \brief Makes a dictionary out of url encoded data.
    void decode(zmm::String url);

    /// \brief Makes a dictionary out of simplified url encoded data.
    void decodeSimple(zmm::String url);

    /// \brief Makes a shallow copy of the dictionary
    zmm::Ref<Dictionary> clone();

    /// \brief Merge dictionary with another dictionary. If keys with the same
    /// name already exist, the existing ones will be overwritten.
    void merge(zmm::Ref<Dictionary> other);

    /// \brief returns true if the dictionary is a subset of another dictionary.#
    bool isSubsetOf(zmm::Ref<Dictionary> other);

    /// \brief checks two dictionaries for equality
    bool equals(zmm::Ref<Dictionary> other);

    zmm::Ref<zmm::Array<DictionaryElement> > getElements();

    /// \brief Frees unnecessary memory
    inline void optimize() { elements->optimize(); }
};


/// \brief Reentrant version of the dictionary
class Dictionary_r : public Dictionary
{
public:
    Dictionary_r() : Dictionary()
    {
        mutex = zmm::Ref<Mutex>(new Mutex(true));
    }
    
    inline void put(zmm::String key, zmm::String value)
    {
        mutex->lock();
        Dictionary::put(key, value);
        mutex->unlock();
    }
    
    inline zmm::String get(zmm::String key)
    {
        mutex->lock();
        zmm::String ret = Dictionary::get(key);
        mutex->unlock();
        return ret;
    }
    
    inline void remove(zmm::String key)
    {
        mutex->lock();
        Dictionary::remove(key);
        mutex->unlock();
    }
    
    inline zmm::String encode()
    {
        mutex->lock();
        zmm::String ret = Dictionary::encode();
        mutex->unlock();
        return ret;
    }
    
    inline void clear()
    {
        mutex->lock();
        Dictionary::clear();
        mutex->unlock();
    }
    
    inline void decode(zmm::String url)
    {
        mutex->lock();
        Dictionary::decode(url);
        mutex->unlock();
    }
    
    inline zmm::Ref<Dictionary_r> clone()
    {
        mutex->lock();
        zmm::Ref<Dictionary_r> ret = RefCast(Dictionary::clone(), Dictionary_r);
        mutex->unlock();
        return ret;
    }
    
    inline bool isSubsetOf(zmm::Ref<Dictionary> other)
    {
        mutex->lock();
        bool ret = Dictionary::isSubsetOf(other);
        mutex->unlock();
        return ret;
    }
    
    inline bool equals(zmm::Ref<Dictionary> other)
    {
        mutex->lock();
        bool ret = Dictionary::equals(other);
        mutex->unlock();
        return ret;
    }                                                                         
    
    inline zmm::Ref<zmm::Array<DictionaryElement> > getElements()
    {
        mutex->lock();
        zmm::Ref<zmm::Array<DictionaryElement> > ret = Dictionary::getElements();
        mutex->unlock();
        return ret;
    }
    
    inline void optimize()
    {
        mutex->lock();
        Dictionary::optimize();
        mutex->unlock();
    }
    
protected:
    zmm::Ref<Mutex> mutex;
};

#endif // __DICTIONARY_H__
