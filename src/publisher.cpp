#include "MessengerTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>

#include <dds/DCPS/PublisherImpl.h>
#include <ace/streams.h>
#include <ace/OS_NS_unistd.h>
#include <orbsvcs/Time_Utilities.h>

///Opps, not in the docs
#include <dds/DCPS/WaitSet.h>
#include "dds/DCPS/StaticIncludes.h"
#include <iostream>
using namespace std;
using namespace DDS;
int main(int argc, char **argv)
{

  try 
  {    
    DomainParticipantFactory_var dpf  = TheParticipantFactoryWithArgs(argc, argv);

    DomainParticipant_var participant = dpf->create_participant(42, // domain ID
                                                                PARTICIPANT_QOS_DEFAULT,
                                                                0,  // No listener required
                                                                OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!participant) 
    {      
      std::cerr << "create_participant failed.\n";
      return EXIT_FAILURE;    
    }

		Messenger::MessageTypeSupport_var mts = new Messenger::MessageTypeSupportImpl();  
    if (RETCODE_OK != mts->register_type(participant, "")) 
    {    
      cerr << "register_type failed.\n";    
      return EXIT_FAILURE;  
    }


    CORBA::String_var type_name = mts->get_type_name();    
    Topic_var topic = participant->create_topic ("Movie Discussion List",                                 
                                                      type_name,                                 
                                                      TOPIC_QOS_DEFAULT,                                 
                                                      0,   // No listener required                                
                                                      OpenDDS::DCPS::DEFAULT_STATUS_MASK);    
    
    if (!topic) 
    {
      cerr << "create_topic failed.\n";
      return EXIT_FAILURE;
    }

    Publisher_var pub = participant->create_publisher(PUBLISHER_QOS_DEFAULT,                                    
                                                           0,    // No listener required                                    
                                                           OpenDDS::DCPS::DEFAULT_STATUS_MASK);    
    
    if (!pub) 
    {      
      cerr << "create_publisher failed.\n";
      return EXIT_FAILURE;
    }


    // Create the datawriter    
    DataWriter_var writer =  pub->create_datawriter(topic,                             
                                                         DATAWRITER_QOS_DEFAULT,
                                                         0,    // No listener required
                                                         OpenDDS::DCPS::DEFAULT_STATUS_MASK);    
    
    if (!writer) 
    {      
      cerr << "create_datawriter failed.\n";
      return EXIT_FAILURE;
    }

    Messenger::MessageDataWriter_var message_writer = Messenger::MessageDataWriter::_narrow(writer);
    if(!message_writer)
    {
      cerr << "_narrow failed\n";
      return EXIT_FAILURE;
    }

    // Block until Subscriber is available    
    StatusCondition_var condition = writer->get_statuscondition();

    condition ->set_enabled_statuses(PUBLICATION_MATCHED_STATUS);
    
    WaitSet_var ws = new WaitSet;
    
    ws->attach_condition(condition);
    
    while (true)
    {
      PublicationMatchedStatus matches;
      if (writer->get_publication_matched_status(matches) != RETCODE_OK)
      {
        cerr << "get_publication_matched_status failed!\n";
        return EXIT_FAILURE;
      }
      
      if (matches.current_count >= 1)
      {
        break;
      }
      ConditionSeq conditions;
      Duration_t timeout = { 60, 0 };
      
      if (ws->wait(conditions, timeout) != RETCODE_OK)
      {
        cerr << "wait failed!\n";
        return EXIT_FAILURE;
      }
    }

    ws->detach_condition(condition);

    // Write samples
    Messenger::Message message;
    message.subject_id = 99;
    message.from       = "Comic Book Guy";
    message.subject    = "Review";
    message.text       = "Worst. Movie. Ever.";
    message.count      = 0;
    for (int i = 0; i < 10; ++i) 
    {
     ReturnCode_t error = message_writer->write(message, HANDLE_NIL);
     ++message.count;
     ++message.subject_id;
     if(error != RETCODE_OK)
     {
       // Log or otherwise handle the error condition
       return 1;
     }
    }
  }
  catch(...)
  {
    cerr << "Uh oh!\n";
  }
  
  cout << "Hello World\n";
  return EXIT_SUCCESS;
}
