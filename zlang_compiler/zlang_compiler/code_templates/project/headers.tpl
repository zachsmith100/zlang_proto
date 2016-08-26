
#pragma once

[[cycle|allHeaders|header.tpl]]

#ifdef __APPLE__
[[cycle|macosxHeaders|header.tpl]]
#endif

#ifdef __unix__
[[cycle|linuxHeaders|header.tpl]]
#endif

#ifdef WIN32
[[cycle|windowsHeaders|header.tpl]]
#endif

#ifdef WIN32
[[cycle|windowsLibraries|library.tpl]]
#endif

