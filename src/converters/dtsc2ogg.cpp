#include<iostream>
#include<vector>
#include <queue>
#include <stdlib.h>
#include "oggconv.h"

#include <mist/timing.h>
#include <mist/dtsc.h>
#include <mist/ogg.h>
#include <mist/theora.h>
#include <mist/vorbis.h>
#include <mist/config.h>
#include <mist/json.h>

namespace Converters{
  int DTSC2OGG(Util::Config & conf){
    DTSC::File DTSCFile(conf.getString("filename"));
    srand (Util::getMS());//randomising with milliseconds from boot
    std::vector<unsigned int> curSegTable;
    OGG::converter oggMeta;
    //Creating ID headers for theora and vorbis
    DTSC::readOnlyMeta fileMeta = DTSCFile.getMeta();
    DTSC::Meta giveMeta(fileMeta);
    
    oggMeta.readDTSCHeader(giveMeta);
    std::cout << oggMeta.parsedPages;//outputting header pages
   
    //create DTSC in OGG pages
    DTSCFile.parseNext();
    std::map< long long int, std::vector<JSON::Value> > DTSCBuffer;
    OGG::Page curOggPage;
    while(DTSCFile.getJSON()){
      std::string tmpString;
      oggMeta.readDTSCVector(DTSCFile.getJSON(), tmpString);
      std::cout << tmpString;
      DTSCFile.parseNext();
    }
    return 0;   
  }
}

int main(int argc, char ** argv){
  Util::Config conf = Util::Config(argv[0], PACKAGE_VERSION);
  conf.addOption("filename", JSON::fromString("{\"arg_num\":1, \"arg\":\"string\", \"help\":\"Filename of the DTSC file to analyse.\"}"));
  conf.parseArgs(argc, argv);
  return Converters::DTSC2OGG(conf);
}
