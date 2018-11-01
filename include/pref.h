// pref.h
//
// Preferences .
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 2002-2010 by Matthias Melcher.
//
//                              FLTK License
//                            December 11, 2001
//
// The FLTK library and included programs are provided under the terms
// of the GNU Library General Public License (LGPL) with the following
// exceptions:
//
//     1. Modifications to the FLTK configure script, config
//        header file, and makefiles by themselves to support
//        a specific platform do not constitute a modified or
//        derivative work.
//
//       The authors do request that such modifications be
//       contributed to the FLTK project - send all contributions
//       through the "Software Trouble Report" on the following page:
//
//            http://www.fltk.org/str.php
//
//     2. Widgets that are subclassed from FLTK widgets do not
//        constitute a derivative work.
//
//     3. Static linking of applications and widgets to the
//        FLTK library does not constitute a derivative work
//        and does not require the author to provide source
//        code for the application or widget, use the shared
//        FLTK libraries, or link their applications or
//        widgets against a user-supplied version of FLTK.
//
//        If you link the application or widget to a modified
//        version of FLTK, then the changes to FLTK must be
//        provided under the terms of the LGPL in sections
//        1, 2, and 4.
//
//     4. You do not have to provide a copy of the FLTK license
//        with programs that are linked to the FLTK library, nor
//        do you have to identify the FLTK license in your
//        program or documentation as required by section 6
//        of the LGPL.
//
//        However, programs must still identify their use of FLTK.
//        The following example statement can be included in user
//        documentation to satisfy this requirement:
//
//            [program/widget] is based in part on the work of
//            the FLTK project (http://www.fltk.org).
//
//     This library is free software; you can redistribute it and/or
//     modify it under the terms of the GNU Library General Public
//     License as published by the Free Software Foundation; either
//     version 2 of the License, or (at your option) any later version.
//
//     This library is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//     Library General Public License for more details.
//
//     You should have received a copy of the GNU Library General Public
//     License along with FLTK.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef Fl_Preferences_H
#define Fl_Preferences_H

#include <stdio.h>

class Fl_Preferences
{

  public:
    enum Root
    {
      SYSTEM = 0,
      USER
    };

    typedef void* ID;

    static const char* newUUID();

    Fl_Preferences( Root root, const char* vendor, const char* application );
    Fl_Preferences( const char* path, const char* vendor, const char* application );
    Fl_Preferences( Fl_Preferences& parent, const char* group );
    Fl_Preferences( Fl_Preferences* parent, const char* group );
    Fl_Preferences( Fl_Preferences& parent, int groupIndex );
    Fl_Preferences( Fl_Preferences* parent, int groupIndex );
    Fl_Preferences(const Fl_Preferences&);
    Fl_Preferences( ID id );
    virtual ~Fl_Preferences();

    ID
    id()
    {
      return (ID)node;
    }

    static char
    remove(ID id_)
    {
      return ((Node*)id_)->remove();
    }

    const char*
    name()
    {
      return node->name();
    }

    const char*
    path()
    {
      return node->path();
    }

    int groups();
    const char* group( int num_group );
    char groupExists( const char* key );
    char deleteGroup( const char* group );
    char deleteAllGroups();

    int entries();
    const char* entry( int index );
    char entryExists( const char* key );
    char deleteEntry( const char* entry );
    char deleteAllEntries();

    char clear();

    char set( const char* entry, int value );
    char set( const char* entry, float value );
    char set( const char* entry, float value, int precision );
    char set( const char* entry, double value );
    char set( const char* entry, double value, int precision );
    char set( const char* entry, const char* value );
    char set( const char* entry, const void* value, int size );

    char get( const char* entry, int& value, int defaultValue );
    char get( const char* entry, float& value,  float defaultValue );
    char get( const char* entry, double& value, double defaultValue );
    char get( const char* entry, char*& value,  const char* defaultValue );
    char get( const char* entry, char* value,   const char* defaultValue,
              int maxSize );
    char get( const char* entry, void*& value,  const void* defaultValue,
              int defaultSize );
    char get( const char* entry, void* value,   const void* defaultValue,
              int defaultSize, int maxSize );

    int size( const char* entry );

    char getUserdataPath( char* path, int pathlen );

    void flush();

    class Name
    {

        char* data_;

      public:
        Name( unsigned int n );
        Name( const char* format, ... );
        operator const char* ()
        {
          return data_;
        }
        ~Name();
    };

    struct Entry
    {
      char* name, *value;
    };

  private:
    Fl_Preferences() : node(0), rootNode(0) { }
    Fl_Preferences& operator=(const Fl_Preferences&);

    static char nameBuffer[128];
    static char uuidBuffer[40];
    static Fl_Preferences* runtimePrefs;

  public:
    class RootNode;

    class Node
    {
        Node* child_, *next_;
        union
        {
          Node* parent_;
          RootNode* root_;
        };
        char* path_;
        Entry* entry_;
        int nEntry_, NEntry_;
        unsigned char dirty_: 1;
        unsigned char top_: 1;
        unsigned char indexed_: 1;
        Node** index_;
        int nIndex_, NIndex_;
        void createIndex();
        void updateIndex();
        void deleteIndex();
      public:
        static int lastEntrySet;
      public:
        Node( const char* path );
        ~Node();
        int write( FILE* f );
        const char* name();
        const char*
        path()
        {
          return path_;
        }
        Node* find( const char* path );
        Node* search( const char* path, int offset = 0 );
        Node* childNode( int ix );
        Node* addChild( const char* path );
        void setParent( Node* parent );
        Node*
        parent()
        {
          return top_ ? 0L : parent_;
        }
        void
        setRoot(RootNode* r)
        {
          root_ = r;
          top_ = 1;
        }
        RootNode* findRoot();
        char remove();
        char dirty();
        void clearDirtyFlags();
        void deleteAllChildren();
        int nChildren();
        const char* child( int ix );
        void set( const char* name, const char* value );
        void set( const char* line );
        void add( const char* line );
        const char* get( const char* name );
        int getEntry( const char* name );
        char deleteEntry( const char* name );
        void deleteAllEntries();
        int
        nEntry()
        {
          return nEntry_;
        }
        Entry&
        entry(int i)
        {
          return entry_[i];
        }
    };
    friend class Node;

    class RootNode
    {
        Fl_Preferences* prefs_;
        char* filename_;
        char* vendor_, *application_;
      public:
        RootNode( Fl_Preferences*, Root root, const char* vendor,
                  const char* application );
        RootNode( Fl_Preferences*, const char* path, const char* vendor,
                  const char* application );
        RootNode( Fl_Preferences* );
        ~RootNode();
        int read();
        int write();
        char getPath( char* path, int pathlen );
    };
    friend class RootNode;

  protected:
    Node* node;
    RootNode* rootNode;
};

#endif // !Fl_Preferences_H
