#include "MessengerTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include <ace/streams.h>
#include <ace/OS_NS_unistd.h>
#include <orbsvcs/Time_Utilities.h>

#include "dds/DCPS/StaticIncludes.h"
#include <iostream>
using namespace std;
using namespace DDS;
int main(int argc, char **argv)
{

  try {    
    DomainParticipantFactory_var dpf  = TheParticipantFactoryWithArgs(argc, argv);

    DomainParticipant_var participant = dpf->create_participant(42, // domain ID
                                                                PARTICIPANT_QOS_DEFAULT,
                                                                0,  // No listener required
                                                                OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!participant) 
    {      
      std::cerr << "create_participant failed." << std::endl;      
      return 1;    
    }
  }
  catch(...)
  {
    cerr << "Uh oh!\n";
  }
  
  cout << "Hello World\n";
  return EXIT_SUCCESS;
}
