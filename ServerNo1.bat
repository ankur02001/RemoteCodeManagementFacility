@echo off
Title SERVER-1

echo *************************************************************************
echo Running SERVER Number 1
echo SERVER-1 
echo *************************************************************************
".\CppCliClient\Debug\ExecutiveServer.exe" "COMMAND:UploadFile" "SRCIP:localhost" "SRCPORT:8080" "FILENAME:.\\TEST_Folder\\TextDocument3.txt" "FILETYPE:binary" "DSTIP:localhost" "DISPORT:8100"
