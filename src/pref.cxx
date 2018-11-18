// pref.cxx
//
// Preferences methods for the Fast Light Tool Kit (FLTK).
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
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "pref.h"
#include "drvsys.h"
#include "fl.h"
#include "filename.h"
#include "flstring.h"
#include "fl_sys.h"
#include "fl_utf8.h"


char Fl_Preferences::nameBuffer[128];
char Fl_Preferences::uuidBuffer[40];
Fl_Preferences* Fl_Preferences::runtimePrefs = 0;

const char*
Fl_Preferences::newUUID()
{
  Fl::system_driver()->newUUID(uuidBuffer);
  return uuidBuffer;
}

Fl_Preferences::Fl_Preferences( Root root, const char* vendor,
                                const char* application )
{
  node = new Node( "." );
  rootNode = new RootNode( this, root, vendor, application );
  node->setRoot(rootNode);
}

Fl_Preferences::Fl_Preferences( const char* path, const char* vendor,
                                const char* application )
{
  node = new Node( "." );
  rootNode = new RootNode( this, path, vendor, application );
  node->setRoot(rootNode);
}

Fl_Preferences::Fl_Preferences( Fl_Preferences& parent, const char* group )
{
  rootNode = parent.rootNode;
  node = parent.node->addChild( group );
}

Fl_Preferences::Fl_Preferences( Fl_Preferences* parent, const char* group )
{
  if (parent == 0)
  {
    if (!runtimePrefs)
    {
      runtimePrefs = new Fl_Preferences();
      runtimePrefs->node = new Node( "." );
      runtimePrefs->rootNode = new RootNode( runtimePrefs );
      runtimePrefs->node->setRoot(rootNode);
    }

    parent = runtimePrefs;
  }

  rootNode = parent->rootNode;
  node = parent->node->addChild( group );
}

Fl_Preferences::Fl_Preferences( Fl_Preferences& parent, int groupIndex )
{
  rootNode = parent.rootNode;

  if (groupIndex < 0 || groupIndex >= parent.groups())
  {
    node = parent.node->addChild( newUUID() );
  }

  else
  {
    node = parent.node->childNode( groupIndex );
  }
}

Fl_Preferences::Fl_Preferences( Fl_Preferences* parent, int groupIndex )
{
  rootNode = parent->rootNode;

  if (groupIndex < 0 || groupIndex >= parent->groups())
  {
    node = parent->node->addChild( newUUID() );
  }

  else
  {
    node = parent->node->childNode( groupIndex );
  }
}

Fl_Preferences::Fl_Preferences( Fl_Preferences::ID id )
{
  node = (Node*)id;
  rootNode = node->findRoot();
}

Fl_Preferences::Fl_Preferences(const Fl_Preferences& rhs)
  : node(rhs.node),
    rootNode(rhs.rootNode)
{ }

Fl_Preferences&
Fl_Preferences::operator=(const Fl_Preferences& rhs)
{
  if (&rhs != this)
  {
    node = rhs.node;
    rootNode = rhs.rootNode;
  }

  return *this;
}

Fl_Preferences::~Fl_Preferences()
{
  if (node && !node->parent()) delete rootNode;

  node = 0L;
  rootNode = 0L;
}

int
Fl_Preferences::groups()
{
  return node->nChildren();
}

const char*
Fl_Preferences::group( int num_group )
{
  return node->child( num_group );
}

char
Fl_Preferences::groupExists( const char* key )
{
  return node->search( key ) ? 1 : 0 ;
}

char
Fl_Preferences::deleteGroup( const char* group )
{
  Node* nd = node->search( group );

  if ( nd ) return nd->remove();

  return 0;
}

char
Fl_Preferences::deleteAllGroups()
{
  node->deleteAllChildren();
  return 1;
}

int
Fl_Preferences::entries()
{
  return node->nEntry();
}

const char*
Fl_Preferences::entry( int index )
{
  return node->entry(index).name;
}

char
Fl_Preferences::entryExists( const char* key )
{
  return node->getEntry( key ) >= 0 ? 1 : 0 ;
}

char
Fl_Preferences::deleteEntry( const char* key )
{
  return node->deleteEntry( key );
}

char
Fl_Preferences::deleteAllEntries()
{
  node->deleteAllEntries();
  return 1;
}

char
Fl_Preferences::clear()
{
  char ret1 = deleteAllGroups();
  char ret2 = deleteAllEntries();
  return ret1 & ret2;
}

char
Fl_Preferences::get( const char* key, int& value, int defaultValue )
{
  const char* v = node->get( key );
  value = v ? atoi( v ) : defaultValue;
  return ( v != 0 );
}

char
Fl_Preferences::set( const char* key, int value )
{
  sprintf( nameBuffer, "%d", value );
  node->set( key, nameBuffer );
  return 1;
}

char
Fl_Preferences::get( const char* key, float& value, float defaultValue )
{
  const char* v = node->get( key );
  value = v ? (float)atof( v ) : defaultValue;
  return ( v != 0 );
}

char
Fl_Preferences::set( const char* key, float value )
{
  sprintf( nameBuffer, "%g", value );
  node->set( key, nameBuffer );
  return 1;
}

char
Fl_Preferences::set( const char* key, float value, int precision )
{
  sprintf( nameBuffer, "%.*g", precision, value );
  node->set( key, nameBuffer );
  return 1;
}

char
Fl_Preferences::get( const char* key, double& value, double defaultValue )
{
  const char* v = node->get( key );
  value = v ? atof( v ) : defaultValue;
  return ( v != 0 );
}

char
Fl_Preferences::set( const char* key, double value )
{
  sprintf( nameBuffer, "%g", value );
  node->set( key, nameBuffer );
  return 1;
}

char
Fl_Preferences::set( const char* key, double value, int precision )
{
  sprintf( nameBuffer, "%.*g", precision, value );
  node->set( key, nameBuffer );
  return 1;
}

static char*
decodeText( const char* src )
{
  int len = 0;
  const char* s = src;

  for ( ; *s; s++, len++ )
  {
    if ( *s == '\\' )
    {
      if ( isdigit( s[1] ) )
      {
        s += 3;
      }

      else
      {
        s += 1;
      }
    }
  }

  char* dst = (char*)malloc( len + 1 ), *d = dst;

  for ( s = src; *s; s++ )
  {
    char c = *s;

    if ( c == '\\' )
    {
      if ( s[1] == '\\' )
      {
        *d++ = c;
        s++;
      }

      else if ( s[1] == 'n' )
      {
        *d++ = '\n';
        s++;
      }

      else if ( s[1] == 'r' )
      {
        *d++ = '\r';
        s++;
      }

      else if ( isdigit( s[1] ) )
      {
        *d++ = ((s[1] - '0') << 6) + ((s[2] - '0') << 3) + (s[3] - '0');
        s += 3;
      }

      else s++;
    }

    else
      *d++ = c;
  }

  *d = 0;
  return dst;
}

char
Fl_Preferences::get( const char* key, char* text, const char* defaultValue,
                     int maxSize )
{
  const char* v = node->get( key );

  if ( v && strchr( v, '\\' ) )
  {
    char* w = decodeText( v );
    strlcpy(text, w, maxSize);
    free( w );
    return 1;
  }

  if ( !v ) v = defaultValue;

  if ( v ) strlcpy(text, v, maxSize);

  else *text = 0;

  return ( v != defaultValue );
}

char
Fl_Preferences::get( const char* key, char*& text, const char* defaultValue )
{
  const char* v = node->get( key );

  if ( v && strchr( v, '\\' ) )
  {
    text = decodeText( v );
    return 1;
  }

  if ( !v ) v = defaultValue;

  if ( v )
    text = strdup( v );

  else
    text = 0;

  return ( v != defaultValue );
}

char
Fl_Preferences::set( const char* key, const char* text )
{
  const char* s = text ? text : "";
  int n = 0, ns = 0;

  for ( ; *s; s++ )
  {
    n++;

    if ( *s < 32 || *s == '\\' || *s == 0x7f ) ns += 4;
  }

  if ( ns )
  {
    char* buffer = (char*)malloc( n + ns + 1 ), *d = buffer;

    for ( s = text; *s; )
    {
      char c = *s;

      if ( c == '\\' )
      {
        *d++ = '\\';
        *d++ = '\\';
        s++;
      }

      else if ( c == '\n' )
      {
        *d++ = '\\';
        *d++ = 'n';
        s++;
      }

      else if ( c == '\r' )
      {
        *d++ = '\\';
        *d++ = 'r';
        s++;
      }

      else if ( c < 32 || c == 0x7f )
      {
        *d++ = '\\';
        *d++ = '0' + ((c >> 6) & 3);
        *d++ = '0' + ((c >> 3) & 7);
        *d++ = '0' + (c & 7);
        s++;
      }

      else *d++ = *s++;
    }

    *d = 0;
    node->set( key, buffer );
    free( buffer );
  }

  else
    node->set( key, text );

  return 1;
}

static void*
decodeHex( const char* src, int& size )
{
  size = (int) strlen( src ) / 2;
  unsigned char* data = (unsigned char*)malloc( size ), *d = data;
  const char* s = src;

  for ( int i = size; i > 0; i-- )
  {
    int v;
    char x = tolower(*s++);

    if ( x >= 'a' ) v = x - 'a' + 10;

    else v = x - '0';

    v = v << 4;
    x = tolower(*s++);

    if ( x >= 'a' ) v += x - 'a' + 10;

    else v += x - '0';

    *d++ = (unsigned char)v;
  }

  return (void*)data;
}

char
Fl_Preferences::get( const char* key, void* data, const void* defaultValue,
                     int defaultSize, int maxSize )
{
  const char* v = node->get( key );

  if ( v )
  {
    int dsize;
    void* w = decodeHex( v, dsize );
    memmove( data, w, dsize > maxSize ? maxSize : dsize );
    free( w );
    return 1;
  }

  if ( defaultValue )
    memmove( data, defaultValue, defaultSize > maxSize ? maxSize : defaultSize );

  return 0;
}

char
Fl_Preferences::get( const char* key, void*& data, const void* defaultValue,
                     int defaultSize )
{
  const char* v = node->get( key );

  if ( v )
  {
    int dsize;
    data = decodeHex( v, dsize );
    return 1;
  }

  if ( defaultValue )
  {
    data = (void*)malloc( defaultSize );
    memmove( data, defaultValue, defaultSize );
  }

  else
    data = 0;

  return 0;
}

char
Fl_Preferences::set( const char* key, const void* data, int dsize )
{
  char* buffer = (char*)malloc( dsize * 2 + 1 ), *d = buffer;;
  unsigned char* s = (unsigned char*)data;

  for ( ; dsize > 0; dsize-- )
  {
    static char lu[] = "0123456789abcdef";
    unsigned char v = *s++;
    *d++ = lu[v >> 4];
    *d++ = lu[v & 0xf];
  }

  *d = 0;
  node->set( key, buffer );
  free( buffer );
  return 1;
}

int
Fl_Preferences::size( const char* key )
{
  const char* v = node->get( key );
  return (int) (v ? strlen( v ) : 0);
}

char
Fl_Preferences::getUserdataPath( char* path, int pathlen )
{
  if ( rootNode )
    return rootNode->getPath( path, pathlen );

  return 0;
}

void
Fl_Preferences::flush()
{
  if ( rootNode && node->dirty() )
    rootNode->write();
}

Fl_Preferences::Name::Name( unsigned int n )
{
  data_ = (char*)malloc(20);
  sprintf(data_, "%u", n);
}

Fl_Preferences::Name::Name( const char* format, ... )
{
  data_ = (char*)malloc(1024);
  va_list args;
  va_start(args, format);
  vsnprintf(data_, 1024, format, args);
  va_end(args);
}

Fl_Preferences::Name::~Name()
{
  if (data_)
  {
    free(data_);
    data_ = 0L;
  }
}

int Fl_Preferences::Node::lastEntrySet = -1;

Fl_Preferences::RootNode::RootNode( Fl_Preferences* prefs, Root root,
                                    const char* vendor, const char* application )
  : prefs_(prefs),
    filename_(0L),
    vendor_(0L),
    application_(0L)
{

  char* filename = Fl::system_driver()->preference_rootnode(prefs, root, vendor,
                                                            application);
  filename_    = filename ? strdup(filename) : 0L;
  vendor_      = strdup(vendor);
  application_ = strdup(application);
  read();
}

Fl_Preferences::RootNode::RootNode( Fl_Preferences* prefs, const char* path,
                                    const char* vendor, const char* application )
  : prefs_(prefs),
    filename_(0L),
    vendor_(0L),
    application_(0L)
{

  if (!vendor)
    vendor = "unknown";

  if (!application)
  {
    application = "unknown";
    filename_ = strdup(path);
  }

  else
  {
    char filename[ FL_PATH_MAX ];
    filename[0] = 0;
    snprintf(filename, sizeof(filename), "%s/%s.prefs", path, application);
    filename_  = strdup(filename);
  }

  vendor_      = strdup(vendor);
  application_ = strdup(application);
  read();
}

Fl_Preferences::RootNode::RootNode( Fl_Preferences* prefs )
  : prefs_(prefs),
    filename_(0L),
    vendor_(0L),
    application_(0L)
{
}

Fl_Preferences::RootNode::~RootNode()
{
  if ( prefs_->node->dirty() )
    write();

  if ( filename_ )
  {
    free( filename_ );
    filename_ = 0L;
  }

  if ( vendor_ )
  {
    free( vendor_ );
    vendor_ = 0L;
  }

  if ( application_ )
  {
    free( application_ );
    application_ = 0L;
  }

  delete prefs_->node;
  prefs_->node = 0L;
}

int
Fl_Preferences::RootNode::read()
{
  if (!filename_)
    return -1;

  char buf[1024];
  FILE* f = Fl::fopen( filename_, "rb" );

  if ( !f )
    return -1;

  if (fgets( buf, 1024, f ) == 0) { /* ignore */ }

  if (fgets( buf, 1024, f ) == 0) { /* ignore */ }

  if (fgets( buf, 1024, f ) == 0) { /* ignore */ }

  Node* nd = prefs_->node;

  for (;;)
  {
    if ( !fgets( buf, 1024, f ) ) break;

    if ( buf[0] == '[' )
    {
      size_t end = strcspn( buf + 1, "]\n\r" );
      buf[ end + 1 ] = 0;
      nd = prefs_->node->find( buf + 1 );
    }

    else if ( buf[0] == '+' )
    {
      size_t end = strcspn( buf + 1, "\n\r" );

      if ( end != 0 )
      {
        buf[ end + 1 ] = 0;
        nd->add( buf + 1 );
      }
    }

    else
    {
      size_t end = strcspn( buf, "\n\r" );

      if ( end != 0 )
      {
        buf[ end ] = 0;
        nd->set( buf );
      }
    }
  }

  fclose( f );
  prefs_->node->clearDirtyFlags();
  return 0;
}

int
Fl_Preferences::RootNode::write()
{
  if (!filename_)
    return -1;

  Fl::make_path_for_file(filename_);
  FILE* f = Fl::fopen( filename_, "wb" );

  if ( !f )
    return -1;

  fprintf( f, "; FLTK preferences file format 1.0\n" );
  fprintf( f, "; vendor: %s\n", vendor_ );
  fprintf( f, "; application: %s\n", application_ );
  prefs_->node->write( f );
  fclose( f );

  if (Fl::system_driver()->preferences_need_protection_check())
  {
    if (strncmp(filename_, "/etc/fltk/", 10) == 0)
    {
      char* p;
      p = filename_ + 9;

      do
      {
        *p = 0;
        Fl::chmod(filename_, 0755);
        *p = '/';
        p = strchr(p + 1, '/');
      }
      while (p);

      Fl::chmod(filename_, 0644);
    }
  }

  return 0;
}

char
Fl_Preferences::RootNode::getPath( char* path, int pathlen )
{
  if (!filename_)
    return 1;

  if (pathlen <= 0)
    return 1;

  strlcpy( path, filename_, pathlen);

  char* name = 0L, *ext = 0L;

  {
    char* s;

    for ( s = path; *s; s++ ) if ( *s == '\\' ) *s = '/';
  }

  name = strrchr( path, '/' );

  if (name)
    name++;

  else
    name = path;

  ext = strrchr( path, '.' );

  if ( (ext == 0L) || (ext < name) )
  {
    if (strlen(name) == 0)
    {
      strlcat( path, "data", pathlen );
    }

    else
    {
      strlcat( path, ".data", pathlen );
    }
  }

  else
  {
    *ext = 0;
  }

  char ret = Fl::make_path( path );

  if (Fl::system_driver()->preferences_need_protection_check()
      && strncmp(path, "/etc/fltk/", 10) == 0)
  {
    Fl::chmod(path, 0755);
  }

  strlcat( path, "/", pathlen );
  return ret;
}

Fl_Preferences::Node::Node( const char* path )
{
  if ( path ) path_ = strdup( path );

  else path_ = 0;

  child_ = 0;
  next_ = 0;
  parent_ = 0;
  entry_ = 0;
  nEntry_ = NEntry_ = 0;
  dirty_ = 0;
  top_ = 0;
  indexed_ = 0;
  index_ = 0;
  nIndex_ = NIndex_ = 0;
}

void
Fl_Preferences::Node::deleteAllChildren()
{
  Node* nx;

  for ( Node* nd = child_; nd; nd = nx )
  {
    nx = nd->next_;
    delete nd;
  }

  child_ = 0L;
  dirty_ = 1;
  updateIndex();
}

void
Fl_Preferences::Node::deleteAllEntries()
{
  if ( entry_ )
  {
    for ( int i = 0; i < nEntry_; i++ )
    {
      if ( entry_[i].name )
      {
        free( entry_[i].name );
        entry_[i].name = 0L;
      }

      if ( entry_[i].value )
      {
        free( entry_[i].value );
        entry_[i].value = 0L;
      }
    }

    free( entry_ );
    entry_ = 0L;
    nEntry_ = 0;
    NEntry_ = 0;
  }

  dirty_ = 1;
}

Fl_Preferences::Node::~Node()
{
  deleteAllChildren();
  deleteAllEntries();
  deleteIndex();

  if ( path_ )
  {
    free( path_ );
    path_ = 0L;
  }

  next_ = 0L;
  parent_ = 0L;
}

char
Fl_Preferences::Node::dirty()
{
  if ( dirty_ ) return 1;

  if ( next_ && next_->dirty() ) return 1;

  if ( child_ && child_->dirty() ) return 1;

  return 0;
}

void
Fl_Preferences::Node::clearDirtyFlags()
{
  Fl_Preferences::Node* nd = this;

  while (nd)
  {
    nd->dirty_ = 0;

    if ( nd->child_ ) nd->child_->clearDirtyFlags();

    nd = nd->next_;
  }
}

int
Fl_Preferences::Node::write( FILE* f )
{
  if ( next_ ) next_->write( f );

  fprintf( f, "\n[%s]\n\n", path_ );

  for ( int i = 0; i < nEntry_; i++ )
  {
    char* src = entry_[i].value;

    if ( src )
    {
      fprintf( f, "%s:", entry_[i].name );
      size_t cnt, written = 0;

      for ( cnt = 0; cnt < 60; cnt++ )
        if ( src[cnt] == 0 ) break;

      written += fwrite( src, cnt, 1, f );
      fprintf( f, "\n" );
      src += cnt;

      for (; *src;)
      {
        for ( cnt = 0; cnt < 80; cnt++ )
          if ( src[cnt] == 0 ) break;

        fputc( '+', f );
        written += fwrite( src, cnt, 1, f );
        fputc( '\n', f );
        src += cnt;
      }
    }

    else
      fprintf( f, "%s\n", entry_[i].name );
  }

  if ( child_ ) child_->write( f );

  dirty_ = 0;
  return 0;
}

void
Fl_Preferences::Node::setParent( Node* pn )
{
  parent_ = pn;
  next_ = pn->child_;
  pn->child_ = this;
  sprintf( nameBuffer, "%s/%s", pn->path_, path_ );
  free( path_ );
  path_ = strdup( nameBuffer );
}

Fl_Preferences::RootNode*
Fl_Preferences::Node::findRoot()
{
  Node* n = this;

  do
  {
    if (n->top_)
      return n->root_;

    n = n->parent();
  }
  while (n);

  return 0L;
}

Fl_Preferences::Node*
Fl_Preferences::Node::addChild( const char* path )
{
  sprintf( nameBuffer, "%s/%s", path_, path );
  char* name = strdup( nameBuffer );
  Node* nd = find( name );
  free( name );
  updateIndex();
  return nd;
}

void
Fl_Preferences::Node::set( const char* name, const char* value )
{
  for ( int i = 0; i < nEntry_; i++ )
  {
    if ( strcmp( name, entry_[i].name ) == 0 )
    {
      if ( !value ) return;

      if ( strcmp( value, entry_[i].value ) != 0 )
      {
        if ( entry_[i].value )
          free( entry_[i].value );

        entry_[i].value = strdup( value );
        dirty_ = 1;
      }

      lastEntrySet = i;
      return;
    }
  }

  if ( NEntry_ == nEntry_ )
  {
    NEntry_ = NEntry_ ? NEntry_ * 2 : 10;
    entry_ = (Entry*)realloc( entry_, NEntry_ * sizeof(Entry) );
  }

  entry_[ nEntry_ ].name = strdup( name );
  entry_[ nEntry_ ].value = value ? strdup( value ) : 0;
  lastEntrySet = nEntry_;
  nEntry_++;
  dirty_ = 1;
}

void
Fl_Preferences::Node::set( const char* line )
{
  char dirt = dirty_;

  if ( line[0] == ';' || line[0] == 0 || line[0] == '#' )
  {
    set( line, 0 );
  }

  else
  {
    const char* c = strchr( line, ':' );

    if ( c )
    {
      size_t len = c - line + 1;

      if ( len >= sizeof( nameBuffer ) )
        len = sizeof( nameBuffer );

      strlcpy( nameBuffer, line, len );
      set( nameBuffer, c + 1 );
    }

    else
    {
      set( line, "" );
    }
  }

  dirty_ = dirt;
}

void
Fl_Preferences::Node::add( const char* line )
{
  if ( lastEntrySet < 0 || lastEntrySet >= nEntry_ ) return;

  char*& dst = entry_[ lastEntrySet ].value;
  size_t a = strlen( dst );
  size_t b = strlen( line );
  dst = (char*)realloc( dst, a + b + 1 );
  memcpy( dst + a, line, b + 1 );
}

const char*
Fl_Preferences::Node::get( const char* name )
{
  int i = getEntry( name );
  return i >= 0 ? entry_[i].value : 0 ;
}

int
Fl_Preferences::Node::getEntry( const char* name )
{
  for ( int i = 0; i < nEntry_; i++ )
  {
    if ( strcmp( name, entry_[i].name ) == 0 )
    {
      return i;
    }
  }

  return -1;
}

char
Fl_Preferences::Node::deleteEntry( const char* name )
{
  int ix = getEntry( name );

  if ( ix == -1 ) return 0;

  memmove( entry_ + ix, entry_ + ix + 1, (nEntry_ - ix - 1) * sizeof(Entry) );
  nEntry_--;
  dirty_ = 1;
  return 1;
}

Fl_Preferences::Node*
Fl_Preferences::Node::find( const char* path )
{
  int len = (int) strlen( path_ );

  if ( strncmp( path, path_, len ) == 0 )
  {
    if ( path[ len ] == 0 )
      return this;

    if ( path[ len ] == '/' )
    {
      Node* nd;

      for ( nd = child_; nd; nd = nd->next_ )
      {
        Node* nn = nd->find( path );

        if ( nn ) return nn;
      }

      const char* s = path + len + 1;
      const char* e = strchr( s, '/' );

      if (e) strlcpy( nameBuffer, s, e - s + 1 );

      else strlcpy( nameBuffer, s, sizeof(nameBuffer));

      nd = new Node( nameBuffer );
      nd->setParent( this );
      dirty_ = 1;
      return nd->find( path );
    }
  }

  return 0;
}

Fl_Preferences::Node*
Fl_Preferences::Node::search( const char* path, int offset )
{
  if ( offset == 0 )
  {
    if ( path[0] == '.' )
    {
      if ( path[1] == 0 )
      {
        return this;
      }

      else if ( path[1] == '/' )
      {
        Node* nn = this;

        while ( nn->parent() ) nn = nn->parent();

        if ( path[2] == 0 )
        {
          return nn;
        }

        return nn->search( path + 2, 2 );
      }
    }

    offset = (int) strlen( path_ ) + 1;
  }

  int len = (int) strlen( path_ );

  if ( len < offset - 1 ) return 0;

  len -= offset;

  if ( ( len <= 0 ) || ( strncmp( path, path_ + offset, len ) == 0 ) )
  {
    if ( len > 0 && path[ len ] == 0 )
      return this;

    if ( len <= 0 || path[ len ] == '/' )
    {
      for ( Node* nd = child_; nd; nd = nd->next_ )
      {
        Node* nn = nd->search( path, offset );

        if ( nn ) return nn;
      }

      return 0;
    }
  }

  return 0;
}

int
Fl_Preferences::Node::nChildren()
{
  if (indexed_)
  {
    return nIndex_;
  }

  else
  {
    int cnt = 0;

    for ( Node* nd = child_; nd; nd = nd->next_ )
      cnt++;

    return cnt;
  }
}

const char*
Fl_Preferences::Node::name()
{
  if ( path_ )
  {
    char* r = strrchr( path_, '/' );
    return r ? r + 1 : path_ ;
  }

  else
  {
    return 0L ;
  }
}

const char*
Fl_Preferences::Node::child( int ix )
{
  Node* nd = childNode( ix );

  if ( nd )
    return nd->name();

  else
    return 0L ;
}

Fl_Preferences::Node*
Fl_Preferences::Node::childNode( int ix )
{
  createIndex();

  if (indexed_)
  {
    return index_[ix];
  }

  else
  {
    int n = nChildren();
    ix = n - ix - 1;
    Node* nd;

    for ( nd = child_; nd; nd = nd->next_ )
    {
      if ( !ix-- ) break;

      if ( !nd ) break;
    }

    return nd;
  }
}

char
Fl_Preferences::Node::remove()
{
  Node* nd = 0, *np;

  if ( parent() )
  {
    nd = parent()->child_;
    np = 0L;

    for ( ; nd; np = nd, nd = nd->next_ )
    {
      if ( nd == this )
      {
        if ( np )
          np->next_ = nd->next_;

        else
          parent()->child_ = nd->next_;

        break;
      }
    }

    parent()->dirty_ = 1;
    parent()->updateIndex();
  }

  delete this;
  return ( nd != 0 );
}

void
Fl_Preferences::Node::createIndex()
{
  if (indexed_) return;

  int n = nChildren();

  if (n > NIndex_)
  {
    NIndex_ = n + 16;
    index_ = (Node**)realloc(index_, NIndex_ * sizeof(Node**));
  }

  Node* nd;
  int i = 0;

  for (nd = child_; nd; nd = nd->next_, i++)
  {
    index_[n - i - 1] = nd;
  }

  nIndex_ = n;
  indexed_ = 1;
}

void
Fl_Preferences::Node::updateIndex()
{
  indexed_ = 0;
}

void
Fl_Preferences::Node::deleteIndex()
{
  if (index_) free(index_);

  NIndex_ = nIndex_ = 0;
  index_ = 0;
  indexed_ = 0;
}

static unsigned char
x2i(char hi, char lo)
{
  return ((hi - 'A') << 4) | (lo - 'A');
}

static void
i2x(unsigned char v, char* d)
{
  d[0] = ((v >> 4) & 0x0f) + 'A';
  d[1] = (v & 0x0f) + 'A';
}

static void*
a2p(const char* s)
{
  union
  {
    void* ret;
    unsigned char d[sizeof(void*)];
  } v;
  v.ret = 0L;
  int i = 0, n = sizeof(void*);

  for (i = 0; i < n; i++)
  {
    v.d[i] = x2i(s[2 * i], s[2 * i + 1]);
  }

  return v.ret;
}

static void
p2a(void* vp, char* d)
{
  union
  {
    void* vp;
    unsigned char s[sizeof(void*)];
  } v;
  v.vp = vp;
  int i = 0, n = sizeof(void*);

  for (i = 0; i < n; i++)
  {
    i2x(v.s[i], d + i * 2);
  }

  d[2 * i] = 0;
}
