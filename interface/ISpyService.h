#ifndef ANALYZER_ISPY_SERVICE_H
#define ANALYZER_ISPY_SERVICE_H

#include <string>
#include <ISpy/Services/interface/zip.h>

class IgDataStorage;

namespace edm {
  class ActivityRegistry;
  class ParameterSet;
  class EventID;
  class Timestamp;
  class Event;
  class EventSetup;
    
  namespace service {
    class ISpyService
    {
    public:
      ISpyService (const edm::ParameterSet& pSet, ActivityRegistry& iRegistry);

      void 		postBeginJob (void);
      void 		postEndJob (void);
      void 		preEventProcessing (const edm::EventID&, const edm::Timestamp&);
      void 		postEventProcessing (const edm::Event&, const edm::EventSetup&);

      void              write(IgDataStorage* storage); 
      IgDataStorage * 	storage (void) { return storages_[0]; }
      IgDataStorage * 	esStorage (void) { return storages_[1]; }
      void		error (const std::string & what);

    private:
      void		init(void);
      void              archive(const std::string& name, zipFile& zf); 
      void		finalize(void);
	    
      const std::string outputFileName_;
      const std::string outputESFileName_;

      int		outputMaxEvents_;
    
      int		eventCounter_;	    
      int		fileCounter_;	    
      int		currentRun_;	    
      int		currentEvent_;
      
      zipFile           zipFile0; // Events
      zipFile           zipFile1; // Geometry
      IgDataStorage 	*storages_[2];
      std::string	currentFile_[2];	    
      int               ziperr_;
    };
  }
}

#endif // ANALYZER_ISPY_SERVICE_H
