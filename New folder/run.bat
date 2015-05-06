@echo off
cls
echo
echo *************************************************************************
echo Demonstaring Remote Code Management Facility
echo *************************************************************************
echo
echo
echo *************************************************************************
echo For Demonstrating project requirement 3 : 
echo This Demonstration Creates Two Client WPF And Two Servers with Following Parameters
echo 
echo     Server1 : Listens at port 8080 
echo     Server2 : Listens at port 8081 
echo     Client1 : Listens at port 8100 or 8200
echo     Client2 : Listens at port 8100 or 8200
echo This Supports uploading and downloading source code files Based on Categories like CPP , JAVA , TEXT , IMAGE
echo This Also gives option for File search 
echo This Also gives option for Text Search
echo This also gives option for concurrent string search in that Category
echo *************************************************************************
echo
echo *************************************************************************
echo For Demonstrating project requirement 4 : 
echo Created Windows Presentation Foundation (WPF) to build a Graphical User Interface
echo which supports file transfer and processing requests and displaying results to Status bar and performing information
echo *************************************************************************
echo
echo *************************************************************************
echo For Demonstrating project requirement 6 And 5: 
echo Client Can Choose Any Server1 or Server2 or both. With Text to be Search / Regular Expression and WildCards 
echo Which will Returns list of files with text and paths and machine on which it found in list box.
echo *************************************************************************
echo
echo *************************************************************************
echo For Demonstrating project requirement 7 : 
echo For file search file with patterns And Category From server 
echo Return a list of files with their All paths from which server
echo *************************************************************************

start ServerNo1
start ClientNo1
start ServerNo2
start ClientNo2