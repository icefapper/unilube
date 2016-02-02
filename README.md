####Description
A utility for extracting character info found in the Unicode Character Database

####Usage
unilube /the/path/to/ucd/xml/file @<character-trait1>:<expected-value1>@<character-trait2>:<expected-value2>...

####Output
The output is the "cp" of the characters that match the criteria given

####Example
Prerequisite: ucd.all.flat.xml from [unicode.org](http://www.unicode.org/Public/8.0.0/ucdxml/ucd.all.flat.zip) 

(Please note, this example assumes the afore said xml is in the current directory)

To extract all code points whose IDS value is Y ( i.e, all code points belonging to "ID_Start"), do
```
icefapper@pard $ unilube @IDS:Y ucd.all.flat.xml 
```

Or to find all cp's that are both in ID_Start and ID_Continue, do
```
icefapper@pard $ unilube @IDS:Y@IDC:Y ucd.all.flat.xml
```

Thanks a lot for reading this far.
