@echo off
Title "SERVER 2"
echo *************************************************************************
echo Running SERVER Number 2
echo SERVER-2 
echo *************************************************************************
".\CppCliClient\Debug\ExecutiveServer.exe" "COMMAND:UploadFile" "SRCIP:localhost" "SRCPORT:8081" "FILENAME:.\\TEST_Folder\\TextDocument3.txt" "FILETYPE:binary" "DSTIP:localhost" "DISPORT:8100"
