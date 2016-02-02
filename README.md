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
 $ unilube @IDS:Y ucd.all.flat.xml 
```

Or to find all cp's that are both in ID_Start and ID_Continue, do
```
 $ unilube @IDS:Y@IDC:Y ucd.all.flat.xml
```

to get the most out of the output, you might want to consider using it along with  [bitlube](https://github.com/icefapper/bitlube) (which reads a list of numbers and creats a 0-indexed bit stream with bits at those indices set to 1 and the rest set to 0) and [lenlube](https://github.com/icefapper/lenlube) (which, when given a bitstream, would produce the corresponding run-length list for it.)

Thanks a lot for reading this far. 
