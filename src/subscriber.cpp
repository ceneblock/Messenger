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
#include <thread>

#include "datareader_listener_impl.h"

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
      cerr << "create_participant failed.\n";
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
    // Create the subscriber    
    DDS::Subscriber_var sub = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
                                                             0,   // No listener required
                                                             OpenDDS::DCPS::DEFAULT_STATUS_MASK);    
    
    if (!sub) 
    { 
      cerr << "Failed to create_subscriber.\n";
      return EXIT_FAILURE;
    }

    DataReaderListener_var listener(new DataReaderListenerImpl);

    DataReader_var dr = sub->create_datareader(topic,
                                               DATAREADER_QOS_DEFAULT,
                                               listener,
                                               OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (!dr) 
    {      
      cerr << "create_datareader failed.\n";
      return EXIT_FAILURE;
    }

    Messenger::MessageDataReader_var reader_i = Messenger::MessageDataReader::_narrow(dr);

    if (!reader_i)
    {
      cerr << "_narrow failed!\n";
      return EXIT_FAILURE;
    }

    // Block until Publisher completes
    StatusCondition_var condition = dr->get_statuscondition();
    condition->set_enabled_statuses(DDS::SUBSCRIPTION_MATCHED_STATUS);

    DDS::WaitSet_var ws = new WaitSet;
    ws->attach_condition(condition);

    while (true) 
    {
      SubscriptionMatchedStatus matches;
      if (dr->get_subscription_matched_status(matches) != RETCODE_OK) 
      {
        cerr << "get_subscription_matched_status failed!\n";
        return EXIT_FAILURE;
      }

      if (matches.current_count == 0 && matches.total_count > 0) {
        break;
      }

      ConditionSeq conditions;
      Duration_t timeout = { 60, 0 };
      if (ws->wait(conditions, timeout) != RETCODE_OK) 
      {
        cerr << "wait failed!\n";
      }
    }

    ws->detach_condition(condition);

    // Clean-up!
    participant->delete_contained_entities();
    dpf->delete_participant(participant);

    TheServiceParticipant->shutdown();
  }
  catch(...)
  {
    cout << "Uh oh!\n";
  }
  cout << "Hello World\n";
  return EXIT_SUCCESS;

}

