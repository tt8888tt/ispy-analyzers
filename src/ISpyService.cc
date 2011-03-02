#include "ISpy/Analyzers/interface/ISpyService.h"
#include "ISpy/Services/interface/IgCollection.h"
#include "ISpy/Services/interface/IgArchive.h"

#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/Provenance/interface/Provenance.h"
#include "DataFormats/Provenance/interface/Timestamp.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <iostream>
#include <cstdio>
#include <sstream>

using namespace edm::service;

ISpyService::ISpyService (const ParameterSet& iPSet, ActivityRegistry& iRegistry)
  : outputFileName_(iPSet.getUntrackedParameter<std::string>( "outputFileName", std::string("default.ig"))),
    outputESFileName_(iPSet.getUntrackedParameter<std::string>( "outputESFileName", std::string("defaultES.ig"))),
    outputMaxEvents_(iPSet.getUntrackedParameter<int>( "outputMaxEvents", -1)),
    eventCounter_(0),
    fileCounter_(1),
    currentRun_(-1)
{
  iRegistry.watchPostBeginJob(this,&ISpyService::postBeginJob);
  iRegistry.watchPostEndJob(this,&ISpyService::postEndJob);

  iRegistry.watchPreProcessEvent(this,&ISpyService::preEventProcessing);
  iRegistry.watchPostProcessEvent(this,&ISpyService::postEventProcessing);

  init ();
}

void
ISpyService::init (void)
{}

void
ISpyService::postBeginJob (void)
{
  currentFile_[0] = outputFileName_;
  archive(currentFile_[0], zipFile0);
}

void
ISpyService::archive(const std::string &outputFileName, zipFile& zf)
{
  zf = zipOpen64(outputFileName.c_str(), APPEND_STATUS_CREATE);  
}

void
ISpyService::postEndJob(void)
{
  ziperr_ = zipClose(zipFile0, NULL);
  assert(ziperr_ == ZIP_OK);
}

void
ISpyService::finalize()
{
  ziperr_ = zipCloseFileInZip(zipFile0);
  assert(ziperr_ == ZIP_OK);
}

void
ISpyService::preEventProcessing(const edm::EventID& event, const edm::Timestamp& timestamp)
{
  currentRun_   = event.run();
  currentEvent_ = event.event();

  storages_[0] = new IgDataStorage;
  storages_[1] = new IgDataStorage;
}

void
ISpyService::postEventProcessing(const edm::Event& event, const edm::EventSetup& eventSetup)
{    
  if ( ! storages_[0]->empty() )
  {
    std::stringstream eoss;
    eoss << "Events/Run_" << currentRun_ << "/Event_" << currentEvent_;
	
    zip_fileinfo zi; // Do something with this 
    zi.tmz_date.tm_sec  = 0;
    zi.tmz_date.tm_min  = 0;
    zi.tmz_date.tm_hour = 0;
    zi.tmz_date.tm_mday = 0;
    zi.tmz_date.tm_mon  = 0;
    zi.tmz_date.tm_year = 0;
    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;
    
    ziperr_ = zipOpenNewFileInZip64(zipFile0, eoss.str().c_str(), &zi,
                                    NULL, 0, NULL, 0, NULL, // other stuff
                                    Z_DEFLATED, // method
                                    9, // compression level
                                    0);
    assert(ziperr_ == ZIP_OK);

    if (outputMaxEvents_ != -1)
      eventCounter_++;
      
    write(storages_[0]);

    finalize();
  }

  delete storages_[0];    
  storages_[0] = 0;
}
	
void 
ISpyService::write(IgDataStorage* storage)
{
  std::stringstream doss;
  doss << *storage; 
 
  long int size_buf = sizeof(std::string::size_type) + doss.str().length();
  void* buf = (void*) malloc(size_buf);
  memcpy((void*) buf, doss.str().c_str(), size_buf);

  ziperr_ = zipWriteInFileInZip(zipFile0, buf, size_buf);
  
  free(buf);
}

void
ISpyService::error (const std::string & what)
{
  assert (storages_[0]);
  IgCollection& collection = storages_[0]->getCollection("Errors_V1");
  IgProperty ERROR_MSG = collection.addProperty("Error", std::string());
  IgCollectionItem item = collection.create();
  item[ERROR_MSG] = what;
}

DEFINE_FWK_SERVICE(ISpyService);
