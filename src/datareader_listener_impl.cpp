#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include "datareader_listener_impl.h"
#include "MessengerTypeSupportC.h"
#include "MessengerTypeSupportImpl.h"

#include <iostream>

using namespace std;
using namespace DDS;
void
DataReaderListenerImpl::on_requested_deadline_missed(
  DataReader_ptr /*reader*/,
  const RequestedDeadlineMissedStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_requested_incompatible_qos(
  DataReader_ptr /*reader*/,
  const RequestedIncompatibleQosStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_sample_rejected(
  DataReader_ptr /*reader*/,
  const SampleRejectedStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_liveliness_changed(
  DataReader_ptr /*reader*/,
  const LivelinessChangedStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_data_available(DataReader_ptr reader)
{
  Messenger::MessageDataReader_var reader_i =
    Messenger::MessageDataReader::_narrow(reader);

  if (!reader_i) {
    cerr << "_narrow failed\n";
  }

  Messenger::Message message;
  SampleInfo info;

  ReturnCode_t error = reader_i->take_next_sample(message, info);

  if (error == RETCODE_OK) 
  {
    cout << "SampleInfo.sample_rank = " << info.sample_rank << endl;
    cout << "SampleInfo.instance_state = " << info.instance_state << endl;

    if (info.valid_data) 
    {
      cout << "Message: subject    = " << message.subject.in() << endl
           << "         subject_id = " << message.subject_id   << endl
           << "         from       = " << message.from.in()    << endl
           << "         count      = " << message.count        << endl
           << "         text       = " << message.text.in()    << endl;
    }
  } 
  else 
  {
    cerr << "take_next_sample failed!\n";
  }
}

void
DataReaderListenerImpl::on_subscription_matched(
  DataReader_ptr /*reader*/,
  const SubscriptionMatchedStatus& /*status*/)
{
}

void
DataReaderListenerImpl::on_sample_lost(
  DataReader_ptr /*reader*/,
  const SampleLostStatus& /*status*/)
{
}

