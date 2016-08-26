win_bison.exe -d yacc.y -t -v --debug
del yacc.tab.cpp
rename yacc.tab.c yacc.tab.cpp