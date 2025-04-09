pdfhandler - A console program that chunks a pdf into paragraph and prints them as json in the output

Uses poppler (https://poppler.freedesktop.org) and the Nlohmann Json library (https://github.com/nlohmann/json)

Build

Can be built with CMAKE and MSVC

Run in windows

Set 'POPPLERPATH' and 'POPPLERCPPPATH' variables in 'pdfhandler.bat' to the folder where you have built the Poppler and Poppler-cpp libraries.

Execute:
pdfhandler.bat <C:\Path\to\example.pdf>