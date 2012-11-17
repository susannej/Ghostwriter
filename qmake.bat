set path=C:\Qt\4.7.3\bin;%path%
qmake.exe -v
qmake.exe -o GhostwriterPlugin.vcproj -t vclib GhostwriterPlugin.pro
pause

